
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// hyperhttp - http://www.cppkit.org
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

#ifndef hyperhttp_uri_h
#define hyperhttp_uri_h

#include <map>
#include "cppkit/ck_exception.h"
#include "cppkit/ck_string.h"

namespace hyperhttp
{

/// \brief Holds the portion of a URI to the right of the IP and port.
class uri
{
public:

    CK_API uri() :
        _fullRawURI("/"),
        _getArgs(),
        _resource(),
        _resourcePath("/")
    {
    }

    CK_API uri( const cppkit::ck_string& fullURI );

    // So string literals work when constructing with =
    CK_API uri( const char* cstr ) :
        _fullRawURI( "/" ),
        _getArgs(),
        _resource(),
        _resourcePath( "/" )
    {
        *this = uri( cppkit::ck_string( cstr ) );
    }

    CK_API uri( const uri& obj ) :
        _fullRawURI( obj._fullRawURI ),
        _getArgs( obj._getArgs ),
        _resource( obj._resource ),
        _resourcePath( obj._resourcePath )
    {
    }

    CK_API virtual ~uri() throw() {}

    CK_API static uri construct_uri( const cppkit::ck_string& resourcePath,
                                     const cppkit::ck_string& resource );

    CK_API static uri construct_uri( const cppkit::ck_string& resourcePath,
                                     const cppkit::ck_string& resource,
                                     const std::map<std::string,cppkit::ck_string>& getArgs );

    CK_API bool operator == ( const uri& other ) const;

    CK_API bool operator != ( const uri& other ) const;

    CK_API bool operator < ( const uri& other ) const;

    CK_API cppkit::ck_string get_full_raw_uri() const { return _fullRawURI; }

    CK_API const std::map<std::string, cppkit::ck_string>& get_get_args() const { return _getArgs; }

    CK_API cppkit::ck_string get_resource() const { return _resource; }

    CK_API cppkit::ck_string get_resource_path() const { return _resourcePath; }

    ///@brief Returns the full resource path.
    ///
    /// Returns the resource path concatenated with the resource in a standard
    /// manner. Anything using the full resource path rather than its pieces
    /// should use this function rather than construct it from the pieces. That
    /// way it's constructed consistently without extra slashes or anything
    /// like that.
    CK_API cppkit::ck_string get_full_resource_path() const;

    CK_API void add_get_arg( const cppkit::ck_string& name, const cppkit::ck_string& value );

    CK_API void clear_get_args();

    CK_API void set_full_raw_uri( const cppkit::ck_string& fullURI );

    CK_API void set_resource( const cppkit::ck_string& resource );

    CK_API void set_resource_path( const cppkit::ck_string& resourcePath );

    /// @brief Sets the getArgs and updates the other parts of the uri accordingly.
    ///
    /// @note getArgs is non-const because of how Hash iterates. Its contents will
    ///       not change.
    CK_API void set_get_args( const std::map<std::string, cppkit::ck_string>& getArgs );

private:
    /// @brief Used to recreate the full raw uri after any of the other pieces
    ///        have been set separately.
    static cppkit::ck_string _construct_full_raw_uri( const cppkit::ck_string& resourcePath,
                                                      const cppkit::ck_string& resource,
                                                      const std::map<std::string,
                                                      cppkit::ck_string>& getArgs );

    /// @brief Takes a uri and returns its getarguments.
    static std::map<std::string, cppkit::ck_string> _parse_get_args( const cppkit::ck_string& fullURI );

    /// @brief Takes a uri and returns the resource portion decoded.
    static cppkit::ck_string _parse_resource( const cppkit::ck_string& fullURI );

    /// @brief Takes a uri and returns the resource path portion decoded.
    static cppkit::ck_string _parse_resource_path( const cppkit::ck_string& fullURI );

    /// @brief Makes sure that the given string begins with a forward slash.
    ///
    /// @return uri with a forward slash prepended onto it if it
    ///         didn't have one before.
    static cppkit::ck_string _verify_starting_slash( const cppkit::ck_string& uri );

    cppkit::ck_string _fullRawURI;
    std::map<std::string, cppkit::ck_string> _getArgs;
    cppkit::ck_string _resource;
    cppkit::ck_string _resourcePath;
};

}

#endif
