
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _MEDIA_PARSER_RBSP_Iter_h_
#define _MEDIA_PARSER_RBSP_Iter_h_

#include "XSDK/XException.h"
#include <cstring>

class RBSPIterTest;

namespace MEDIA_PARSER
{

/// \brief An iterator which wraps a const uint8_t* and skips the emulation
///        prevention bytes as it iterates over the RBSP section of an H.264
///        frame.
class RBSPIter
{
    friend class ::RBSPIterTest;
public:

    // end is only used to check whether we try and iterate past the end of
    // of the buffer. It has no effect on comparisons or anything else.
    X_API RBSPIter(const uint8_t* begin, const uint8_t* end)
        : _iter(begin),
          _end(end),
          _numZeroes(0)
    {}

    // For when we want to have an RBSPIterator which is an end iterator.
    X_API RBSPIter(const uint8_t* end)
        : _iter(end),
          _end(end),
          _numZeroes(0)
    {}

    X_API RBSPIter() : _iter(0), _end(0), _numZeroes(0) {}

    X_API uint8_t operator*() const
    {
        if(_iter == _end)
            X_THROW(("Attempted to dereference RBSPIter which is at the end of its buffer."));
        return *_iter;
    }

    // prefix
    X_API RBSPIter& operator++();

    // postfix
    X_API RBSPIter operator++(int) { RBSPIter retval = *this; ++_iter; return retval; }

    X_API bool operator==(const RBSPIter& other) const { return _iter == other._iter; }
    X_API bool operator!=(const RBSPIter& other) const { return _iter != other._iter; }
    X_API bool operator< (const RBSPIter& other) const { return _iter <  other._iter; }
    X_API bool operator<=(const RBSPIter& other) const { return _iter <= other._iter; }
    X_API bool operator>=(const RBSPIter& other) const { return _iter >= other._iter; }
    X_API bool operator> (const RBSPIter& other) const { return _iter >  other._iter; }

private:

    const uint8_t* _iter;
    const uint8_t* _end;
    int _numZeroes;
};


X_API RBSPIter GetRBSPFromNAL(const uint8_t* nalUnit, size_t length, RBSPIter* rbspEnd);

}

#endif
