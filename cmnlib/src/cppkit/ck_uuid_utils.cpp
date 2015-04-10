
#include <cstring>
#include <cctype>
#include "cppkit/ck_types.h"
#include "cppkit/ck_uuid_utils.h"
#include "cppkit/ck_string.h"

using namespace cppkit;

#ifdef IS_WINDOWS
    // Save the current state regarding which warnings are enables. This state
    // is restored at the end of this file.
    #pragma warning(push)

    // Disables: "'strncpy': This function or variable may be unsafe." At the
    // time of this writing, safe string functions are not part of the C
    // standard.
    #pragma warning(disable:4996)
#endif

#define BITS_IN_NIBBLE 4
#define BITS_IN_BYTE 8
#define NIBBLE_BITMASK 0xF

#define BITS_TO_NIBBLES(x) ((x) >> 2)  // Divide by 4

// UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
// SECTION:     1      2    3    4        5
//
// Note: The index of the '-' character after each section is equal to
//       the section index + length.
#define UUID_SECT1_INDEX  0
#define UUID_SECT1_LENGTH 8
#define UUID_SECT2_INDEX  9
#define UUID_SECT2_LENGTH 4
#define UUID_SECT3_INDEX  14
#define UUID_SECT3_LENGTH 4
#define UUID_SECT4_INDEX  19
#define UUID_SECT4_LENGTH 4
#define UUID_SECT5_INDEX  24
#define UUID_SECT5_LENGTH 12

