
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_TYPES_H_
#define _XSDK_TYPES_H_

#include <cstdint>

//==============================================================================
// Definitions used to identify the build environment.
//
// Compiler (reference: http://predef.sourceforge.net/precomp.html):
//     IS_GCC     - GNU Compiler Collection
//     IS_MSC     - Microsoft Compiler
//
// Operating System:
//     IS_LINUX   - Target Linux platform.
//     IS_WINDOWS - Target Windows platform.
//
// Other:
//     IS_DEBUG     - Include debug support.
//     IS_KERNEL    - For building code for the Linux kernel environment.
//     IS_CPLUSPLUS - Building with a C++ compiler.
//     IS_EVEREST   - Must play nicely with legacy Pelco Everest code.
//==============================================================================

// Compilers
#ifdef __GNUC__
    #undef  IS_GCC
    #define IS_GCC
#elif  _MSC_VER
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
        #error ">> Unknown Architecture!"
    #endif

    #if defined(ARM)
        #ifndef _DM81XXARM
            #define LLONG_MAX (-9223372036854775807ll-1)
            #define LLONG_MIN (-9223372036854775807ll)
            #define ULLONG_MAX (18446744073709551615ll)
        #endif
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
        #error ">> Unknown Architecture!"
    #endif
#else
    #error ">> Unknown OS!"
#endif


// Other
#if (defined(IS_LINUX) && !defined(NDEBUG)) || defined(_DEBUG) || defined(DEBUG)
    #undef  IS_DEBUG
    #define IS_DEBUG
#endif
#if defined(_KERNEL_)
    #undef  IS_KERNEL
    #define IS_KERNEL
#endif
#if defined(__cplusplus)
    #undef  IS_CPLUSPLUS
    #define IS_CPLUSPLUS
#endif
#if defined(__EVEREST_TYPES_H__)
    #undef  IS_EVEREST
    #define IS_EVEREST
#endif

// Endianness
// We default to little endian, and only #define BIG_ENDIAN when necessary...

#if defined(ARM)
    #define XSDK_BIG_ENDIAN
#endif


//==============================================================================
// Include assert definitions.
//==============================================================================
#include "XSDK/Assert.h"

//==============================================================================
// Definitions of fixed-length datatypes separated by target platform.
//==============================================================================
//#if defined(IS_WINDOWS)
//#if !defined(XSDK_NO_TYPES)
//    typedef __int8             int8_t;
//    typedef unsigned __int8    uint8_t;
//    typedef __int16            int16_t;
//    typedef unsigned __int16   uint16_t;
//    typedef __int32            int32_t;
//    typedef unsigned __int32   uint32_t;
//    typedef __int64            int64_t;
//    typedef unsigned __int64   uint64_t;
//#endif
//#elif defined(IS_LINUX)
//    #include <inttypes.h>
//#else
//#error ">> Platform undefined!"

// If you get to this point, the specific target platform has not been
// defined. If you are targeting Windows or Linux, make sure the appropriate
// macro is defined (IS_WINDOWS or IS_LINUX respectively). If you are
// building for a platform not defined here, add the requisite type
// definitions and contact Pelco to have them added to the official release.
//#endif


#if !defined(IS_EVEREST)
    #ifndef true
        #define TRUE  1
    #endif
    #ifndef false
        #define FALSE 0
    #endif
    #ifndef NULL
        #define NULL  0
    #endif
#endif // IS_EVEREST


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
typedef struct _X_UUID_RFC
{
    uint32_t data1;    ///< The first 32-bits of the UUID
    uint16_t data2;    ///< The next 16-bits of the UUID
    uint16_t data3;    ///< The next 16-bits of the UUID
    union _data4        ///  The last 64-bits of the UUID
    {
        uint8_t  uc[8];///< The last 64-bits of the UUID as an array of bytes.
        uint64_t ull;  ///< The last 64-bits of the UUID as a single field.
    }
    data4;
}
X_UUID_RFC;


