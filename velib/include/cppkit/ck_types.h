
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

#ifndef cppkit_types_h
#define cppkit_types_h

#include <cstdint>

// Compilers
#ifdef __GNUC__
  #undef  IS_GCC
  #define IS_GCC
#elif _MSC_VER
  #undef  IS_MSC
  #define IS_MSC
#endif

// Operating System
#if defined(LINUX_OS)
  #undef  IS_LINUX
  #define IS_LINUX

  #include "sys/types.h"

  #if __WORDSIZE == 64
    #undef  IS_64BIT
    #define IS_64BIT
  #elif __WORDSIZE == 32
    #undef  IS_32BIT
    #define IS_32BIT
  #else
    #error "Unknown Architecture!"
  #endif

#elif defined(WIN32) || defined(WIN64) || defined(IS_MSC)
  #undef  IS_WINDOWS
  #define IS_WINDOWS

  // Architecture
  #ifdef WIN64
    #undef  IS_64BIT
    #define IS_64BIT
  #elif defined(WIN32)
    #undef  IS_32BIT
    #define IS_32BIT
  #else
    #error "Unknown Architecture!"
  #endif
#else
  #error "Unknown OS!"
#endif

#if (defined(IS_LINUX) && !defined(NDEBUG)) || defined(_DEBUG) || defined(DEBUG)
  #undef  IS_DEBUG
  #define IS_DEBUG
#endif

#ifdef _NOEXCEPT
#define noexcept _NOEXCEPT
#endif

//==============================================================================
/// @brief Represents a Universally Unique Identifier (RFC 4122 memory layout).
///
/// This structure represents a Universally Unique Identifier (UUID) in the
/// format laid out in the RFC 4122. While this representation is provided
/// as a convenience, it should be used with care. If not used carefully,
/// (i.e., when setting the uint32_t field or the uint16_t fields directly,
/// etc.) the structure can suffer from byte order problems. This UUID layout
/// most reflects "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" (hex digits) format
/// when UUIDs are represented in string format.
//==============================================================================
typedef struct _ck_uuid_rfc
{
  uint32_t data1;    ///< The first 32-bits of the UUID
  uint16_t data2;    ///< The next 16-bits of the UUID
  uint16_t data3;    ///< The next 16-bits of the UUID
  union _data4        ///  The last 64-bits of the UUID
  {
    uint8_t uc[8];///< The last 64-bits of the UUID as an array of bytes.
    uint64_t ull;  ///< The last 64-bits of the UUID as a single field.
  } data4;
} ck_uuid_rfc;

//==============================================================================
/// @brief Represents a Universally Unique Identifier (32-bit words layout).
///
/// This structure represents a Universally Unique Identifier (UUID) in a
/// format based on the 32-bit word size. This layout for the UUID memory is
/// primarily used when assigning and comparing equality between UUID values.
//==============================================================================
typedef struct _ck_uuid_words
{
  uint32_t w1;       ///< The first 32-bits of the UUID
  uint32_t w2;       ///< The second 32-bits of the UUID
  uint32_t w3;       ///< The third 32-bits of the UUID
  uint32_t w4;       ///< The forth 32-bits of the UUID
} ck_uuid_words;

//==============================================================================
/// @brief Represents a Universally Unique Identifier (UUID).
///
/// UUIDs are 128-bit values commonly used to identify objects
/// in a system. They can be represented as strings with the format
/// "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX", where each 'X' represents a
/// hexadecimal character. In some systems, UUIDs are referred to as GUIDs,
/// which stands for Globally Unique Identifier. When defining a UUID value,
/// the uint8_t array should be used in order to avoid byte order problems.
/// UUIDs should always be stored in the UUID memory in network byte order.
//==============================================================================
typedef union _ck_uuid_bits
{
    uint8_t uc[16]; /// Network order independent representation
    ck_uuid_words words; /// 32-bit words representation
    ck_uuid_rfc rfc; /// RFC 4122 representation
} ck_uuid_bits;

//==============================================================================
/// @brief The expected length of a string that represents a UUID. Format:
///        "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx".
//==============================================================================
#define CK_UUID_STRING_LENGTH 36

namespace cppkit
{

//======================================================================
/// @brief Defines the value of an empty UUID.
//======================================================================
const ck_uuid_bits CK_EMPTY_UUID = {{0}};

//======================================================================
/// @brief Defines the type of value that is stored in a XVariant
//======================================================================
enum CK_VARIANT_TYPE
{
  CK_VARTYPE_EMPTY = 0,
  CK_VARTYPE_BOOL,
  CK_VARTYPE_CHAR,
  CK_VARTYPE_SIGNED_CHAR,
  CK_VARTYPE_UNSIGNED_CHAR,
  CK_VARTYPE_SIGNED_SHORT,
  CK_VARTYPE_UNSIGNED_SHORT,
  CK_VARTYPE_SIGNED_INT,
  CK_VARTYPE_UNSIGNED_INT,
  CK_VARTYPE_SIGNED_LONG,
  CK_VARTYPE_UNSIGNED_LONG,
  CK_VARTYPE_SIGNED_LONG_LONG,
  CK_VARTYPE_UNSIGNED_LONG_LONG,
  CK_VARTYPE_FLOAT,
  CK_VARTYPE_DOUBLE,
  CK_VARTYPE_LONG_DOUBLE,
  CK_VARTYPE_UUID,
  CK_VARTYPE_TEXT,
  CK_VARTYPE_BYTES,
  CK_VARTYPE_PTR
};

}

#ifndef IN
  #define IN
#endif
#ifndef OUT
  #define OUT
#endif

//==============================================================================
/// @brief Disable unwanted compiler specific warnings.
//==============================================================================
#ifdef IS_WINDOWS
  // C4290: C++ exception specification ignored except
  // to indicate a function is not __declspec(nothrow)
  #pragma warning(disable : 4290)

  // C4800: 'type' : forcing value to bool 'true' or 'false' (performance warning)
  #pragma warning(disable : 4800)
#endif

#endif
