/****************************************************************************
**
** Copyright (C) 2017 The veyesys.com.
** Contact: http://www.veyesys.com/licensing/
**
** This file is part of Rapidvms.
**
** Commercial License Usage
** Licensees holding valid commercial Rapidvms licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The veyesys. For licensing terms
** and conditions see http://www.veyesys.com/licensing/. For further
** information use the contact form at http://www.veyesys.com/licensing/.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/
#ifndef __RAPID_DEC_HPP__
#define __RAPID_DEC_HPP__
#include "utility/type.hpp"
#include "utility/videotype.hpp"
#include "stddef.h"
#include "ffkit/ffoptions.h"
#include "ffkit/av_demuxer.h"
#include "ffkit/av_packet.h"
#include "ffkit/av_packet_factory.h"
#include "ffkit/fflocky.h"

using namespace cppkit;
using namespace std;
using namespace ffkit;

class RapidDec
{
public:
	RapidDec(RMRawVideoHandler pHandler = NULL,  void * pVideoContext = NULL)
	:m_Init(FALSE), m_rawVideoHandler(pHandler), m_pRawVideoContext(pVideoContext)
	{
	}
	virtual ~RapidDec() {}
public:
	virtual BOOL Init(){return FALSE;}
	virtual BOOL Decode( uint8_t* pBuf, int nSize, RawFrame & pFrame){return FALSE;}

protected:

	BOOL m_Init;
	RMRawVideoHandler m_rawVideoHandler;
	void *m_pRawVideoContext;
};

#endif /* __RAPID_DEC_HPP__ */