//==============================================================================
/// @brief Represents a Universally Unique Identifier (32-bit words layout).
///
/// This structure represents a Universally Unique Identifier (UUID) in a
/// format based on the 32-bit word size. This layout for the UUID memory is
/// primarily used when assigning and comparing equality between UUID values.
//==============================================================================
typedef struct _X_UUID_WORDS
{
    uint32_t w1;       ///< The first 32-bits of the UUID
    uint32_t w2;       ///< The second 32-bits of the UUID
    uint32_t w3;       ///< The third 32-bits of the UUID
    uint32_t w4;       ///< The forth 32-bits of the UUID
}
X_UUID_WORDS;


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
typedef union _X_UUID
{
    uint8_t        uc[16];     ///< Network order independent representation
    X_UUID_WORDS   words;      ///< 32-bit words representation
    X_UUID_RFC     rfc;        ///< RFC 4122 representation
}
X_UUID;


//==============================================================================
/// @brief The expected length of a string that represents a UUID. Format:
///        "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx".
//==============================================================================
#define X_UUID_STRING_LENGTH    36


//==============================================================================
/// @brief This namespace is used to encapsulate all functions and constants
///        defined by the Pelco Base code.
//==============================================================================
namespace XSDK
{
    //======================================================================
    /// @brief Defines the value of an empty UUID.
    //======================================================================
    const X_UUID X_EMPTY_UUID = {{0}};

    //======================================================================
    /// @brief Defines the type of value that is stored in a XVariant
    //======================================================================
    enum X_VARIANT_TYPE
    {
        XVARTYPE_EMPTY = 0,
        XVARTYPE_BOOL,
        XVARTYPE_CHAR,
        XVARTYPE_SIGNED_CHAR,
        XVARTYPE_UNSIGNED_CHAR,
        XVARTYPE_SIGNED_SHORT,
        XVARTYPE_UNSIGNED_SHORT,
        XVARTYPE_SIGNED_INT,
        XVARTYPE_UNSIGNED_INT,
        XVARTYPE_SIGNED_LONG,
        XVARTYPE_UNSIGNED_LONG,
        XVARTYPE_SIGNED_LONG_LONG,
        XVARTYPE_UNSIGNED_LONG_LONG,
        XVARTYPE_FLOAT,
        XVARTYPE_DOUBLE,
        XVARTYPE_LONG_DOUBLE,
        XVARTYPE_UUID,
        XVARTYPE_TEXT,
        XVARTYPE_BYTES,
        XVARTYPE_PTR
    };

} // XSDK

//==============================================================================
/// @brief Defines function argument modifiers to help document code.
///
///        IN and OUT can be used in a function declaration to make the
///        dataflow direction clear to the user. throw_any should be used
///        when a function does not specify what exceptions it throws (meaning
///        it can throw any exception). The purpose for this is to indicate that
///        the author intended to allow any exceptions to be thrown and it is
///        not just a simple oversight.
///
///        Here's an example on how to use these macros:
///        \n\n
///        Header file
///        -----------------
/// @code  void foo
///        (
///            IN  int  arg1,
///            OUT int* arg2
///        )
///        throw_any; @endcode
///        \n\n
///        In this example, the value of 'arg1' is provides by the caller,
///        while the value for 'arg2' is filled in by the function foo().
//==============================================================================
#ifndef IN
    #define IN
#endif
#ifndef OUT
    #define OUT
#endif
#define throw_any


//==============================================================================
/// @brief Compile-time assert that verifies all the length-specific datatypes
///        are really the size they say they are.
//==============================================================================
#ifdef IS_DEBUG
    struct __XbTypesAssert
    {
        unsigned int SizeofInt8  : sizeof(int8_t) == 1 && sizeof(uint8_t) == 1;
        unsigned int SizeofInt16 : sizeof(int16_t) == 2 && sizeof(uint16_t) == 2;
        unsigned int SizeofInt32 : sizeof(int32_t) == 4 && sizeof(uint32_t) == 4;
        unsigned int SizeofInt64 : sizeof(int64_t) == 8 && sizeof(uint64_t) == 8;
        unsigned int SizeofUuid  : sizeof(X_UUID) == 16;

#ifdef IS_64BIT
        unsigned int SizeofSizeT : sizeof(size_t) == 8;
#elif defined(IS_32BIT)
        unsigned int SizeofSizeT : sizeof(size_t) == 4;
#else
    #error ">> Unknown Architecture!"
#endif
    };
#endif // IS_DEBUG

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


// Enables the counting of references for each object type in XObjCount.
//#define COUNT_REFS

// Enables tracking of the stacks for objects' construction in XObjCount.
//#define TRACK_STACK

#endif //_XSDK_TYPES_H_
