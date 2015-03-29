// Copyright (c) 2015, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef  TERA_UTILS_ATOMIC_H_
#define  TERA_UTILS_ATOMIC_H_

namespace tera {

static inline int atomic_add(volatile int *mem, int add)
{
#ifdef LEVELDB_PLATFORM_POSIX
    asm volatile(
            "lock xadd %0, (%1);"
            : "=a"(add)
            : "r"(mem), "a"(add)
            : "memory"
    );
#endif
    return add;
}

static inline int64_t atomic_add64(volatile int64_t* mem, int64_t add)
{
#ifdef LEVELDB_PLATFORM_POSIX
    asm volatile(
            "lock xaddq %0, (%1)"
            : "=a" (add)
            : "r" (mem), "a" (add)
            : "memory"
    );
#endif
    return add;
}

static inline void atomic_inc(volatile int *mem)
{
#ifdef LEVELDB_PLATFORM_POSIX
    asm volatile(
            "lock incl %0;"
            : "=m"(*mem)
            : "m"(*mem)
    );
#endif
}
static inline void atomic_inc64(volatile int64_t *mem)
{
#ifdef LEVELDB_PLATFORM_POSIX
    asm volatile(
            "lock incq %0;"
            : "=m"(*mem)
            : "m"(*mem)
    );
#endif
}

static inline void atomic_dec(volatile int *mem)
{
#ifdef LEVELDB_PLATFORM_POSIX
    asm volatile(
            "lock decl %0;"
            : "=m"(*mem)
            : "m"(*mem)
    );
#endif
}

static inline void atomic_dec64(volatile int64_t *mem)
{
#ifdef LEVELDB_PLATFORM_POSIX
    asm volatile(
            "lock decq %0;"
            : "=m"(*mem)
            : "m"(*mem)
    );
#endif
}

static inline int atomic_swap(volatile void *lockword, int value)
{
#ifdef LEVELDB_PLATFORM_POSIX
    asm volatile(
            "lock xchg %0, (%1);"
            : "=a"(value)
            : "r"(lockword), "a"(value)
            : "memory"
    );
#endif
    return value;
}

static inline int64_t atomic_swap64(volatile void *lockword, int64_t value)
{
#ifdef LEVELDB_PLATFORM_POSIX
    asm volatile(
            "lock xchg %0, (%1);"
            : "=a"(value)
            : "r"(lockword), "a"(value)
            : "memory"
    );
#endif
    return value;
}

static inline int atomic_comp_swap(volatile void *mem, int xchg, int cmp)
{
#ifdef LEVELDB_PLATFORM_POSIX
    asm volatile(
            "lock cmpxchg %1, (%2)"
            :"=a"(cmp)
            :"d"(xchg), "r"(mem), "a"(cmp)
    );
#endif
    return cmp;
}

static inline int64_t atomic_comp_swap64(volatile void *mem, int64_t xchg, int64_t cmp)
{
#ifdef LEVELDB_PLATFORM_POSIX
    asm volatile(
            "lock cmpxchg %1, (%2)"
            :"=a"(cmp)
            :"d"(xchg), "r"(mem), "a"(cmp)
    );
#endif
    return cmp;
}

}
#endif  // TERA_UTILS_ATOMIC_H_
