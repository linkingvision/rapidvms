
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <cstring>
#include <cctype>
#include "XSDK/Types.h"
#include "XSDK/UuidUtils.h"
#include "XSDK/XString.h"

//==============================================================================
// Certain MS compiler warnings can be disabled because they warn against
// things that are not a problem.
//==============================================================================
#ifdef IS_WINDOWS
    // Save the current state regarding which warnings are enables. This state
    // is restored at the end of this file.
    #pragma warning(push)

    // Disables: "'strncpy': This function or variable may be unsafe." At the
    // time of this writing, safe string functions are not part of the C
    // standard.
    #pragma warning(disable:4996)
#endif

//==============================================================================
// Private definitions and helper method prototypes.
//==============================================================================

#define BITS_IN_NIBBLE       4
#define BITS_IN_BYTE         8
#define NIBBLE_BITMASK       0xF

#define BITS_TO_NIBBLES(x)   ((x) >> 2)  // Divide by 4

// UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
// SECTION:     1      2    3    4        5
//
// Note: The index of the '-' character after each section is equal to
//       the section index + length.
#define UUID_SECT1_INDEX     0
#define UUID_SECT1_LENGTH    8
#define UUID_SECT2_INDEX     9
#define UUID_SECT2_LENGTH    4
#define UUID_SECT3_INDEX     14
#define UUID_SECT3_LENGTH    4
#define UUID_SECT4_INDEX     19
#define UUID_SECT4_LENGTH    4
#define UUID_SECT5_INDEX     24
#define UUID_SECT5_LENGTH    12

//==============================================================================
// Converts a single hex character into the corresponding 4-bit nibble.
//==============================================================================
static uint8_t CharToHex(const char c);

//==============================================================================
// Converts a 4-bit nibble into the corresponding hex character.
//==============================================================================
static char HexToChar(const uint8_t n);

//==============================================================================
// Public UUID methods.
//==============================================================================

//==============================================================================
// Sets all fields in a UUID to 0.
//==============================================================================
void XUuidClear(IN X_UUID* pUuid)
{
    XUuidCopy(&XSDK::X_EMPTY_UUID, pUuid);
}

//==============================================================================
// Copies the UUID from one variable to another.
//==============================================================================
void XUuidCopy(IN const X_UUID* pSrc, OUT X_UUID* pDest)
{
    X_ASSERT_NO_LOG(pSrc  != NULL);
    X_ASSERT_NO_LOG(pDest != NULL);
    pDest->words.w1 = pSrc->words.w1;
    pDest->words.w2 = pSrc->words.w2;
    pDest->words.w3 = pSrc->words.w3;
    pDest->words.w4 = pSrc->words.w4;
}

//==============================================================================
// Tests if two UUID values are equal.
//==============================================================================
bool XUuidIsEqual(IN const X_UUID* pUuid1, IN const X_UUID* pUuid2)
{
    X_ASSERT_NO_LOG(pUuid1 != NULL);
    X_ASSERT_NO_LOG(pUuid2 != NULL);
    return (((pUuid1->words.w1 == pUuid2->words.w1)
        && (pUuid1->words.w2 == pUuid2->words.w2)
        && (pUuid1->words.w3 == pUuid2->words.w3)
        && (pUuid1->words.w4 == pUuid2->words.w4)) == true);
}

//==============================================================================
// Tests if a UUID value is empty.
//==============================================================================
bool XUuidIsEmpty(IN const X_UUID* pUuid)
{
    // We must use the PELCO_BASE namespace if compiling in C++.
    #ifdef __cplusplus
        return (XUuidIsEqual(pUuid, &XSDK::X_EMPTY_UUID));
    #else
        return (XUuidIsEqual(pUuid, &X_EMPTY_UUID));
    #endif
}

