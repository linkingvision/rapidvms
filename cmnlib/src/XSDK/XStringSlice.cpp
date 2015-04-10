
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XStringSlice.h"
#include <algorithm>

using namespace std;
using namespace XSDK;

static const XString emptyStr;

XStringSlice::XStringSlice() : _strPtr(&emptyStr), _first(0), _length(0) {}

bool XStringSlice::StartsWith(const XString& str) const
{
    const size_t strSize = str.size();
    return strSize <= _length && equal(begin(), begin() + strSize, str.begin());
}

bool XStringSlice::StartsWith(const std::string& stdstr) const
{
    const size_t strSize = stdstr.size();
    return strSize <= _length && equal(begin(), begin() + strSize, stdstr.begin());
}

bool XStringSlice::StartsWith(const char* cstr) const
{
    if(!cstr)
        X_THROW(("NULL string passed to XStringSlice::StartsWith()."));

    const size_t strSize = strlen(cstr);
    return strSize <= _length && equal(begin(), begin() + strSize, cstr);
}

bool XStringSlice::StartsWith(const wchar_t* wcstr) const
{
    if(!wcstr)
        X_THROW(("NULL string passed to XStringSlice::StartsWith()."));
    return StartsWith(XString::ConvertWideStringToMultiByteString(wcstr));
}


bool XStringSlice::EndsWith(const XString& str) const
{
    const size_t strSize = str.size();
    return strSize <= _length && equal(rbegin(), rbegin() + strSize, str.rbegin());
}

bool XStringSlice::EndsWith(const std::string& stdstr) const
{
    const size_t strSize = stdstr.size();
    return strSize <= _length && equal(rbegin(), rbegin() + strSize, stdstr.rbegin());
}

bool XStringSlice::EndsWith(const char* cstr) const
{
    if(!cstr)
        X_THROW(("NULL string passed to XStringSlice::EndsWith()."));
    return EndsWith(XString(cstr));
}

bool XStringSlice::EndsWith(const wchar_t* wcstr) const
{
    if(!wcstr)
        X_THROW(("NULL string passed to XStringSlice::EndsWith()."));
    return EndsWith(XString::ConvertWideStringToMultiByteString(wcstr));
}


//This is necessary rather than just using std::find, because std::find
//doesn't handle searching for a sequence of elements.
template<typename I>
size_t _FindIndex(XStringSlice::const_iterator iter1, XStringSlice::const_iterator end1, I iter2, I end2)
{
    X_ASSERT(end1 >= iter1);
    X_ASSERT(end2 >= iter2);

    if(iter2 == end2)
        return 0;

    XStringSlice::const_iterator orig = iter1;
    const size_t needleLen = end2 - iter2;

    //FIXME Make this more efficient - this is the brain-dead simple way to do it.
    while(true)
    {
        iter1 = find(iter1, end1, *iter2);
        if(size_t(end1 - iter1) < needleLen)
            return string::npos;

        if(equal(iter2, end2, iter1))
            return iter1 - orig;
        ++iter1;
    }

    return string::npos;
}


XStringSlice XStringSlice::Find(const XString& str) const
{
    return _Find(_FindIndex(begin(), end(), str.begin(), str.end()));
}

XStringSlice XStringSlice::Find(const std::string& stdstr) const
{
    return _Find(_FindIndex(begin(), end(), stdstr.begin(), stdstr.end()));
}

XStringSlice XStringSlice::Find(const char* cstr) const
{
    return _Find(_FindIndex(begin(), end(), cstr, cstr + strlen(cstr)));
}

XStringSlice XStringSlice::Find(const wchar_t* wcstr) const
{
    return Find(XString(wcstr));
}

XStringSlice XStringSlice::Find(char c) const
{
    return _Find(std::find(begin(), end(), c) - begin());
}

XStringSlice XStringSlice::_Find(size_t indexFound) const
{
    if(indexFound == string::npos)
        return Slice(0, 0);
    return XStringSlice(_strPtr, _first + indexFound, _length - indexFound);
}



XStringSlice XStringSlice::FindUntil(const XString& str, bool includeFound) const
{
    return _FindUntil(_FindIndex(begin(), end(), str.begin(), str.end()), str.size(), includeFound);
}

XStringSlice XStringSlice::FindUntil(const std::string& stdstr, bool includeFound) const
{
    return _FindUntil(_FindIndex(begin(), end(), stdstr.begin(), stdstr.end()), stdstr.size(), includeFound);
}

XStringSlice XStringSlice::FindUntil(const char* cstr, bool includeFound) const
{
    const size_t cstrLen = strlen(cstr);
    return _FindUntil(_FindIndex(begin(), end(), cstr, cstr + cstrLen), cstrLen, includeFound);
}

XStringSlice XStringSlice::FindUntil(char c, bool includeFound) const
{
    return _FindUntil(std::find(begin(), end(), c) - begin(), 1, includeFound);
}

XStringSlice XStringSlice::FindUntil(const wchar_t* wcstr, bool includeFound) const
{
    return FindUntil(XString(wcstr), includeFound);
}

XStringSlice XStringSlice::_FindUntil(size_t indexFound, size_t lenOfFound, bool includeFound) const
{
    if(indexFound == string::npos)
        return *this;
    const size_t targetIndex = includeFound ? indexFound + lenOfFound : indexFound;
    return XStringSlice(_strPtr, _first, targetIndex);
}