namespace cppkit
{

// Converts a single hex character into the corresponding 4-bit nibble.
static uint8_t char_to_hex( const char c );

// Converts a 4-bit nibble into the corresponding hex character.
static char hex_to_char( const uint8_t n );

// Sets all fields in a UUID to 0.
void ck_uuid_clear( ck_uuid_bits* uuid )
{
    ck_uuid_copy( &CK_EMPTY_UUID, uuid );
}

// Copies the UUID from one variable to another.
void ck_uuid_copy( const ck_uuid_bits* src, ck_uuid_bits* dest)
{
    dest->words.w1 = src->words.w1;
    dest->words.w2 = src->words.w2;
    dest->words.w3 = src->words.w3;
    dest->words.w4 = src->words.w4;
}

// Tests if two UUID values are equal.
bool ck_uuid_is_equal( const ck_uuid_bits* uuid1, const ck_uuid_bits* uuid2 )
{
    return (((uuid1->words.w1 == uuid2->words.w1)
        && (uuid1->words.w2 == uuid2->words.w2)
        && (uuid1->words.w3 == uuid2->words.w3)
        && (uuid1->words.w4 == uuid2->words.w4)) == true);
}

// Tests if a UUID value is empty.
bool ck_uuid_is_empty( const ck_uuid_bits* uuid )
{
    return ck_uuid_is_equal( uuid, &CK_EMPTY_UUID );
}

// Tests if a UUID string representation is the correct format.
bool ck_uuid_is_string_valid( const char* str )
{
    size_t ii = 0;
    size_t length = 0;

    // Make sure the string length is correct
    length = strlen(str);
    if( length != CK_UUID_STRING_LENGTH)
        return false;

    // Loop through each character and make sure it is valid
    for( ii=0; ii<length; ++ii )
    {
        switch( ii )
        {
        // The index of each '-' is found by adding the leading
        // section's index + length.
        case (UUID_SECT1_INDEX + UUID_SECT1_LENGTH):
        case (UUID_SECT2_INDEX + UUID_SECT2_LENGTH):
        case (UUID_SECT3_INDEX + UUID_SECT3_LENGTH):
        case (UUID_SECT4_INDEX + UUID_SECT4_LENGTH):
            // Make sure a '-' appears where we expect.
            if( str[ii] != '-')
                return false;
            break;
        default:
            // Make sure the current character is a valid hex value.
            if( !isxdigit(str[ii]))
                return false;
            break;
        }
    }

    return true;
}

// Sets a UUID value from a string.
bool ck_uuid_from_string( const char* str, ck_uuid_bits* uuid )
{
    uint8_t* uuid8 = uuid->uc;

    // Set the UUID to an empty value
    ck_uuid_clear( uuid );

    // Make sure the string format is valid
    if( !ck_uuid_is_string_valid( str ) )
        return false;

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //              ^
    *uuid8      = char_to_hex(*str) << BITS_IN_NIBBLE;      // 1
    *uuid8     |= char_to_hex(*(++str));                    // 2
    *(++uuid8)  = char_to_hex(*(++str)) << BITS_IN_NIBBLE;  // 3
    *uuid8     |= char_to_hex(*(++str));                    // 4
    *(++uuid8)  = char_to_hex(*(++str)) << BITS_IN_NIBBLE;  // 5
    *uuid8     |= char_to_hex(*(++str));                    // 6
    *(++uuid8)  = char_to_hex(*(++str)) << BITS_IN_NIBBLE;  // 7
    *uuid8     |= char_to_hex(*(++str));                    // 8

    // Skip the dash
    ++str;

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                     ^
    *(++uuid8)  = char_to_hex(*(++str)) << BITS_IN_NIBBLE;  // 1
    *uuid8     |= char_to_hex(*(++str));                    // 2
    *(++uuid8)  = char_to_hex(*(++str)) << BITS_IN_NIBBLE;  // 3
    *uuid8     |= char_to_hex(*(++str));                    // 4

    // Skip the dash
    ++str;

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                          ^
    *(++uuid8)  = char_to_hex(*(++str)) << BITS_IN_NIBBLE;  // 1
    *uuid8     |= char_to_hex(*(++str));                    // 2
    *(++uuid8)  = char_to_hex(*(++str)) << BITS_IN_NIBBLE;  // 3
    *uuid8     |= char_to_hex(*(++str));                    // 4

    // Skip the dash
    ++str;

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                               ^
    *(++uuid8)  = char_to_hex(*(++str)) << BITS_IN_NIBBLE;  // 1
    *uuid8     |= char_to_hex(*(++str));                    // 2
    *(++uuid8)  = char_to_hex(*(++str)) << BITS_IN_NIBBLE;  // 3
    *uuid8     |= char_to_hex(*(++str));                    // 4

    // Skip the dash
    ++str;

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                                        ^
    *(++uuid8)  = char_to_hex(*(++str)) << BITS_IN_NIBBLE;  // 1
    *uuid8     |= char_to_hex(*(++str));                    // 2
    *(++uuid8)  = char_to_hex(*(++str)) << BITS_IN_NIBBLE;  // 3
    *uuid8     |= char_to_hex(*(++str));                    // 4
    *(++uuid8)  = char_to_hex(*(++str)) << BITS_IN_NIBBLE;  // 5
    *uuid8     |= char_to_hex(*(++str));                    // 6
    *(++uuid8)  = char_to_hex(*(++str)) << BITS_IN_NIBBLE;  // 7
    *uuid8     |= char_to_hex(*(++str));                    // 8
    *(++uuid8)  = char_to_hex(*(++str)) << BITS_IN_NIBBLE;  // 9
    *uuid8     |= char_to_hex(*(++str));                    // 10
    *(++uuid8)  = char_to_hex(*(++str)) << BITS_IN_NIBBLE;  // 11
    *uuid8     |= char_to_hex(*(++str));                    // 12

    return true;
}

// Converts a UUID value to a string representation.
bool ck_uuid_to_string( const ck_uuid_bits* uuid, char* str, size_t len )
{
    const char* uuid_char = (const char*) uuid;

    // Verify that the buffer is large enough to hold the final string.
    if( len >= 0 && len < (CK_UUID_STRING_LENGTH + 1))
        return false;

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //              ^
    *str     = hex_to_char(*uuid_char >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = hex_to_char(*(++uuid_char) >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = hex_to_char(*(++uuid_char) >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = hex_to_char(*(++uuid_char) >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = '-';

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                     ^
    *(++str) = hex_to_char(*(++uuid_char) >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = hex_to_char(*(++uuid_char) >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = '-';

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                          ^
    *(++str) = hex_to_char(*(++uuid_char) >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = hex_to_char(*(++uuid_char) >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = '-';

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                               ^
    *(++str) = hex_to_char(*(++uuid_char) >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = hex_to_char(*(++uuid_char) >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = '-';

    // UUID:    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    //                                        ^
    *(++str) = hex_to_char(*(++uuid_char) >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = hex_to_char(*(++uuid_char) >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = hex_to_char(*(++uuid_char) >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = hex_to_char(*(++uuid_char) >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = hex_to_char(*(++uuid_char) >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = hex_to_char(*(++uuid_char) >> BITS_IN_NIBBLE);
    *(++str) = hex_to_char(*uuid_char);
    *(++str) = '\0';

    return true;
}

uint8_t char_to_hex(const char c)
{
    // For each character range, we subtract the base ASCII value for that range
    // from the input character. This normalizes the character value to 0. We
    // then optionally add a constant to scale the value to the desired output
    // range. For characters that represent numbers ('0' - '9'), we don't need
    // to scale the value because we want the output normalized to 0. The alpha
    // characters need to be scaled by 10 so 'A' = 10, 'B' = 11, and so on.

    if( (c >= '0') && (c <= '9'))
        return  (c - '0');
    else if( (c >= 'A') && (c <= 'F'))
        return ((c - 'A') + 10);
    else if( (c >= 'a') && (c <= 'f'))
        return ((c - 'a') + 10);

    return 0;
}

char hex_to_char(const uint8_t n)
{
    // For input values that fall within range of base 10 numbers, we simple add
    // the value to the '0' character. For input values outside this range, we
    // must normalize the value by subtracting 10 (this is because the ASCII
    // character set assumes base 10).

    uint8_t nibble = n & NIBBLE_BITMASK;
    if( nibble < 10)
        return ('0' + nibble);
    else
        return ('A' + (nibble - 10));
}

}

// Restore the compiler warning settings back the way they were.
#ifdef IS_WINDOWS
    #pragma warning(pop)
#endif
