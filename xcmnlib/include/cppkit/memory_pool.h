/*
  memory_pool.hh -- Copyright (c) 2013 Noel Cower. All rights reserved.
  See COPYING under the project root for the source code license. If this file
  is not present, refer to <https://raw.github.com/nilium/snow/master/COPYING>.
*/
#ifndef __SNOW__MEMORY_POOL_HH__
#define __SNOW__MEMORY_POOL_HH__


#include "../config.hh"
#include <mutex>
#include <cstddef>
#include <cstdint>


namespace snow {


/*!
  \file

  The API for creating, destroying, and allocating from memory pools. Memory
  pools are essentially linked lists that represent chunks of memory in a
  larger chunk of memory. The purpose of this is to ensure that, when
  necessary, memory that ought to be close together is. There is no solid way
  to reduce fragmentation, but ::pool_malloc will always attempt to use the next
  free or most recently freed block of memory for any new allocations.
*/


#define SBSFMT "%zu"
#define SBDFMT "%td"


using buffersize_t = size_t;
using bufferdiff_t = ptrdiff_t;


/*!
 * Memory block header. This is mainly for internal and debugging use.
 */
struct block_head_t
{
  /*! Whether the block is in use. Zero if not in use, one if a header
      block, otherwise a regular block. */
  int32_t used;
  /*! Identifying tag for the block. Zero if unused. */
  int32_t tag;
  /*! Size of the memory block. Includes header, memory guard, and
      alignment adjustment. */
  buffersize_t size;
  /*! Next block in the memory pool. */
  block_head_t *prev;
  /*! Previous block in the memory pool. */
  block_head_t *next;
  /*! Pointer back to the memory pool the block belongs to. */
  struct mempool_t *pool;

#if !NDEBUG

  /* debugging info for tracking allocations */
  struct
  {
    /* the source file this block was allocated from */
    char *source_file;
    /* the function this block was allocated from */
    char *function;
    /* the line in the source file that this block was allocated from */
    size_t line;
    /* the size requested (this always differs from the above size) */
    buffersize_t requested_size;
  } debug_info;

#endif
};


/*!
 * Memory pool structure. Do not touch its members unless you want to break stuff.
 */
struct mempool_t
{
  /*! Size of the memory pool. Includes adjustment for alignment. */
  buffersize_t size;
  /*! Counter for block allocation - can overflow. */
  int32_t sequence;
  /*! The memory used by the memory pool. */
  char *buffer;
  /*! Whether the buffer should be freed on destruction. If managed, free the
      memory. If not, do nothing to it. */
  bool managed;
  /*! The next free block of memory. */
  block_head_t *next_unused;
  /*! Header block - size is always 0, used is always 1, etc. */
  block_head_t head;
  /*! Pool lock */
  std::mutex lock;
};

/*!
 * Initializes the global memory pool.
 */
void sys_pool_init(void);

/*!
 * Destroys the global memory pool. This will not destroy any other pools.
 */
void sys_pool_shutdown(void);

/*!
 * Initializes a new memory pool.
 *
 * \param[inout]  pool The address of an uninitialized pool to be initialized.
 * \param[in]   size The size of the memory pool to initialize.
 */
int pool_init(mempool_t *pool, buffersize_t size);

/*!
 * Initializes a new memory pool with an existing block of memory.
 */
int pool_init_with_pointer(mempool_t *pool, void *p, buffersize_t size);

/*!
 * Destroys a memory pool.
 * @param pool The address of a previously-initialized pool to be destroyed.
 */
void pool_destroy(mempool_t *pool);

/*! \fn void *pool_malloc(mempool_t *pool, buffersize_t size, int32_t tag)
 * Allocates a buffer of the requested size with the given tag.
 *
 * \param[in] pool  The pool to allocate the memory from. If NULL, memory
 *  will be allocated from the global memory pool.
 * \param[in] size  The requested size of the buffer.
 * \param[in] tag   An identifying tag for the buffer. Cannot be zero.
 * \returns A new buffer of the requested size, or NULL if one cannot be
 *  allocated. In case of an error, a message will be written to stderr.
 */

#if !NDEBUG
#define pool_malloc(POOL, SIZE, TAG) pool_malloc_debug((POOL), (SIZE), (TAG), __FILE__, __FUNCTION__, __LINE__)
void *pool_malloc_debug(mempool_t *pool, buffersize_t size, int32_t tag, const char *file, const char *function, size_t line);
#else // !NDEBUG
void *pool_malloc(mempool_t *pool, buffersize_t size, int32_t tag);
#endif

/*!
 * Reallocates memory for the given pointer with the new buffer size.
 * Returns NULL on failure. If mem_realloc fails, the original buffer
 * is not freed.
 *
 * Unlike normal realloc, this will NOT allocate new memory if you
 * pass NULL as the original pointer. This is because, of course, there
 * is no pool associated with a NULL pointer.
 */
void *pool_realloc(void *p, buffersize_t size);

/*!
 * Frees a buffer previously allocated by ::pool_malloc.
 *
 * \param[in] buffer The buffer to be freed.
 */
void pool_free(void *buffer);

/*!
 * Frees all memory tagged with the given tag.
 */
void pool_free_tagged(mempool_t *pool, int32_t tag);

/*!
 * Frees all memory in the pool.
 *
 * \param[in] pool The pool to free memory in. If NULL, will release all blocks
 * in the global memory pool.
 */
void pool_free_all(mempool_t *pool);

size_t pool_allocated(mempool_t *pool);
size_t pool_unallocated(mempool_t *pool);
size_t pool_count_used_blocks(mempool_t *pool);
size_t pool_count_free_blocks(mempool_t *pool);

/*!
 * Gets the block header for a given buffer. This must be a block allocated
 * through ::pool_malloc. Attempting to use this with any other pointer is
 * liable to cause explosions.
 *
 * \remark This method will check for the block's memory guard. If the memory
 *  guard is corrupted, this function will return NULL, as it is no longer
 *  safe to interact with the block. Assume that something has gone
 *  horribly wrong if this happens.
 *
 * \param[in] buffer  The buffer to get the block header for.
 * \returns The address of the buffer's block header, or NULL if the block is
 *  suspected to be corrupt.
 */
const block_head_t *pool_block_for_pointer(const void *buffer);


} // namespace snow

#endif /* end of include guard: __SNOW__MEMORY_POOL_HH__ */
