/*
  memory_pool.cc -- Copyright (c) 2013 Noel Cower. All rights reserved.
  See COPYING under the project root for the source code license. If this file
  is not present, refer to <https://raw.github.com/nilium/snow/master/COPYING>.
*/
#define __SNOW__MEMORY_POOL_CC__

#include "memory_pool.hh"
#include <cstdlib>
#include <cstdio>
#include <cstring>


namespace snow {


#define USE_MEMORY_GUARD 1


using guard_t = uint32_t;


/*!
 * The minimum size of a memory pool.  This number is kind of arbitrary, but by
 * default the minimum is the size of four minimum-size blocks.
 */
#define MIN_POOL_SIZE (MIN_BLOCK_SIZE * 4)
/*! Default memory pool size for main pools. */
#define DEFAULT_POOL_SIZE (128/*mb*/ * 1024/*kb*/ * 1024/*b*/)
/*! Alignment for memory blocks.  Must be a power of two minus one. */
#define BLOCK_ALIGNMENT (16)
#if USE_MEMORY_GUARD
#define MEMORY_GUARD_SIZE (sizeof(guard_t))
#else
#define MEMORY_GUARD_SIZE (0)
#endif /* USE_MEMORY_GUARD */
/*! Macro for quickly getting the actual size of a block given a requested size. */
#define BLOCK_SIZE(SZ) ((buffersize_t)((SZ) + sizeof(block_head_t) + (BLOCK_ALIGNMENT) & ~(BLOCK_ALIGNMENT - 1) + MEMORY_GUARD_SIZE))
/*! Minimum allocation size - defaults to larger of a pointer or size_t. */
#define MIN_ALLOC_SIZE (sizeof(void *) >= sizeof(size_t) ? sizeof(void *) : sizeof(size_t))
/*! Size of a block for a minimum-size allocation. */
#define MIN_BLOCK_SIZE BLOCK_SIZE(MIN_ALLOC_SIZE)
/*! Memory guard value - used to determine if something has written outside the
  bounds of a block. */
#define MEMORY_GUARD (0xD3ADBE3F)

#if !defined(MAIN_POOL_SIZE)
#define MAIN_POOL_SIZE DEFAULT_POOL_SIZE
#endif


namespace {


/*! The main memory pool. */
mempool_t g_main_pool;


} // namespace <anon>


/*!
 * Prints a block's properties to stderr.
 * @param block The block to write to stderr.
 */
static void dbg_print_block(const block_head_t *block);
/*!
 * Checks a pool's blocks for problems.
 * @param pool The pool to check.
 */
static void pool_check_for_errors(const mempool_t *pool);
/*!
 * Checks a block for problems (incorrect size, corrupt memory guard, NULL
 * pool/neighboring block pointers).
 * @param block The block to check.
 */
static void pool_check_block_for_errors(const block_head_t *block, int debug_block);
/*!
 * Returns whether a particular block can be split into a new block of
 * pred_size and its difference.
 */
static bool pool_can_split_block(block_head_t *block, buffersize_t pred_size);
/*!
 * Splits a block into two blocks, the original block sized pred_size and the
 * new block taking up the difference.
 */
static int pool_split_block(block_head_t *block, buffersize_t pred_size);
/*!
 * Combines two blocks into one. They must be seated next to each other, or
 * the function will return -1 (failure). Returns 0 on success.
 */
static int pool_merge_blocks(block_head_t *blka, block_head_t *blkb);

static int pool_set_up(mempool_t *pool, char *buffer, buffersize_t pool_size, bool managed);

static void pool_free_nolock(void *buffer);



void sys_pool_init()
{
  if (g_main_pool.head.used) return;

  pool_init(&g_main_pool, MAIN_POOL_SIZE);
}



void sys_pool_shutdown(void)
{
  if ( ! g_main_pool.head.used) return;

  pool_destroy(&g_main_pool);
}



static int pool_set_up(mempool_t *pool, char *buffer, buffersize_t pool_size, bool managed)
{
  if (pool_size < MIN_POOL_SIZE) {
    s_log_error("Attempt to allocate pool smaller than the minimum pool size.");
    return -1;
  }

  pool->lock.lock();

  pool->size = pool_size;
  pool->buffer = buffer;

  block_head_t *block = (block_head_t *)(((unsigned long)pool->buffer + (BLOCK_ALIGNMENT)) & ~(BLOCK_ALIGNMENT - 1));
  block->size = pool_size;
  block->used = 0;
  block->next = &pool->head;
  block->prev = &pool->head;
  block->pool = pool;

  pool->head.used = 1;
  pool->head.size = 0;
  pool->head.next = block;
  pool->head.prev = block;
  pool->head.pool = pool;

  pool->next_unused = block;
  pool->sequence = 1;

  pool->managed = managed;

  pool->lock.unlock();

  return 0;
}



int pool_init(mempool_t *pool, buffersize_t size)
{
  buffersize_t buffer_size;
  char *buffer;

  if (pool == NULL) {
    s_log_error("Attempt to initialize NULL memory pool.");
    return -1;
  } else if (size < MIN_POOL_SIZE) {
    s_log_warning("Attempt to initialize pool with size less than minimum pool size.");
    size = MIN_POOL_SIZE;
  }

  buffer_size = (size + BLOCK_ALIGNMENT) & ~(BLOCK_ALIGNMENT - 1);

  if ( ! pool->buffer) {
    buffer = (char *)malloc(buffer_size);

    if (buffer == NULL) {
      s_log_error("Failed to allocate buffer for memory pool.");

      return -1;
    }

    if (pool_set_up(pool, buffer, size, true)) {
      free(buffer);

      s_log_error("Failed to set up memory pool.");

      return -1;
    }

  } else {
    s_log_error("Attempt to initialize already-initialized memory pool (%p) with new", (const void *)pool);
    return -1;
  }

  return 0;
}



int pool_init_with_pointer(mempool_t *pool, void *p, buffersize_t size)
{
  if (pool == NULL) {
    s_log_error("Attempt to initialize NULL memory pool.");
    return -1;
  } else if (size < MIN_POOL_SIZE) {
    s_log_error("Attempt to create memory pool with too small outside buffer (must be " SBSFMT " or greater).",
      MIN_POOL_SIZE);
    return -1;
  } else if (p == NULL) {
    s_log_error("Attempt to initialize memory pool with NULL buffer.");
    return -1;
  }

  if (pool_set_up(pool, (char *)p, size, false)) {
    s_log_error("Failed to initialize memory pool.");
    return -1;
  }

  return 0;
}



void pool_destroy(mempool_t *pool)
{
  if (pool->head.used) {
    pool->lock.lock();

    pool_check_for_errors(pool);

    if (pool->managed)
      free(pool->buffer);

    pool->buffer = NULL;
    pool->head.next = NULL;
    pool->head.prev = NULL;
    pool->next_unused = NULL;
    pool->head.used = 0;
    pool->sequence = 0;
    pool->managed = false;

    pool->lock.unlock();
  } else {
    s_log_error("Attempt to destroy already-destroyed memory pool (%p)", (const void *)pool);
  }
}



static bool pool_can_split_block(block_head_t *block, buffersize_t pred_size)
{
  if (block->size < pred_size)
    return 0;

  return ((block->size - pred_size) > MIN_BLOCK_SIZE);
}



static int pool_split_block(block_head_t *block, buffersize_t pred_size)
{
  if (block->size < pred_size) {
    /* Original block is too small to split */
    return -1;
  }

  /* Check if the block can be split */
  if ((block->size - pred_size) > MIN_BLOCK_SIZE) {
    block_head_t *unused = (block_head_t *)((char *)block + pred_size);

    unused->size = block->size - pred_size;
    block->size = pred_size;

    unused->used = 0;
    unused->tag = 0;
    unused->pool = block->pool;
    /* Update list */
    unused->next = block->next;
    unused->prev = block;
    unused->next->prev = unused;
    block->next = unused;

    return 0;
    }

    /* New block is too small to split */
    return -1;
}



static int pool_merge_blocks(block_head_t *blka, block_head_t *blkb)
{
  if ( ! blka || ! blkb) {
    s_log_error("Attempt to join one or more NULL blocks.");
    return -1;
  }

  /* Swap blocks if they're out of order */
  if (blkb < blka) {
    block_head_t *swap = blka;
    blka = blkb;
    blkb = swap;
  }

  /* Make sure blocks are adjacent */
  if (blka->next != blkb) {
    s_log_error("Attempt to join non-adjacent memory blocks.");
    return -2;
  }

  blka->next = blkb->next;
  blka->next->prev = blka;
  blka->size += blkb->size;

  return 0;
}



#if NDEBUG
void *pool_malloc(mempool_t *pool, buffersize_t size, int32_t tag)
#else
void *pool_malloc_debug(mempool_t *pool, buffersize_t size, int32_t tag, const char *file, const char *function, size_t line)
#endif
{
  block_head_t *block = NULL;
  block_head_t *terminator = NULL;
  buffersize_t block_size;

  if (pool == NULL)
    pool = &g_main_pool;

  pool->lock.lock();

  if (tag == 0) {
    s_log_error("Allocation failed - invalid tag %X", tag);
    goto alloc_unlock_and_exit;
  }

  if ( ! pool->head.used) {
    s_log_error("Allocation failed - pool is not initialized or corrupt");
    goto alloc_unlock_and_exit;
  }

  block_size = BLOCK_SIZE(size);

  if (block_size < MIN_BLOCK_SIZE) {
    block_size = MIN_BLOCK_SIZE;
    s_log_warning("Allocation of %zu is too small, allocating minimum size of %zu instead", size, MIN_ALLOC_SIZE);
  }

  if (block_size > pool->size) {
    s_log_error("Allocation failed - requested size %zu exceeds pool capacity (%zu)", size, pool->size);
    goto alloc_unlock_and_exit;
  }


  block = pool->next_unused;

  if ( ! block) {
    s_log_error("Allocation failed - pool corrupted.");
    return NULL;
  }

  terminator = pool->next_unused->prev;

  if ( ! terminator) {
    s_log_error("Allocation failed - pool corrupted.");
    return NULL;
  }

  for (; block != terminator; block = block->next) {
    /* Skip used blocks and blocks that're too small */
    if (block->used)
      continue;
    else if (block->size < block_size)
      continue;

    /* If the free block is large enough to be split into two blocks, do that */
    if (pool_can_split_block(block, block_size))
      if (pool_split_block(block, block_size))
        s_log_error("Failed to split block, using unsplit block.");

    if (pool->sequence == 0)
      pool->sequence = 1; /* In case of overflow */

    block->used = ++pool->sequence;
    block->tag = tag;


#if USE_MEMORY_GUARD
    ((guard_t *)((char *)block + block_size))[-1] = MEMORY_GUARD;
#endif


#if !NDEBUG
    /* Store source file, function, line, and requested size in debugging struct */

    size_t file_length = strlen(file) + 1;
    size_t fn_length = strlen(function) + 1; /* Account for \0 in both cases */
    /* To avoid pointless pool fragmentation, allocate this using malloc normally */
    char *file_copy = (char *)malloc((file_length + fn_length) * sizeof(char));
    strncpy(file_copy, file, file_length + 1);
    block->debug_info.source_file = file_copy;

    /* Copy function as well */
    char *fn_copy = file_copy + file_length;
    strncpy(fn_copy, function, fn_length);
    block->debug_info.function = fn_copy;

    block->debug_info.line = line;
    block->debug_info.requested_size = size;
#endif /* !NDEBUG */

    pool->next_unused = block->next;

    pool->lock.unlock();

    return block + 1;
  }

  /* Out of memory */
  s_log_error("Failed to allocate %zu bytes - pool is out of memory", size);

alloc_unlock_and_exit:
  pool->lock.unlock();

  return NULL;
}



/*
  pool_realloc tries to make reallocation as cheap as possible by expanding its
  block to fit the new size. This is done by splitting previous/next blocks in
  the pool and merging those blocks.

  The best case scenario is that the new block is smaller. In that case, the
  block may not be resized at all.  It will be split if it can, but it's not
  an error if the block isn't resized at all.

  Second best case is that the next block in the pool can be split and merged
  with the original block. If it can't be split, it's not used, period.

  Third best case is that the previous block can be split and merged, with the
  original block memmove'd into place.

  Worst case is a new block is allocated, the contents memcpy'd to the new
  block, and then the old block is released.

  In the event of an error, NULL is returned and a log message is written
  describing what went wrong.
*/
void *pool_realloc(void *p, buffersize_t size)
{
  block_head_t *block;
  mempool_t *pool;
  size_t new_size;
  off_t size_diff;

  if ( ! p) {
    s_log_error("Realloc on NULL");
    return NULL;
  }

  block = (block_head_t *)p - 1;
  pool = block->pool;

  if ( ! pool) {
    s_log_error("Attempt to reallocate block without an associated pool");
    return NULL;
  }

  pool->lock.lock();

  if (block == &block->pool->head) {
    s_log_error("Realloc on header block of pool");
    p = NULL;
    goto pool_realloc_exit;
  }

  new_size = BLOCK_SIZE(size);

  if (new_size < MIN_BLOCK_SIZE)
    new_size = MIN_BLOCK_SIZE; /* New size cannot go below the minimum
                                    block size */

  size_diff = (off_t)new_size - (off_t)block->size;

  if (size_diff == 0)
    return p; /* Not resized at all */
  if (size_diff < 0 && abs(size_diff) < MIN_BLOCK_SIZE)
    return p; /* The size difference is small enough that resizing is
                 pointless, so don't bother */

  if (size_diff < 0) {
    /* New block is smaller, see if I can split it */
    if (pool_can_split_block(block, new_size)) {
      /* If I can split the block, do so */
      if (pool_split_block(block, new_size) == 0) {
        p = block + 1;

        if ( ! block->next->next->used) {
          pool_merge_blocks(block->next, block->next->next);
        }

        pool->next_unused = block->next;
      } else {
        s_log_warning("Failed to split block, using unsplit block.");
      }
    } else {
      s_log_warning("New block size is too small to resize, leaving as-is.");
    }

    /* If the block can't be split, leave it as is -- the size difference is
       small enough that resizing is probably pointless. */

  } else if ( ! block->next->used && (block->next->size - size_diff) >= MIN_BLOCK_SIZE) {
    /* If the next block is unused, try to join it with that.
       not using pool_split_block because the new block is the only one that
       has to be valid -- in a sense, it's more like moving a block. */
    block_head_t copy = *block->next;
    block_head_t *split = (block_head_t *)((char *)block->next + size_diff);

    /* Copy old to new */
    *split = copy;

    /* Reset pointers */
    split->next->prev = split;
    split->prev->next = split;

#if USE_MEMORY_GUARD
    /* Put memory guard in place (if in use) */
    ((guard_t *)((char *)block + new_size))[-1] = MEMORY_GUARD;
#endif /* USE_MEMORY_GUARD */

    /* Reset next unused pointer */
    pool->next_unused = split;

    /* Done */
    block->size = new_size;
  } else if ( ! block->prev->used) {
    /* If the last block is unused, try to join it with that */
    block_head_t *split = (block_head_t *)((char *)block->prev + (block->size - size_diff));
    block->prev->size -= size_diff;

    /* Copy old to new */
    memmove(split, block, block->size);

    /* Reset pointers */
    split->next->prev = split;
    split->prev->next = split;

#if USE_MEMORY_GUARD
    ((guard_t *)((char *)block + new_size))[-1] = MEMORY_GUARD;
#endif /* USE_MEMORY_GUARD */

    /* Again, reset next unused pointer */
    pool->next_unused = block->prev;

    block->size = new_size;
    p = block+1;
  } else {
    /* Last resort: allocate a new block, copy, free the old block */
    void *new_p = pool_malloc(block->pool, size, block->tag);

    if (new_p) {
      memcpy(new_p, p, block->size - sizeof(block_head_t));
      pool_free_nolock(p);
    } else {
      s_log_error("Failed to allocate new memory block for realloc");
    }

    p = new_p;
  }

  pool_realloc_exit:
  pool->lock.unlock();
  return p;
}



static void pool_free_nolock(void *buffer)
{
    block_head_t *block = (block_head_t *)buffer - 1;
    mempool_t *pool = block->pool;
  #if USE_MEMORY_GUARD
    guard_t guard;
  #endif

    if (block == &block->pool->head) {
      s_log_error("Free on header block of pool");
      return;
    }

    if (block->size < MIN_BLOCK_SIZE) {
      s_log_error("Invalid block, too small (%zu) - may be corrupted", block->size);
      return;
    }

  #if USE_MEMORY_GUARD
    guard = ((guard_t *)((char *)block + block->size))[-1];
    if (guard != MEMORY_GUARD) {
      s_log_error("Block memory guard corrupted - reads %X", guard);
    }
  #endif

    if ( ! block->used) {
      s_log_error("Double-free on block");
      return;
    }

    /*block->tag = DEFAULT_BLOCK_TAG_UNUSED;*/
    block->used = 0;

    if ( ! block->next->used) {
      block->size += block->next->size;
      block->next = block->next->next;
      block->next->prev = block;
    }

    if ( ! block->prev->used) {
      block = block->prev;
      block->size += block->next->size;
      block->next = block->next->next;
      block->next->prev = block;
    }

  #if !NDEBUG

    /* Clear debug info */
    free(block->debug_info.source_file);
    block->debug_info.source_file = NULL;
    block->debug_info.function = NULL;
    block->debug_info.line = -1;
    block->debug_info.requested_size = 0;

  #endif /* !NDEBUG */

    pool->next_unused = block;
}



void pool_free(void *buffer)
{
  if ( ! buffer) {
    s_log_error("Free on NULL");
    return;
  }

  block_head_t *block = (block_head_t *)buffer - 1;
  mempool_t *pool = block->pool;

  if ( ! pool) {
    s_log_error("Attempt to free block without an associated pool");
    return;
  }

  pool->lock.lock();
  pool_free_nolock(buffer);
  pool->lock.unlock();
}



void pool_free_tagged(mempool_t *pool, int32_t tag)
{
  if ( ! pool) {
    pool = &g_main_pool;
  }

  pool->lock.lock();

  block_head_t *block = pool->head.next;
  for (; block != &pool->head; block = block->next) {
    if (block->tag == tag) {
      pool_free_nolock(block);
      block = pool->next_unused;
    }
  }

  pool->lock.unlock();
}



void pool_free_all(mempool_t *pool)
{
  if ( ! pool) {
    pool = &g_main_pool;
  }

  pool->lock.lock();

  block_head_t *block = (block_head_t *)(((unsigned long)pool->buffer + (BLOCK_ALIGNMENT)) & ~(BLOCK_ALIGNMENT - 1));
  block->size = pool->size;
  block->used = 0;
  block->next = &pool->head;
  block->prev = &pool->head;
  block->pool = pool;

  pool->head.used = 1;
  pool->head.next = block;
  pool->head.prev = block;

  pool->next_unused = block;

  pool->lock.unlock();
}



size_t pool_allocated(mempool_t *pool)
{
  size_t size = 0;

  if ( ! pool) {
    pool = &g_main_pool;
  }

  pool->lock.lock();

  block_head_t *block = pool->head.next;
  for (; block != &pool->head; block = block->next) {
    if (block->used) {
      size += block->size;
    }
  }

  pool->lock.unlock();

  return size;
}



size_t pool_unallocated(mempool_t *pool)
{
  size_t size = pool->size;

  if ( ! pool) {
    pool = &g_main_pool;
  }

  pool->lock.lock();

  block_head_t *block = pool->head.next;
  for (; block != &pool->head; block = block->next) {
    if (block->used) {
      size -= block->size;
    }
  }

  pool->lock.unlock();

  return size;
}



size_t pool_count_used_blocks(mempool_t *pool)
{
  size_t count = 0;

  if ( ! pool) {
    pool = &g_main_pool;
  }

  pool->lock.lock();

  block_head_t *block = pool->head.next;
  for (; block != &pool->head; block = block->next) {
    if (block->used) {
      ++count;
    }
  }

  pool->lock.unlock();

  return count;
}



size_t pool_count_free_blocks(mempool_t *pool)
{
  size_t count = 0;

  if ( ! pool) {
    pool = &g_main_pool;
  }

  pool->lock.lock();

  block_head_t *block = pool->head.next;
  for (; block != &pool->head; block = block->next) {
    if (!block->used) {
      ++count;
    }
  }

  pool->lock.unlock();

  return count;
}



static void dbg_print_block(const block_head_t *block)
{
  s_log_note("BLOCK [header: %p | buffer: %p] {\n", (void *)block, (void *)(block+1));
#if USE_MEMORY_GUARD
  s_log_note("  guard: %X\n", ((const guard_t *)((const char *)block+block->size))[-1]);
#endif /* USE_MEMORY_GUARD */
  s_log_note("  block size: %zu bytes\n", block->size);
  s_log_note("  prev: %p\n", block->prev);
  s_log_note("  next: %p\n", block->next);
  s_log_note("  used: %d\n", block->used);
  s_log_note("  tag: %X\n", block->tag);
#if !NDEBUG
  s_log_note("  source file: %s [%d]\n", block->debug_info.source_file, block->debug_info.line);
  s_log_note("  source function: %s\n", block->debug_info.function);
  s_log_note("  buffer size: %zu bytes\n", block->debug_info.requested_size);
#endif /* !NDEBUG */
  s_log_note("  pool: %p\n}\n", block->pool);
}



static void pool_check_for_errors(const mempool_t *pool)
{
  if (pool == NULL) {
    s_throw(std::invalid_argument, "Attempt to check NULL pool.");
    return;
  }

  const block_head_t *block = pool->head.next;
  for (; block != &pool->head; block = block->next) {
    if (block) {
      pool_check_block_for_errors(block, block->used);
    } else {
      s_throw(std::runtime_error, "Memory pool links are corrupted.");
      return;
    }
  };
}



static void pool_check_block_for_errors(const block_head_t *block, int debug_block)
{
  if (block->pool && block == &block->pool->head) {
    s_log_error("Cannot check pool header block");
    return;
  }

  int spew_block = debug_block;

  if (block->size < MIN_BLOCK_SIZE) {
    s_log_error("Block smaller than minimum required size (%zu) - may be corrupt", MIN_BLOCK_SIZE);
  }

  if (block->next == NULL || block->prev == NULL) {
    s_log_error("Block detached from block list");
  }

#if USE_MEMORY_GUARD
  if (block->used) {
    guard_t guard = ((const guard_t *)((const char *)block + block->size))[-1];
    if (guard != MEMORY_GUARD) {
      s_log_error("Memory guard corrupted");
    }
  }
#endif

  if ( ! block->pool) {
    s_log_error("Block detached from memory pool");
  }

  if (spew_block) {
    dbg_print_block(block);
  }
}



const block_head_t *pool_block_for_pointer(const void *buffer)
{
  const block_head_t *block = (const block_head_t *)buffer - 1;

#if USE_MEMORY_GUARD
  guard_t guard = ((guard_t *)((const char *)buffer + block->size))[-1];
  if (guard != MEMORY_GUARD) {
    s_log_error("Memory guard corrupted");
    dbg_print_block(block);
    return NULL;
  }
#endif

  return block;
}


} // namespace snow

