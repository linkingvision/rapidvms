
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

#include "hyperhttp/hyperuri.h"

#include <map>
#include <iostream>

#include "cppkit/ck_string.h"
#include "hyperhttp/hyperhttp_exception.h"

using namespace std;
using namespace cppkit;
using namespace hyperhttp;

uri::uri( const ck_string& fullURI ) :
    _fullRawURI( _verify_starting_slash( fullURI ) ),
    _getArgs( _parse_get_args( _fullRawURI ) ),
    _resource( _parse_resource( _fullRawURI ) ),
    _resourcePath( _parse_resource_path( _fullRawURI ) )
{
    //We want to make sure that the uri is formed properly without any trailing
    //question marks or invalid arguments.
    _fullRawURI = _construct_full_raw_uri( _resourcePath, _resource, _getArgs );
}

uri uri::construct_uri( const ck_string& resourcePath,
                        const ck_string& resource )
{
    uri retVal;

    retVal.set_resource_path( resourcePath );
    retVal.set_resource( resource );

    return retVal;
}


uri uri::construct_uri( const ck_string& resourcePath,
                        const ck_string& resource,
                        const map<string, ck_string>& getArgs )
{
    uri retVal;

    retVal.set_resource_path( resourcePath );
    retVal.set_resource( resource );
    retVal.set_get_args( getArgs );

    return retVal;
}

bool uri::operator == ( const uri& other ) const
{
    return _fullRawURI == other._fullRawURI;
}

bool uri::operator != ( const uri& other ) const
{
    return _fullRawURI != other._fullRawURI;
}

bool uri::operator < ( const uri& other ) const
{
    return _fullRawURI < other._fullRawURI;
}

ck_string uri::get_full_resource_path() const
{
    return _resourcePath + _resource;
}

void uri::add_get_arg( const ck_string& name, const ck_string& value )
{
    if(name.empty())
        CK_STHROW(hyperhttp_exception, ("Empty get argument name."));
    if(value.empty())
        CK_STHROW(hyperhttp_exception, ("Empty get argument value."));

    _getArgs[name] = value;
    _fullRawURI = _construct_full_raw_uri(_resourcePath, _resource, _getArgs);
}


void uri::clear_get_args()
{
    map<string, ck_string> getArgs = _getArgs;

    //Doing it roundabout like this is a bit convoluted, but it guarantees
    //that we won't end up in an inconsistent state if either Clear or
    //_ConstructFullRawuri throws.
    getArgs.clear();
    _fullRawURI = _construct_full_raw_uri( _resourcePath, _resource, getArgs );
    _getArgs = getArgs;
}


void uri::set_full_raw_uri(const ck_string& fullURI)
{
    const ck_string slashedURI = _verify_starting_slash(fullURI);

    map<string, ck_string> getArgs = _parse_get_args(slashedURI);
    const ck_string resource = _parse_resource(slashedURI);
    const ck_string resourcePath = _parse_resource_path(slashedURI);

    //Nothing is set until all functions are called in order
    //to avoid leaving the object in an inconsistent state
    //if an exception is thrown.
    _fullRawURI = _construct_full_raw_uri(resourcePath, resource, getArgs);
    _getArgs = getArgs;
    _resource = resource;
    _resourcePath = resourcePath;
}


void uri::set_resource(const ck_string& resource)
{
    if(resource != resource.uri_encode())
        CK_STHROW(hyperhttp_exception, ("Resource can only have alphanumeric characters."));

    //Assignment order ensures that the object stays in a valid state.
    _fullRawURI = _construct_full_raw_uri(_resourcePath, resource, _getArgs);
    _resource = resource;
}


void uri::set_resource_path(const ck_string& resourcePath)
{
    {
        const ck_string unslashedResourcePath = resourcePath.erase_all('/');

        if(unslashedResourcePath != unslashedResourcePath.uri_encode())
            CK_STHROW(hyperhttp_exception, ("Resource Path can only have slashes and alphanumeric characters."));
    }

    ck_string slashedResourcePath = _verify_starting_slash(resourcePath);

    //Verify ending slash.
    if(slashedResourcePath[slashedResourcePath.size()-1] != '/')
        slashedResourcePath.push_back('/');

    //Assignment order ensures that the object stays in a valid state.
    _fullRawURI = _construct_full_raw_uri(slashedResourcePath, _resource, _getArgs);
    _resourcePath = slashedResourcePath;
}


void uri::set_get_args(const map<string, ck_string>& getArgs)
{
    _fullRawURI = _construct_full_raw_uri(_resourcePath, _resource, getArgs);
    _getArgs = getArgs;
}


