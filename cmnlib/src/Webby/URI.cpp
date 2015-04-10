
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "Webby/URI.h"

#include <map>
#include <iostream>

#include "XSDK/XString.h"
#include "Webby/WebbyException.h"

using namespace std;
using namespace XSDK;
using namespace WEBBY;

//------------------------------------------------------------------------------
// Constructors/Destructor
//------------------------------------------------------------------------------

URI::URI(const XString& fullURI)
    : _fullRawURI(_VerifyStartingSlash(fullURI)),
      _getArgs(_ParseGetArgs(_fullRawURI)),
      _resource(_ParseResource(_fullRawURI)),
      _resourcePath(_ParseResourcePath(_fullRawURI))
{
    //We want to make sure that the URI is formed properly without any trailing
    //question marks or invalid arguments.
    _fullRawURI = _ConstructFullRawURI(_resourcePath, _resource, _getArgs);
}


//------------------------------------------------------------------------------
// Factory Functions
//------------------------------------------------------------------------------

URI URI::ConstructURI(const XString& resourcePath,
                      const XString& resource)
{
    URI retVal;

    retVal.SetResourcePath(resourcePath);
    retVal.SetResource(resource);

    return retVal;
}


URI URI::ConstructURI(const XString& resourcePath,
                      const XString& resource,
                      const XHash<XString>& getArgs)
{
    URI retVal;

    retVal.SetResourcePath(resourcePath);
    retVal.SetResource(resource);
    retVal.SetGetArgs(getArgs);

    return retVal;
}


//------------------------------------------------------------------------------
// Overloaded Operators
//------------------------------------------------------------------------------

bool URI::operator==(const URI& other) const
{
    return _fullRawURI == other._fullRawURI;
}


bool URI::operator!=(const URI& other) const
{
    return _fullRawURI != other._fullRawURI;
}


bool URI::operator<(const URI& other) const
{
    return _fullRawURI < other._fullRawURI;
}


//------------------------------------------------------------------------------
// Getters
//------------------------------------------------------------------------------

XString URI::GetFullResourcePath() const
{
    return _resourcePath + _resource;
}


//------------------------------------------------------------------------------
// Setters
//------------------------------------------------------------------------------

void URI::AddGetArg(const XString& name, const XString& value)
{
    if(name.empty())
        X_STHROW(WebbyException, ("Empty get argument name."));
    if(value.empty())
        X_STHROW(WebbyException, ("Empty get argument value."));

    _getArgs.Add(name, value);
    _fullRawURI = _ConstructFullRawURI(_resourcePath, _resource, _getArgs);
}


void URI::ClearGetArgs()
{
    XHash<XString> getArgs = _getArgs;

    //Doing it roundabout like this is a bit convoluted, but it guarantees
    //that we won't end up in an inconsistent state if either Clear or
    //_ConstructFullRawURI throws.
    getArgs.Clear();
    _fullRawURI = _ConstructFullRawURI(_resourcePath, _resource, getArgs);
    _getArgs = getArgs;
}


void URI::SetFullRawURI(const XString& fullURI)
{
    const XString slashedURI = _VerifyStartingSlash(fullURI);

    XHash<XString> getArgs = _ParseGetArgs(slashedURI);
    const XString resource = _ParseResource(slashedURI);
    const XString resourcePath = _ParseResourcePath(slashedURI);

    //Nothing is set until all functions are called in order
    //to avoid leaving the object in an inconsistent state
    //if an exception is thrown.
    _fullRawURI = _ConstructFullRawURI(resourcePath, resource, getArgs);
    _getArgs = getArgs;
    _resource = resource;
    _resourcePath = resourcePath;
}


void URI::SetResource(const XString& resource)
{
    if(resource != resource.URIEncode())
        X_STHROW(WebbyException, ("Resource can only have alphanumeric characters."));

    //Assignment order ensures that the object stays in a valid state.
    _fullRawURI = _ConstructFullRawURI(_resourcePath, resource, _getArgs);
    _resource = resource;
}


