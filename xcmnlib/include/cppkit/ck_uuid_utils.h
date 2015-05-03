#ifndef cppkit_uuid_utils_h
#define cppkit_uuid_utils_h

#include "cppkit/ck_types.h"
#include "cppkit/os/ck_exports.h"

namespace cppkit
{

/// Sets all fields in a UUID to 0.
/// @param   pUuid A pointer to the UUID to clear.
CK_API void ck_uuid_clear( ck_uuid_bits* uuid );

/// Copies the UUID from one variable to another.
/// @param pSrc A pointer to the UUID to copy from.
/// @param pDest A pointer to the UUID to copy to.
CK_API void ck_uuid_copy( const ck_uuid_bits* src, ck_uuid_bits* dest );

/// Tests if two UUID values are equal.
/// @param pUuid1 A pointer to one of the UUIDs to test.
/// @param pUuid2 A pointer the other UUID to test.
/// @return true if the UUIDs are equal, false otherwise.
CK_API bool ck_uuid_is_equal( const ck_uuid_bits* uuid1, const ck_uuid_bits* uuid2 );

/// Tests if a UUID value is empty.
/// @param pUuid A pointer to the UUID to test.
CK_API bool ck_uuid_is_empty( const ck_uuid_bits* uuid );

/// Tests if a UUID string representation is the correct format.
/// @param pStr A pointer to the string to test.
CK_API bool ck_uuid_is_string_valid( const char* str );

/// Sets a UUID value from a string.
/// @param pStr A pointer to a NULL-terminated string representing the UUID
///             value.
/// @param pUuid A pointer to the UUID that will receive the converted value.
CK_API bool ck_uuid_from_string( const char* str, ck_uuid_bits* uuid );

/// Converts a UUID value to a string representation.
/// @param pUuid A pointer to the UUID to convert.
/// @param pStr A pointer to the buffer that will receive the string. The
///        buffer must be long enough to hold the string and the terminating
///        NULL character (ck_uuid_bits_STRING_LENGTH + 1).
/// @param len This value indicates how large the output buffer (pStr) is
///        in number of char characters. The routine verifies the length
///        and will report a failure if the buffer is too small to fit the
///        string form of the given UUID value. If this value is negative,
///        it is assumed that the buffer is large enough to hold the number
///        of characters required for a UUID (and a terminating NULL character
///        (i.e., ck_uuid_bits_STRING_LENGTH + 1). In this case, the caller is
///        responsible for always providing a buffer large enough for the
///        routine's output.
CK_API bool ck_uuid_to_string( const ck_uuid_bits* uuid, char* str, size_t len );

}

#endif
