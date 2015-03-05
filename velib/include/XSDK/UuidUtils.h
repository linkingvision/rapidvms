
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_UUID_UTILS_H_
#define _XSDK_UUID_UTILS_H_

#include "XSDK/Types.h"
#include "XSDK/XSharedLib.h"

//==============================================================================
/// @brief   Sets all fields in a UUID to 0.
///
/// @param   pUuid A pointer to the UUID to clear.
//==============================================================================
X_API void XUuidClear(IN X_UUID* pUuid);

//==============================================================================
/// @brief   Copies the UUID from one variable to another.
///
/// @param   pSrc A pointer to the UUID to copy from.
///
/// @param   pDest A pointer to the UUID to copy to.
//==============================================================================
X_API void XUuidCopy(IN const X_UUID* pSrc, OUT X_UUID* pDest);

//==============================================================================
/// @brief   Tests if two UUID values are equal.
///
/// @param   pUuid1 A pointer to one of the UUIDs to test.
///
/// @param   pUuid2 A pointer the other UUID to test.
///
/// @return  true if the UUIDs are equal, false otherwise.
//==============================================================================
X_API bool XUuidIsEqual(IN const X_UUID* pUuid1, IN const X_UUID* pUuid2);

//==============================================================================
/// @brief   Tests if a UUID value is empty.
///
/// @param   pUuid A pointer to the UUID to test.
///
/// @return  true if the UUID is empty, false otherwise.
//==============================================================================
X_API bool XUuidIsEmpty(IN const X_UUID* pUuid);

//==============================================================================
/// @brief   Tests if a UUID string representation is the correct format.
///
/// @param   pStr A pointer to the string to test.
///
/// @return  true if the UUID is formatted correctly. false otherwise.
///
/// @note    The expected format of the string is
///          "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx". For example:
///          "bef9cf75-1f0b-45d0-82c7-3fe8fe97de21".
//==============================================================================
X_API bool XUuidIsStringValid(IN const char* pStr);

//==============================================================================
/// @brief   Sets a UUID value from a string.
///
/// @param   pStr A pointer to a NULL-terminated string representing the UUID
///          value.
///
/// @param   pUuid A pointer to the UUID that will receive the converted value.
///
/// @return  true if the string is the correct format and the operation
///          completed successfully. false if the format of 'pString' is not
///          valid.
///
/// @note    The expected format of the string is
///          "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx". For example:
///          "bef9cf75-1f0b-45d0-82c7-3fe8fe97de21".
//==============================================================================
X_API bool XUuidFromString(IN const char* pStr, OUT X_UUID* pUuid);

//==============================================================================
/// @brief   Converts a UUID value to a string representation.
///
/// @param   pUuid A pointer to the UUID to convert.
///
/// @param   pStr A pointer to the buffer that will receive the string. The
///          buffer must be long enough to hold the string and the terminating
///          NULL character (X_UUID_STRING_LENGTH + 1).
///
/// @param   len This value indicates how large the output buffer (pStr) is
///          in number of char characters. The routine verifies the length
///          and will report a failure if the buffer is too small to fit the
///          string form of the given UUID value. If this value is negative,
///          it is assumed that the buffer is large enough to hold the number
///          of characters required for a UUID (and a terminating NULL character
///          (i.e., X_UUID_STRING_LENGTH + 1). In this case, the caller is
///          responsible for always providing a buffer large enough for the
///          routine's output.
///
/// @return  true if the conversion was successful. false if 'pString' is not
///          large enough to hold the resulting string.
///
/// @note    The format of the converted UUID will be
///          "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx". For example:
///          "bef9cf75-1f0b-45d0-82c7-3fe8fe97de21". (A terminating NULL
///          will be appended to the end of the output string.)
//==============================================================================
X_API bool XUuidToString(IN const X_UUID* pUuid, OUT char* pStr, IN size_t len);

#endif // _XSDK_UUID_UTILS_H_
