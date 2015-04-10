
#include "cppkit/ck_string_slice.h"
#include <algorithm>

using namespace std;
using namespace cppkit;

static const ck_string emptyStr;

ck_string_slice::ck_string_slice() : _strPtr(&emptyStr), _first(0), _length(0) {}

bool ck_string_slice::starts_with(const ck_string& str) const
{
    const size_t strSize = str.size();
    return strSize <= _length && equal(begin(), begin() + strSize, str.begin());
}

bool ck_string_slice::starts_with(const std::string& stdstr) const
{
    const size_t strSize = stdstr.size();
    return strSize <= _length && equal(begin(), begin() + strSize, stdstr.begin());
}

bool ck_string_slice::starts_with(const char* cstr) const
{
    if(!cstr)
        CK_THROW(("NULL string passed to ck_string_slice::starts_with()."));

    const size_t strSize = strlen(cstr);
    return strSize <= _length && equal(begin(), begin() + strSize, cstr);
}

bool ck_string_slice::starts_with(const wchar_t* wcstr) const
{
    if(!wcstr)
        CK_THROW(("NULL string passed to ck_string_slice::starts_with()."));
    return starts_with(ck_string::convert_wide_string_to_multi_byte_string(wcstr));
}

bool ck_string_slice::ends_with(const ck_string& str) const
{
    const size_t strSize = str.size();
    return strSize <= _length && equal(rbegin(), rbegin() + strSize, str.rbegin());
}

bool ck_string_slice::ends_with(const std::string& stdstr) const
{
    const size_t strSize = stdstr.size();
    return strSize <= _length && equal(rbegin(), rbegin() + strSize, stdstr.rbegin());
}

bool ck_string_slice::ends_with(const char* cstr) const
{
    if(!cstr)
        CK_THROW(("NULL string passed to ck_string_slice::ends_with()."));
    return ends_with(ck_string(cstr));
}

bool ck_string_slice::ends_with(const wchar_t* wcstr) const
{
    if(!wcstr)
        CK_THROW(("NULL string passed to ck_string_slice::ends_with()."));
    return ends_with(ck_string::convert_wide_string_to_multi_byte_string(wcstr));
}

//This is necessary rather than just using std::find, because std::find
//doesn't handle searching for a sequence of elements.
template<typename I>
size_t _find_index(ck_string_slice::const_iterator iter1, ck_string_slice::const_iterator end1, I iter2, I end2)
{
    if( end1 < iter1 )
        CK_THROW(("Invalid iterator position."));
    if( end2 < iter2 )
        CK_THROW(("Invalid iterator position."));

    if(iter2 == end2)
        return 0;

    ck_string_slice::const_iterator orig = iter1;
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

ck_string_slice ck_string_slice::find(const ck_string& str) const
{
    return _find(_find_index(begin(), end(), str.begin(), str.end()));
}

ck_string_slice ck_string_slice::find(const std::string& stdstr) const
{
    return _find(_find_index(begin(), end(), stdstr.begin(), stdstr.end()));
}

ck_string_slice ck_string_slice::find(const char* cstr) const
{
    return _find(_find_index(begin(), end(), cstr, cstr + strlen(cstr)));
}

ck_string_slice ck_string_slice::find(const wchar_t* wcstr) const
{
    return find(ck_string(wcstr));
}

ck_string_slice ck_string_slice::find(char c) const
{
    return _find(std::find(begin(), end(), c) - begin());
}

ck_string_slice ck_string_slice::_find(size_t indexFound) const
{
    if(indexFound == string::npos)
        return slice(0, 0);
    return ck_string_slice(_strPtr, _first + indexFound, _length - indexFound);
}

ck_string_slice ck_string_slice::find_until(const ck_string& str, bool includeFound) const
{
    return _find_until(_find_index(begin(), end(), str.begin(), str.end()), str.size(), includeFound);
}

ck_string_slice ck_string_slice::find_until(const std::string& stdstr, bool includeFound) const
{
    return _find_until(_find_index(begin(), end(), stdstr.begin(), stdstr.end()), stdstr.size(), includeFound);
}

ck_string_slice ck_string_slice::find_until(const char* cstr, bool includeFound) const
{
    const size_t cstrLen = strlen(cstr);
    return _find_until(_find_index(begin(), end(), cstr, cstr + cstrLen), cstrLen, includeFound);
}

ck_string_slice ck_string_slice::find_until(char c, bool includeFound) const
{
    return _find_until(std::find(begin(), end(), c) - begin(), 1, includeFound);
}

ck_string_slice ck_string_slice::find_until(const wchar_t* wcstr, bool includeFound) const
{
    return find_until(ck_string(wcstr), includeFound);
}

ck_string_slice ck_string_slice::_find_until(size_t indexFound, size_t lenOfFound, bool includeFound) const
{
    if(indexFound == string::npos)
        return *this;
    const size_t targetIndex = includeFound ? indexFound + lenOfFound : indexFound;
    return ck_string_slice(_strPtr, _first, targetIndex);
}