//------------------------------------------------------------------------------
// Static Helper Functions
//------------------------------------------------------------------------------

ck_string uri::_construct_full_raw_uri(const ck_string& resourcePath,
                                       const ck_string& resource,
                                       const map<string, ck_string>& getArgs)
{
    ck_string fullRawURI = resourcePath;

    fullRawURI.append(resource);

    if(!getArgs.empty())
    {
        fullRawURI.push_back('?');

        map<string, ck_string> sortedGetArgs;

        for( auto iter = getArgs.begin();
             iter != getArgs.end();
             iter++ )
        {
            const ck_string name = (*iter).first;
            const ck_string value = (*iter).second;

            if(name.empty())
                CK_STHROW(hyperhttp_exception, ("Empty get argument name."));
            if(value.empty())
                CK_STHROW(hyperhttp_exception, ("Empty get argument value."));

            sortedGetArgs[name] = value;
        }

        unsigned long i = 0;
        const unsigned long size = sortedGetArgs.size();
        for(auto iter = sortedGetArgs.begin();
            iter != sortedGetArgs.end();
            iter++)
        {
            fullRawURI.append( ck_string((*iter).first).uri_encode() );
            fullRawURI.push_back('=');
            fullRawURI.append( (*iter).second.uri_encode() );

            if(++i < size)
                fullRawURI.push_back('&');
        }
    }

    return fullRawURI;
}


map<string, ck_string> uri::_parse_get_args(const ck_string& fullURI)
{
    map<string, ck_string> getArgs;

    const size_t questionIndex = fullURI.find('?');

    if(questionIndex != string::npos)
    {
        size_t argLen = fullURI.size() - (questionIndex + 1);
        const ck_string wholeArgs = fullURI.substr(questionIndex+1, argLen).replace_all('?', '&');

        const vector<ck_string> nvPairs = wholeArgs.split("&");

        for( auto iter = nvPairs.begin(), end  = nvPairs.end();
             iter != end;
             ++iter )
        {
            const vector<ck_string> nvParts = iter->split("=");

            if(nvParts.size() == 2)
            {
                const ck_string& name = nvParts[0];
                const ck_string& value = nvParts[1];

                getArgs[name.uri_decode()] = value.uri_decode();
            }
        }
    }

    return getArgs;
}


ck_string uri::_parse_resource(const ck_string& fullRawURI)
{
    size_t questionIndex = fullRawURI.find("?");

    if(questionIndex == string::npos)
        questionIndex = fullRawURI.size();

    // The goal is to determine the index of the start of the resource name. When we FindLast()
    // from the right their are 3 possible cases:
    //   1 - We find the rightmost / in a path consisting of many sub dirs.
    //   2 - We find the rightmost / in a path that consists of only a single /.
    //   3 - We find NO /, so FindLast() returns -1.
    // In all 3 of these cases, incrementing the lastSlashIndex is the right thing to do!

    size_t lastSlashIndex = fullRawURI.substr(0, questionIndex).rfind("/") + 1;
    uint32_t len = 0;

    if((fullRawURI.size() > 1) && ((fullRawURI.size() == lastSlashIndex) || (lastSlashIndex == questionIndex)))
    {
        lastSlashIndex = fullRawURI.substr(0, lastSlashIndex - 1).rfind("/") + 1;
        len = questionIndex - lastSlashIndex - 1;
    }
    else
    {
        len = questionIndex - lastSlashIndex;
    }

    const ck_string resource = fullRawURI.substr(lastSlashIndex, len);

    return resource;
}


ck_string uri::_parse_resource_path(const ck_string& fullRawURI)
{
    // Get Question Mark Index
    size_t questionIndex = fullRawURI.find("?");

    if(questionIndex == string::npos)
        questionIndex = fullRawURI.size();

    ck_string tempURI = fullRawURI;
    tempURI = fullRawURI.substr(0,questionIndex);

    // If we have a slash as the last character, chop it off
    if(tempURI.size() > 1 && tempURI.size() == tempURI.rfind("/") + 1)
        tempURI = fullRawURI.substr(0,tempURI.rfind("/"));

    const ck_string resourcePath = tempURI.substr(0, tempURI.rfind("/") + 1);

    return resourcePath;
}


ck_string uri::_verify_starting_slash(const ck_string& uri)
{
    if(uri.empty())
        return "/";
    else if(uri[0] != '/')
        return "/" + uri;

    return uri;
}

