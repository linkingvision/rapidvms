
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_X_UUID_H_
#define _XSDK_X_UUID_H_

#include "XSDK/Types.h"
#include "XSDK/XSharedLib.h"
#include "XSDK/UuidUtils.h"
#include "XSDK/XException.h"
#include "XSDK/XString.h"

//==============================================================================
// Certain MS compiler warnings can be disabled because they warn against
// things that are not a problem.
//==============================================================================
#ifdef IS_WINDOWS
    // Save the current state regarding which warnings are enables. This state
    // is restored at the end of this file.
    #pragma warning(push)

    // Disables "C++ exception specification ignored except to indicate a
    // function is not __declspec(nothrow)", related to ""
    // being declared on IXUnknown::QueryInterface.
    #pragma warning(disable:4290)
#endif

namespace XSDK
{
    //=========================================================================
    /// @brief Provides an object-oriented representation of a UUID.
    //=========================================================================
    class XUuid
    {
    public:
        //=====================================================================
        /// @brief Initializes the object with an empty UUID.
        //=====================================================================
        X_API XUuid();

        //=====================================================================
        /// @brief Initializes the object with a UUID value.
        ///
        /// @param uuid The UUID value to initialize with.
        //=====================================================================
        X_API XUuid( IN const X_UUID& uuid );

        //=====================================================================
        /// @brief Initializes the object with a UUID represented in string
        ///        format.
        ///
        /// @param pStr The string representing the UUID, in the form:
        ///             "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX".
        ///
        /// @throws XException (E_STRING_FORMAT) The string format is not
        ///                     valid.
        //=====================================================================
        X_API XUuid( IN const XSDK::XString& str );

        //=====================================================================
        /// @brief Copy constructor
        ///
        /// @param other A reference to the object to copy.
        //=====================================================================
        X_API XUuid( IN const XUuid& other );

        //=====================================================================
        /// @brief Destructor
        //=====================================================================
        X_API ~XUuid() throw();

        //=====================================================================
        /// @brief Gets the UUID value represented by this class.
        ///
        /// @returns A constant reference to the UUID value.
        //=====================================================================
        X_API const X_UUID& GetUuid() const;

        //=====================================================================
        /// @brief Sets the UUID to an empty value.
        //=====================================================================
        X_API void Clear();

        //=====================================================================
        /// @brief Returns a boolean indicating if the UUID value is empty.
        ///
        /// @returns true if the UUID is empty, false otherwise.
        //=====================================================================
        X_API bool IsEmpty() const;

        //=====================================================================
        /// @brief Tests a string to determine if it represents a valid UUID.
        ///
        ///        A valid UUID string must be formatted as
        ///        "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX", where each 'X'
        ///        represents a hexadecimal character.
        ///
        /// @param str A reference to the string to test.
        ///
        /// @returns true if the string represents a valid UUID, false
        ///          otherwise.
        //=====================================================================
        X_API static bool IsStringValid( IN const XSDK::XString& str );


        ///
        X_API static XSDK::XString Generate();

        //=====================================================================
        /// @brief Gets a string representation of the UUID.
        ///
        /// @returns A string representing the UUID in the format
        ///          "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX", where each 'X'
        ///          represents a hexadecimal character.
        //=====================================================================
        X_API XSDK::XString ToString() const;

        //=====================================================================
        /// @brief Set the UUID value from a string.
        ///
        ///        A valid UUID string must be formatted as
        ///        "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX", where each 'X'
        ///        represents a hexadecimal character.
        ///
        /// @param str A reference to the string containing the UUID.
        ///
        /// @throws XException (E_STRING_FORMAT) The specified string is not
        ///                     in the correct format.
        //=====================================================================
        X_API void FromString( IN const XSDK::XString& str );

        //=====================================================================
        /// @brief Sets the UUID of this object equal to the specified value.
        ///
        /// @param other A reference to the XUuid object to copy.
        ///
        /// @returns A reference to the current object.
        //=====================================================================
        X_API XUuid& operator=( IN const XUuid& other );

        //=====================================================================
        /// @brief Sets the UUID of this object equal to the specified value.
        ///
        /// @param other A reference to the UUID to copy.
        ///
        /// @returns A reference to the current object.
        //=====================================================================
        X_API XUuid& operator=( IN const X_UUID& other );

        //=====================================================================
        /// @brief Sets the UUID of this object equal to the specified value.
        ///
        /// @param other A reference to the XString object which represents a
        ///              UUID to set this object to.
        ///
        /// @returns A reference to the current object.
        //=====================================================================
        X_API XUuid& operator=( IN const XString& other );

        //=====================================================================
        /// @brief Tests to see if this object is equal to the specified UUID.
        ///
        /// @param other A reference to the XUuid object to compare against.
        ///
        /// @returns true if the UUIDs are equal, false otherwise.
        //=====================================================================
        X_API bool operator==( IN const XUuid& other ) const;

        //=====================================================================
        /// @brief Tests to see if this object is equal to the specified UUID.
        ///
        /// @param other A reference to the UUID to compare against.
        ///
        /// @returns true if the UUIDs are equal, false otherwise.
        //=====================================================================
        X_API bool operator==( IN const X_UUID& other ) const;

        //=====================================================================
        /// @brief Tests to see if this object is <b>NOT</b> equal to the
        ///        specified UUID.
        ///
        /// @param other A reference to the XUuid object to compare against.
        ///
        /// @returns true if the UUIDs are <b>NOT</b>equal, false otherwise.
        //=====================================================================
        X_API bool operator!=( IN const XUuid& other ) const;

        //=====================================================================
        /// @brief Tests to see if this object is <b>NOT</b> equal to the
        ///        specified UUID.
        ///
        /// @param other A reference to the UUID object to compare against.
        ///
        /// @returns true if the UUIDs are <b>NOT</b>equal, false otherwise.
        //=====================================================================
        X_API bool operator!=( IN const X_UUID& other ) const;

        //=====================================================================
        /// @brief Casts the object back to a X_UUID.
        ///
        /// @returns A const reference to the raw X_UUID value.
        //=====================================================================
        X_API operator const X_UUID&() const;

    private:
        //=====================================================================
        /// The current value of the UUID.
        //=====================================================================
        X_UUID _uuid;

    }; // XUuid

} // XSDK

//==============================================================================
// Restore the compiler warning settings back the way they were.
//==============================================================================
#ifdef IS_WINDOWS
    #pragma warning(pop)
#endif

#endif // _XSDK_X_UUID_H_