//==============================================================================
// Tests if a UUID string representation is the correct format.
//==============================================================================
bool XUuidIsStringValid(IN const char* pStr)
{
    size_t ii = 0;
    size_t length = 0;

    X_ASSERT_NO_LOG(pStr != NULL);

    // Make sure the string length is correct
    length = strlen(pStr);
    if (length != X_UUID_STRING_LENGTH)
        return false;

    // Loop through each character and make sure it is valid
    for (ii=0; ii<length; ++ii)
    {
        switch (ii)
        {
        // The index of each '-' is found by adding the leading
        // section's index + length.
        case (UUID_SECT1_INDEX + UUID_SECT1_LENGTH):
        case (UUID_SECT2_INDEX + UUID_SECT2_LENGTH):
        case (UUID_SECT3_INDEX + UUID_SECT3_LENGTH):
        case (UUID_SECT4_INDEX + UUID_SECT4_LENGTH):
            // Make sure a '-' appears where we expect.
            if (pStr[ii] != '-')
                return false;
            break;
        default:
            // Make sure the current character is a valid hex value.
            if (!isxdigit(pStr[ii]))
                return false;
            break;
        }
    }

    return true;
}

//==============================================================================
// Sets a UUID value from a string.
//==============================================================================
bool XUuidFromString(IN const char* pStr, OUT X_UUID* pUuid)
{
    uint8_t* pUuid8 = pUuid->uc;

    X_ASSERT_NO_LOG(pStr  != NULL);
    X_ASSERT_NO_LOG(pUuid != NULL);

    // Set the UUID to an empty value
    XUuidClear(pUuid);

    // Make sure the string format is valid
    if(!XUuidIsStringValid(pStr))
        return false;

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //              ^
    *pUuid8      = CharToHex(*pStr) << BITS_IN_NIBBLE;      // 1
    *pUuid8     |= CharToHex(*(++pStr));                    // 2
    *(++pUuid8)  = CharToHex(*(++pStr)) << BITS_IN_NIBBLE;  // 3
    *pUuid8     |= CharToHex(*(++pStr));                    // 4
    *(++pUuid8)  = CharToHex(*(++pStr)) << BITS_IN_NIBBLE;  // 5
    *pUuid8     |= CharToHex(*(++pStr));                    // 6
    *(++pUuid8)  = CharToHex(*(++pStr)) << BITS_IN_NIBBLE;  // 7
    *pUuid8     |= CharToHex(*(++pStr));                    // 8

    // Skip the dash
    ++pStr;

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                     ^
    *(++pUuid8)  = CharToHex(*(++pStr)) << BITS_IN_NIBBLE;  // 1
    *pUuid8     |= CharToHex(*(++pStr));                    // 2
    *(++pUuid8)  = CharToHex(*(++pStr)) << BITS_IN_NIBBLE;  // 3
    *pUuid8     |= CharToHex(*(++pStr));                    // 4

    // Skip the dash
    ++pStr;

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                          ^
    *(++pUuid8)  = CharToHex(*(++pStr)) << BITS_IN_NIBBLE;  // 1
    *pUuid8     |= CharToHex(*(++pStr));                    // 2
    *(++pUuid8)  = CharToHex(*(++pStr)) << BITS_IN_NIBBLE;  // 3
    *pUuid8     |= CharToHex(*(++pStr));                    // 4

    // Skip the dash
    ++pStr;

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                               ^
    *(++pUuid8)  = CharToHex(*(++pStr)) << BITS_IN_NIBBLE;  // 1
    *pUuid8     |= CharToHex(*(++pStr));                    // 2
    *(++pUuid8)  = CharToHex(*(++pStr)) << BITS_IN_NIBBLE;  // 3
    *pUuid8     |= CharToHex(*(++pStr));                    // 4

    // Skip the dash
    ++pStr;

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                                        ^
    *(++pUuid8)  = CharToHex(*(++pStr)) << BITS_IN_NIBBLE;  // 1
    *pUuid8     |= CharToHex(*(++pStr));                    // 2
    *(++pUuid8)  = CharToHex(*(++pStr)) << BITS_IN_NIBBLE;  // 3
    *pUuid8     |= CharToHex(*(++pStr));                    // 4
    *(++pUuid8)  = CharToHex(*(++pStr)) << BITS_IN_NIBBLE;  // 5
    *pUuid8     |= CharToHex(*(++pStr));                    // 6
    *(++pUuid8)  = CharToHex(*(++pStr)) << BITS_IN_NIBBLE;  // 7
    *pUuid8     |= CharToHex(*(++pStr));                    // 8
    *(++pUuid8)  = CharToHex(*(++pStr)) << BITS_IN_NIBBLE;  // 9
    *pUuid8     |= CharToHex(*(++pStr));                    // 10
    *(++pUuid8)  = CharToHex(*(++pStr)) << BITS_IN_NIBBLE;  // 11
    *pUuid8     |= CharToHex(*(++pStr));                    // 12

    return true;
}

