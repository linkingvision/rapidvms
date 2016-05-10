/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		tpc.cpp
 *	DESCRIPTION:	TIP Cache for Database
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/jrd.h"
#include "../jrd/ods.h"
#include "../jrd/tra.h"
#include "../jrd/pag.h"
#include "../jrd/cch_proto.h"
#include "../jrd/lck_proto.h"
#include "../jrd/ods_proto.h"
#include "../jrd/tpc_proto.h"
#include "../jrd/tra_proto.h"


using namespace Firebird;

namespace Jrd {

TipCache::TipCache(Database* dbb)
	: m_dbb(dbb),
	  m_cache(*m_dbb->dbb_permanent)
{
}


TipCache::~TipCache()
{
	SyncLockGuard sync(&m_sync, SYNC_EXCLUSIVE, "TipCache::~TipCache");
	clearCache();
}


int TipCache::cacheState(thread_db* tdbb, TraNumber number)
{
/**************************************
 *
 *	T P C _ c a c h e _ s t a t e
 *
 **************************************
 *
 * Functional description
 *	Get the current state of a transaction in the cache.
 *
 **************************************/

	if (number && TRA_precommited(tdbb, number, number))
		return tra_precommitted;

	SyncLockGuard sync(&m_sync, SYNC_SHARED, "TipCache::cacheState");

	if (!m_cache.getCount())
	{
		SyncUnlockGuard unlock(sync);
		initializeTpc(tdbb, number);
	}

	// if the transaction is older than the oldest
	// transaction in our tip cache, it must be committed
	// hvlad: system transaction is always committed too

	TxPage* tip_cache = m_cache.front();
	if (number < tip_cache->tpc_base || number == 0)
		return tra_committed;

	// locate the specific TIP cache block for the transaction

	const ULONG trans_per_tip = m_dbb->dbb_page_manager.transPerTIP;
	const TraNumber base = number - number % trans_per_tip;

	FB_SIZE_T pos;
	if (m_cache.find(base, pos))
	{
		tip_cache = m_cache[pos];

		fb_assert(number >= tip_cache->tpc_base);
		fb_assert(tip_cache->tpc_base < MAX_TRA_NUMBER - trans_per_tip);
		fb_assert(number < (tip_cache->tpc_base + trans_per_tip));

		return TRA_state(tip_cache->tpc_transactions, tip_cache->tpc_base, number);
	}

	// Cover all possibilities by returning active

	return tra_active;
}


static inline bool check_state(int state, ULONG mask)
{
	return ((1 << state) & mask) != 0;
}

TraNumber TipCache::findStates(thread_db* tdbb, TraNumber minNumber, TraNumber maxNumber,
	ULONG mask, int& state)
{
/**************************************
 *
 *	T P C _ f i n d _ s t a t e s
 *
 **************************************
 *
 * Functional description
 *	Return the oldest transaction in the given state. Lookup in the
 *  [min_number, max_number) bounds.
 *  If not found, return zero and don't change value of "state".
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* const dbb = tdbb->getDatabase();
	CHECK_DBB(dbb);

	fb_assert(minNumber <= maxNumber);

	// Ensure that the TIP cache is extended to fit the requested transactions

	initializeTpc(tdbb, maxNumber);

	SyncLockGuard sync(&m_sync, SYNC_SHARED, "TipCache::findStates");

	const TxPage* tip_cache = m_cache.front();

	// Check for too old transactions (assumed committed)

	if (maxNumber < tip_cache->tpc_base)
		return 0;

	if (minNumber < tip_cache->tpc_base || minNumber == 0)
	{
		if (check_state(tra_committed, mask))
		{
			state = tra_committed;
			return minNumber;
		}

		minNumber = tip_cache->tpc_base;
	}

	bool check_precommitted = false;
	if (check_state(tra_precommitted, mask))
	{
		// If we looking for tra_precommitted only and there is no precommitted
		// transactions - return immediately

		SyncLockGuard syncPC(&dbb->dbb_pc_sync, SYNC_SHARED, "TipCache::findLimbo");
		if (dbb->dbb_pc_transactions != NULL)
			check_precommitted = true;
		else if (mask == (1 << tra_precommitted))
			return 0;
	}

	const ULONG trans_per_tip = m_dbb->dbb_page_manager.transPerTIP;
	const TraNumber base = minNumber - minNumber % trans_per_tip;

	// Scan the TIP cache and return the first (i.e. oldest) transaction in
	// state we are looking for

	FB_SIZE_T pos;
	if (m_cache.find(base, pos))
	{
		for (TraNumber number = minNumber;
			 pos < m_cache.getCount() && number < maxNumber;
			 pos++)
		{
			tip_cache = m_cache[pos];

			fb_assert(number >= tip_cache->tpc_base);
			fb_assert(tip_cache->tpc_base < MAX_TRA_NUMBER - trans_per_tip);

			for (; number < (tip_cache->tpc_base + trans_per_tip) && number < maxNumber;
				 number++)
			{
				if (check_precommitted && number && TRA_precommited(tdbb, number, number))
				{
					state = tra_precommitted;
					return number;
				}

				const int tx_state = TRA_state(tip_cache->tpc_transactions, tip_cache->tpc_base, number);
				if (check_state(tx_state, mask))
				{
					state = tx_state;
					return number;
				}
			}
		}
	}

	return 0;
}


void TipCache::initializeTpc(thread_db* tdbb, TraNumber number)
{
/**************************************
 *
 *	T P C _ i n i t i a l i z e _ t p c
 *
 **************************************
 *
 * Functional description
 *	At transaction startup, intialize the tip cache up to
 *	number.  This is used at TRA_start () time.
 *
 **************************************/

	SyncLockGuard sync(&m_sync, SYNC_EXCLUSIVE, "TipCache::initializeTpc");

	if (m_cache.isEmpty())
	{
		sync.unlock();
		cacheTransactions(tdbb, 0);
		return;
	}

	// If there is already a cache, extend it if required.
	// find the end of the linked list, and cache
	// all transactions from that point up to the most recent transaction

	const ULONG trans_per_tip = m_dbb->dbb_page_manager.transPerTIP;

	const TxPage* tip_cache = m_cache.back();

	fb_assert(tip_cache->tpc_base < MAX_TRA_NUMBER - trans_per_tip);
	if (number < (tip_cache->tpc_base + trans_per_tip))
		return;

	if (tip_cache->tpc_base < MAX_TRA_NUMBER - trans_per_tip)
	{
		// ensure last_known calculated *before* unlock !!!
		const TraNumber last_known = tip_cache->tpc_base;
		sync.unlock();

		cacheTransactions(tdbb, last_known + trans_per_tip);
	}
}


void TipCache::setState(TraNumber number, SSHORT state)
{
/**************************************
 *
 *	T P C _ s e t _ s t a t e
 *
 **************************************
 *
 * Functional description
 *	Set the state of a particular transaction
 *	in the TIP cache.
 *
 **************************************/

	const ULONG trans_per_tip = m_dbb->dbb_page_manager.transPerTIP;
	const TraNumber base = number - number % trans_per_tip;
	const ULONG byte = TRANS_OFFSET(number % trans_per_tip);
	const USHORT shift = TRANS_SHIFT(number);

	SyncLockGuard sync(&m_sync, SYNC_EXCLUSIVE, "TipCache::setState");

	FB_SIZE_T pos;
	if (m_cache.find(base, pos))
	{
		TxPage* tip_cache = m_cache[pos];

		fb_assert(number >= tip_cache->tpc_base);
		fb_assert(tip_cache->tpc_base < MAX_TRA_NUMBER - trans_per_tip);
		fb_assert(number < (tip_cache->tpc_base + trans_per_tip));

		UCHAR* address = tip_cache->tpc_transactions + byte;
		*address &= ~(TRA_MASK << shift);
		*address |= state << shift;
		return;
	}

	// right now we don't set the state of a transaction on a page
	// that has not already been cached -- this should probably be done
}


int TipCache::snapshotState(thread_db* tdbb, TraNumber number)
{
/**************************************
 *
 *	T P C _ s n a p s h o t _ s t a t e
 *
 **************************************
 *
 * Functional description
 *	Get the current state of a transaction.
 *	Look at the TIP cache first, but if it
 *	is marked as still alive we must do some
 *	further checking to see if it really is.
 *
 **************************************/

	fb_assert(m_dbb == tdbb->getDatabase());

	if (number && TRA_precommited(tdbb, number, number))
		return tra_precommitted;

	SyncLockGuard sync(&m_sync, SYNC_SHARED, "TipCache::snapshotState");

	if (m_cache.isEmpty())
	{
		sync.unlock();
		cacheTransactions(tdbb, 0);
		sync.lock(SYNC_SHARED, "TipCache::snapshotState");
	}

	// if the transaction is older than the oldest
	// transaction in our tip cache, it must be committed
	// hvlad: system transaction always committed too

	TxPage* tip_cache = m_cache.front();
	if (number < tip_cache->tpc_base || number == 0)
		return tra_committed;

	// locate the specific TIP cache block for the transaction

	const ULONG trans_per_tip = m_dbb->dbb_page_manager.transPerTIP;
	const TraNumber base = number - number % trans_per_tip;

	FB_SIZE_T pos;
	if (m_cache.find(base, pos))
	{
		tip_cache = m_cache[pos];

		fb_assert(number >= tip_cache->tpc_base);
		fb_assert(tip_cache->tpc_base < MAX_TRA_NUMBER - trans_per_tip);
		fb_assert(number < (tip_cache->tpc_base + trans_per_tip));

		const int state = TRA_state(tip_cache->tpc_transactions, tip_cache->tpc_base, number);

		sync.unlock();

		// committed or dead transactions always stay that
		// way, so no need to check their current state

		if (state == tra_committed || state == tra_dead)
			return state;

		// see if we can get a lock on the transaction; if we can't
		// then we know it is still active
		Lock temp_lock(tdbb, sizeof(TraNumber), LCK_tra);
		temp_lock.lck_key.lck_long = number;

		// If we can't get a lock on the transaction, it must be active.

		if (!LCK_lock(tdbb, &temp_lock, LCK_read, LCK_NO_WAIT))
		{
			fb_utils::init_status(tdbb->tdbb_status_vector);
			return tra_active;
		}

		fb_utils::init_status(tdbb->tdbb_status_vector);
		LCK_release(tdbb, &temp_lock);

		// as a last resort we must look at the TIP page to see
		// whether the transaction is committed or dead; to minimize
		// having to do this again we will check the state of all
		// other transactions on that page

		return TRA_fetch_state(tdbb, number);
	}
	// if the transaction has been started since we last looked, extend the cache upward

	sync.unlock();

	return extendCache(tdbb, number);
}


void TipCache::updateCache(const Ods::tx_inv_page* tip_page, ULONG sequence)
{
/**************************************
 *
 *	T P C _ u p d a t e _ c a c h e
 *
 **************************************
 *
 * Functional description
 *	A TIP page has been fetched into memory,
 *	so we should take the opportunity to update
 *	the TIP cache with the state of all transactions
 *	on that page.
 *
 **************************************/

	const ULONG trans_per_tip = m_dbb->dbb_page_manager.transPerTIP;
	const TraNumber first_trans = (TraNumber) sequence * trans_per_tip;

	// while we're in the area we can check to see if there are
	// any tip cache pages we can release--this is cheaper and
	// easier than finding out when a TIP page is dropped

	SyncLockGuard sync(&m_sync, SYNC_EXCLUSIVE, "TipCache::updateCache");

	TxPage* tip_cache = NULL;

	while (m_cache.hasData())
	{
		tip_cache = m_cache.front();

		fb_assert(tip_cache->tpc_base < MAX_TRA_NUMBER - trans_per_tip);
		if (m_dbb->dbb_oldest_transaction >= (tip_cache->tpc_base + trans_per_tip))
		{
			m_cache.remove((FB_SIZE_T) 0);
			delete tip_cache;
		}
		else
			break;
	}

	// find the appropriate page in the TIP cache and assign all transaction
	// bits -- it's not worth figuring out which ones are actually used

	FB_SIZE_T pos;
	if (m_cache.find(first_trans, pos))
		tip_cache = m_cache[pos];
	else
	{
		tip_cache = allocTxPage(first_trans);
		m_cache.insert(pos, tip_cache);
	}

	fb_assert(first_trans == tip_cache->tpc_base);

	const USHORT len = TRANS_OFFSET(trans_per_tip);
	memcpy(tip_cache->tpc_transactions, tip_page->tip_transactions, len);
}


TipCache::TxPage* TipCache::allocTxPage(TraNumber base)
{
/**************************************
 *
 *	a l l o c a t e _ t p c
 *
 **************************************
 *
 * Functional description
 *	Create a tip cache block to hold the state
 *	of all transactions on one page.
 *
 **************************************/
	fb_assert(m_sync.ourExclusiveLock());

	const ULONG trans_per_tip = m_dbb->dbb_page_manager.transPerTIP;

	// allocate a TIP cache block with enough room for all desired transactions

	TxPage* tip_cache = FB_NEW_RPT(*m_dbb->dbb_permanent, trans_per_tip / 4) TxPage();
	tip_cache->tpc_base = base;

	return tip_cache;
}


TraNumber TipCache::cacheTransactions(thread_db* tdbb, TraNumber oldest)
{
/**************************************
 *
 *	c a c h e _ t r a n s a c t i o n s
 *
 **************************************
 *
 * Functional description
 *	Cache the state of all the transactions since
 *	the last time this routine was called, or since
 *	the oldest interesting transaction.
 *
 **************************************/

	// m_sync should be unlocked here !

	// check the header page for the oldest and newest transaction numbers

#ifdef SUPERSERVER_V2
	const TraNumber top = m_dbb->dbb_next_transaction;
	const TraNumber hdr_oldest = m_dbb->dbb_oldest_transaction;
#else
	WIN window(HEADER_PAGE_NUMBER);
	const Ods::header_page* header = (Ods::header_page*) CCH_FETCH(tdbb, &window, LCK_read, pag_header);
	const TraNumber top = Ods::getNT(header);
	const TraNumber hdr_oldest = Ods::getOIT(header);
	CCH_RELEASE(tdbb, &window);
#endif

	// hvlad: No need to cache TIP pages below hdr_oldest just refreshed from
	// header page. Moreover our tip cache can now contain a gap between the last
	// cached tip page and new pages if our process was idle for long time

	oldest = MAX(oldest, hdr_oldest);

	// now get the inventory of all transactions, which will automatically
	// fill in the tip cache pages
	// hvlad: note, call below will call updateCache() which will acquire m_sync
	// in exclusive mode. This is the reason why m_sync must be unlocked at the
	// entry of this routine

	TRA_get_inventory(tdbb, NULL, oldest, top);

	SyncLockGuard sync(&m_sync, SYNC_EXCLUSIVE, "TipCache::updateCache");

	const ULONG trans_per_tip = m_dbb->dbb_page_manager.transPerTIP;

	while (m_cache.hasData())
	{
		TxPage* tip_cache = m_cache.front();

		fb_assert(tip_cache->tpc_base < MAX_TRA_NUMBER - trans_per_tip);
		if ((tip_cache->tpc_base + trans_per_tip) < hdr_oldest)
		{
			m_cache.remove((FB_SIZE_T) 0);
			delete tip_cache;
		}
		else
			break;
	}

	return hdr_oldest;
}


void TipCache::clearCache()
{
	fb_assert(m_sync.ourExclusiveLock());

	while (m_cache.hasData())
		delete m_cache.pop();
}


int TipCache::extendCache(thread_db* tdbb, TraNumber number)
{
/**************************************
 *
 *	e x t e n d _ c a c h e
 *
 **************************************
 *
 * Functional description
 *	Extend the transaction inventory page
 *	cache to include at least all transactions
 *	up to the passed transaction, and return
 *	the state of the passed transaction.
 *
 **************************************/

	// m_sync should be unlocked here !

	const ULONG trans_per_tip = m_dbb->dbb_page_manager.transPerTIP;

	// find the end of the linked list, and cache
	// all transactions from that point up to the
	// most recent transaction

	Sync sync(&m_sync, "extendCache");
	sync.lock(SYNC_SHARED);

	fb_assert(m_cache.hasData());
	TxPage* tip_cache = m_cache.back();

	if (tip_cache->tpc_base < MAX_TRA_NUMBER - trans_per_tip)
	{
		// ensure last_known calculated *before* unlock !!!
		const TraNumber last_known = tip_cache->tpc_base;
		sync.unlock();

		const TraNumber oldest = cacheTransactions(tdbb, last_known + trans_per_tip);
		if (number < oldest)
			return tra_committed;

		sync.lock(SYNC_SHARED);
	}

	// find the right block for this transaction and return the state

	const TraNumber base = number - number % trans_per_tip;

	FB_SIZE_T pos;
	if (m_cache.find(base, pos))
	{
		tip_cache = m_cache[pos];

		fb_assert(number >= tip_cache->tpc_base);
		fb_assert(tip_cache->tpc_base < MAX_TRA_NUMBER - trans_per_tip);
		fb_assert(number < (tip_cache->tpc_base + trans_per_tip));

		return TRA_state(tip_cache->tpc_transactions, tip_cache->tpc_base, number);
	}

	// we should never get to this point, but if we do the
	// safest thing to do is return active

	fb_assert(false);
	return tra_active;
}

} // namespace Jrd
