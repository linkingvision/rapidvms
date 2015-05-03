
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// cppkit - http://www.cppkit.org
/// Copyright (c) 2013, Tony Di Croce
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without modification, are permitted
/// provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and
///    the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
///    and the following disclaimer in the documentation and/or other materials provided with the
///    distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
/// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
/// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
/// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
/// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
/// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// The views and conclusions contained in the software and documentation are those of the authors and
/// should not be interpreted as representing official policies, either expressed or implied, of the cppkit
/// Project.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-

#ifndef cppkit_os_platform_h
#define cppkit_os_platform_h

#ifdef IS_WINDOWS
#pragma warning(disable:4127)
#endif

#ifdef IS_WINDOWS
  #define _WINSOCKAPI_
  #include <windows.h>
  #include <malloc.h>
#else
  #include <stdlib.h>
#endif

#define CK_MACRO_BEGIN do {
#define CK_MACRO_END }while(0)

#define CK_MACRO_END_LOOP_FOREVER }while(1)

#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))

// Aligned memory alocations
#ifdef IS_WINDOWS
  /// Allocates a buffer that's aligned to a power of 2.
  #define CK_NEW_ALIGNED_BUFFER( POINTER, SIZE, ALIGNMENT ) \
  CK_MACRO_BEGIN \
    POINTER = (char*)_aligned_malloc( SIZE, ALIGNMENT ); \
    if( POINTER == 0 ) \
      CK_THROW_CRITICAL(("Failed to allocate an aligned buffer.")); \
  CK_MACRO_END

  /// Frees an aligned buffer.
  #define CK_DELETE_ALIGNED_BUFFER( POINTER ) \
  CK_MACRO_BEGIN \
   _aligned_free( POINTER ); \
  CK_MACRO_END

  #define FULL_MEM_BARRIER MemoryBarrier

#else // not IS_WINDOWS

  /// @brief Allocates a buffer that's aligned to a power of 2 and a multiple of sizeof(void*).
  #define CK_NEW_ALIGNED_BUFFER( POINTER, SIZE, ALIGNMENT ) \
  CK_MACRO_BEGIN \
    size_t __alignment = (ALIGNMENT); \
    if (__alignment & (sizeof(void*) - 1)) \
        __alignment = (__alignment & ~(sizeof(void*) - 1)) + sizeof(void*); \
    size_t __size = (SIZE); \
    if (__size & (sizeof(void*) - 1)) \
        __size = (__size & ~(sizeof(void*) - 1)) + sizeof(void*); \
    if( posix_memalign( (void**)&(POINTER), __alignment, __size ) != 0 ) \
        CK_THROW_CRITICAL(("Failed to allocate an aligned buffer.")); \
  CK_MACRO_END

  /// Frees an aligned buffer.
  #define CK_DELETE_ALIGNED_BUFFER( POINTER ) \
  CK_MACRO_BEGIN \
   free( POINTER ); \
  CK_MACRO_END

  #define FULL_MEM_BARRIER __sync_synchronize

#endif

#endif
