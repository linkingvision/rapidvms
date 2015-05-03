
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _Webby_URI_h
#define _Webby_URI_h

#include "XSDK/XException.h"
#include "XSDK/XHash.h"
#include "XSDK/XString.h"

namespace WEBBY
{

/// \brief Holds the portion of a URI to the right of the IP and port.
///
/// If you want to have a full URI/URL, then use WEBBY::URL.
class URI
{
public:

    X_API URI()
        : _fullRawURI("/"),
          _getArgs(),
          _resource(),
          _resourcePath("/")
   {}

    X_API URI(const XSDK::XString& fullURI);

    // So string literals work when constructing with =
    X_API URI(const char* cstr) : _fullRawURI("/"), _getArgs(), _resource(), _resourcePath("/")
    { *this = URI(XSDK::XString(cstr)); }

    X_API URI(const URI& obj) :
        _fullRawURI(obj._fullRawURI),
        _getArgs(obj._getArgs),
        _resource(obj._resource),
        _resourcePath(obj._resourcePath)
    {}

    X_API virtual ~URI() throw() {}

    X_API static URI ConstructURI(const XSDK::XString& resourcePath,
                                   const XSDK::XString& resource);

    X_API static URI ConstructURI(const XSDK::XString& resourcePath,
                                   const XSDK::XString& resource,
                                   const XSDK::XHash<XSDK::XString>& getArgs);

    X_API bool operator==(const URI& other) const;

    X_API bool operator!=(const URI& other) const;

    X_API bool operator<(const URI& other) const;

    X_API XSDK::XString GetFullRawURI() const { return _fullRawURI; }

    X_API const XSDK::XHash<XSDK::XString>& GetGetArgs() const { return _getArgs; }

    X_API XSDK::XString GetResource() const { return _resource; }

    X_API XSDK::XString GetResourcePath() const { return _resourcePath; }

    ///@brief Returns the full resource path.
    ///
    /// Returns the resource path concatenated with the resource in a standard
    /// manner. Anything using the full resource path rather than its pieces
    /// should use this function rather than construct it from the pieces. That
    /// way it's constructed consistently without extra slashes or anything
    /// like that.
    X_API XSDK::XString GetFullResourcePath() const;

    X_API void AddGetArg(const XSDK::XString& name, const XSDK::XString& value);

    X_API void ClearGetArgs();

    X_API void SetFullRawURI(const XSDK::XString& fullURI);

    X_API void SetResource(const XSDK::XString& resource);

    X_API void SetResourcePath(const XSDK::XString& resourcePath);

    /// @brief Sets the getArgs and updates the other parts of the URI accordingly.
    ///
    /// @note getArgs is non-const because of how Hash iterates. Its contents will
    ///       not change.
    X_API void SetGetArgs(const XSDK::XHash<XSDK::XString>& getArgs);

private:

//------------------------------------------------------------------------------
// Static Helper Functions
//------------------------------------------------------------------------------

    /// @brief Used to recreate the full raw URI after any of the other pieces
    ///        have been set separately.
    static XSDK::XString _ConstructFullRawURI(const XSDK::XString& resourcePath,
                                               const XSDK::XString& resource,
                                               const XSDK::XHash<XSDK::XString>& getArgs);

    /// @brief Takes a URI and returns its getarguments.
    static XSDK::XHash<XSDK::XString> _ParseGetArgs(const XSDK::XString& fullURI);

    /// @brief Takes a URI and returns the resource portion decoded.
    static XSDK::XString _ParseResource(const XSDK::XString& fullURI);

    /// @brief Takes a URI and returns the resource path portion decoded.
    static XSDK::XString _ParseResourcePath(const XSDK::XString& fullURI);

    /// @brief Makes sure that the given string begins with a forward slash.
    ///
    /// @return uri with a forward slash prepended onto it if it
    ///         didn't have one before.
    static XSDK::XString _VerifyStartingSlash(const XSDK::XString& uri);

    XSDK::XString _fullRawURI;
    XSDK::XHash<XSDK::XString> _getArgs;
    XSDK::XString _resource;
    XSDK::XString _resourcePath;
};

}

#endif