//==============================================================================
// Converts a UUID value to a string representation.
//==============================================================================
bool XUuidToString(IN const X_UUID* pUuid, OUT char* pStr, IN size_t len)
{
    const char* pUuidChar = (const char*) pUuid;

    X_ASSERT_NO_LOG(pUuid != NULL);
    X_ASSERT_NO_LOG(pStr  != NULL);

    // Verify that the buffer is large enough to hold the final string.
    if (len >= 0 && len < (X_UUID_STRING_LENGTH + 1))
        return false;

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //              ^
    *pStr     = HexToChar(*pUuidChar >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = HexToChar(*(++pUuidChar) >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = HexToChar(*(++pUuidChar) >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = HexToChar(*(++pUuidChar) >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = '-';

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                     ^
    *(++pStr) = HexToChar(*(++pUuidChar) >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = HexToChar(*(++pUuidChar) >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = '-';

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                          ^
    *(++pStr) = HexToChar(*(++pUuidChar) >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = HexToChar(*(++pUuidChar) >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = '-';

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                               ^
    *(++pStr) = HexToChar(*(++pUuidChar) >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = HexToChar(*(++pUuidChar) >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = '-';

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                                        ^
    *(++pStr) = HexToChar(*(++pUuidChar) >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = HexToChar(*(++pUuidChar) >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = HexToChar(*(++pUuidChar) >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = HexToChar(*(++pUuidChar) >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = HexToChar(*(++pUuidChar) >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = HexToChar(*(++pUuidChar) >> BITS_IN_NIBBLE);
    *(++pStr) = HexToChar(*pUuidChar);
    *(++pStr) = '\0';

    return true;
}

//=============================================================================
// Private helper method implementations.
//=============================================================================

uint8_t CharToHex(const char c)
{
    // For each character range, we subtract the base ASCII value for that range
    // from the input character. This normalizes the character value to 0. We
    // then optionally add a constant to scale the value to the desired output
    // range. For characters that represent numbers ('0' - '9'), we don't need
    // to scale the value because we want the output normalized to 0. The alpha
    // characters need to be scaled by 10 so 'A' = 10, 'B' = 11, and so on.

    if ((c >= '0') && (c <= '9'))
        return  (c - '0');
    else if ((c >= 'A') && (c <= 'F'))
        return ((c - 'A') + 10);
    else if ((c >= 'a') && (c <= 'f'))
        return ((c - 'a') + 10);
    X_ASSERT_NO_LOG(false);
    return 0;
}

char HexToChar(const uint8_t n)
{
    // For input values that fall within range of base 10 numbers, we simple add
    // the value to the '0' character. For input values outside this range, we
    // must normalize the value by subtracting 10 (this is because the ASCII
    // character set assumes base 10).

    uint8_t nibble = n & NIBBLE_BITMASK;
    if (nibble < 10)
        return ('0' + nibble);
    else
        return ('A' + (nibble - 10));
}

//==============================================================================
// Restore the compiler warning settings back the way they were.
//==============================================================================
#ifdef IS_WINDOWS
    #pragma warning(pop)
#endif