void URI::SetResourcePath(const XString& resourcePath)
{
    {
        const XString unslashedResourcePath = resourcePath.EraseAll('/');

        if(unslashedResourcePath != unslashedResourcePath.URIEncode())
            X_STHROW(WebbyException, ("Resource Path can only have slashes and alphanumeric characters."));
    }

    XString slashedResourcePath = _VerifyStartingSlash(resourcePath);

    //Verify ending slash.
    if(slashedResourcePath[slashedResourcePath.size()-1] != '/')
        slashedResourcePath.push_back('/');

    //Assignment order ensures that the object stays in a valid state.
    _fullRawURI = _ConstructFullRawURI(slashedResourcePath, _resource, _getArgs);
    _resourcePath = slashedResourcePath;
}


void URI::SetGetArgs(const XHash<XString>& getArgs)
{
    _fullRawURI = _ConstructFullRawURI(_resourcePath, _resource, getArgs);
    _getArgs = getArgs;
}


//------------------------------------------------------------------------------
// Static Helper Functions
//------------------------------------------------------------------------------

XString URI::_ConstructFullRawURI(const XString& resourcePath,
                                 const XString& resource,
                                 const XHash<XString>& getArgs)
{
    X_ASSERT(resourcePath.empty() || resourcePath[0] == '/');

    XString fullRawURI = resourcePath;

    fullRawURI.append(resource);

    if(!getArgs.Empty())
    {
        fullRawURI.push_back('?');

        XHash<XString> sortedGetArgs(Compare);

        for(XHash<XString>::XHashIter iter = getArgs.GetIterator();
            iter.IterValid();
            iter.Iterate())
        {
            const XString name = iter.IterKey();
            const XString* value = iter.IterData();

            if(name.empty())
                X_STHROW(WebbyException, ("Empty get argument name."));
            if(value->empty())
                X_STHROW(WebbyException, ("Empty get argument value."));

            sortedGetArgs.Add(name, *value);
        }

        unsigned long i = 0;
        const unsigned long size = sortedGetArgs.GetElemCount();
        for(XHash<XString>::XHashIter iter = sortedGetArgs.GetIterator();
            iter.IterValid();
            iter.Iterate())
        {
            fullRawURI.append(iter.IterKey().URIEncode());
            fullRawURI.push_back('=');
            fullRawURI.append(iter.IterData()->URIEncode());

            if(++i < size)
                fullRawURI.push_back('&');
        }
    }

    return fullRawURI;
}


XHash<XString> URI::_ParseGetArgs(const XString& fullURI)
{
    XHash<XString> getArgs;

    const size_t questionIndex = fullURI.find('?');

    if(questionIndex != string::npos)
    {
        size_t argLen = fullURI.size() - (questionIndex + 1);
        const XString wholeArgs = fullURI.substr(questionIndex+1, argLen).ReplaceAllInPlace('?', '&');

        const vector<XString> nvPairs = wholeArgs.Split("&");

        for(vector<XString>::const_iterator iter = nvPairs.begin(),
                                           end  = nvPairs.end();
            iter != end;
            ++iter)
        {
            const vector<XString> nvParts = iter->Split("=");

            if(nvParts.size() == 2)
            {
                const XString& name = nvParts[0];
                const XString& value = nvParts[1];

                getArgs.Add(name.URIDecode(), value.URIDecode());
            }

        }
    }

    return getArgs;
}


XString URI::_ParseResource(const XString& fullRawURI)
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

    const XString resource = fullRawURI.substr(lastSlashIndex, len);

    return resource;
}


XString URI::_ParseResourcePath(const XString& fullRawURI)
{
    // Get Question Mark Index
    size_t questionIndex = fullRawURI.find("?");

    if(questionIndex == string::npos)
        questionIndex = fullRawURI.size();

    XString tempURI = fullRawURI;
    tempURI = fullRawURI.substr(0,questionIndex);

    // If we have a slash as the last character, chop it off
    if(tempURI.size() > 1 && tempURI.size() == tempURI.rfind("/") + 1)
        tempURI = fullRawURI.substr(0,tempURI.rfind("/"));

    const XString resourcePath = tempURI.substr(0, tempURI.rfind("/") + 1);

    return resourcePath;
}


XString URI::_VerifyStartingSlash(const XString& uri)
{
    if(uri.empty())
        return "/";
    else if(uri[0] != '/')
        return "/" + uri;

    return uri;
}

