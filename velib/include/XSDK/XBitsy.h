
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XBitsy_h
#define XBitsy_h

#include "XSDK/XSharedLib.h"
#include "XSDK/Errors.h"

class XBitsyTest;

namespace XSDK
{

/// XBitsy provides a simple way to read bits from a memory buffer.
/// One way to think about XBitsy is as a bit aware iterator that allows a reader
/// to read any number of bits (up to 32 at a time) from a buffer. XBitsy
/// internally keeps track of any pointers (and bitmasks) necessary. A user of
/// XBitsy need only write code like this:
///
/// XBitsy bitsy(foo, 128);
///
/// val = bitsy.GetBits(12);
/// val2 = bitsy.GetBits(16);
///
/// Where val and val2 were previously declared to be of type uint32_t.
template<typename Iter>
class XBitsy
{
    friend class ::XBitsyTest;
public:
    ///
    X_API XBitsy(Iter begin, Iter end);

    ///
    X_API XBitsy(const XBitsy& obj);

    ///
    X_API virtual ~XBitsy() {}

    ///
    X_API XBitsy& operator=(const XBitsy& obj);

    ///
    X_API void SetBuffer(Iter begin, Iter end);

    ///
    X_API void Reset();

    /// Returns the next bitCount bits as uint32_t.
    ///
    /// If there aren't enough bits remaining, then all of the remaining
    /// bits are returned. If there are no bits remaining, then 0 is
    /// returned.
    X_API uint32_t GetBits(int bitCount);

    /// Same as GetBits but throws if there aren't enough bits left.
    X_API uint32_t GetExactBits(int bitCount);

    /// Returns the number of bits remaining to be read.
    X_API size_t GetBitsRemaining() const;

    /// Returns the minimum number of bits needed to represent the value of dword
    X_API int NeededBits(uint32_t dword);

private:

    inline void _IncPos();
    inline size_t _GetBytesRemaining() const;

    Iter _begin;
    Iter _end;
    Iter _pos;
    uint8_t _currentMask;
    mutable size_t _bytesRemaining;
};

template<typename Iter>
XBitsy<Iter>::XBitsy(Iter begin, Iter end)
    : _begin(begin),
      _end(end),
      _pos(begin),
      _currentMask(128),
      _bytesRemaining(-1)
{
}

template<typename Iter>
XBitsy<Iter>::XBitsy(const XBitsy& obj)
    : _begin(obj._begin),
      _end(obj._end),
      _pos(obj._pos),
      _currentMask(obj._currentMask),
      _bytesRemaining(obj._bytesRemaining)
{
}

template<typename Iter>
XBitsy<Iter>& XBitsy<Iter>::operator =(const XBitsy& obj)
{
    _begin = obj._begin;
    _end = obj._end;
    _pos = obj._pos;
    _currentMask = obj._currentMask;
    _bytesRemaining = obj._bytesRemaining;
    return *this;
}

template<typename Iter>
void XBitsy<Iter>::SetBuffer(Iter begin, Iter end)
{
    _begin = begin;
    _end = end;
    Reset();
}

template<typename Iter>
void XBitsy<Iter>::Reset()
{
    _pos = _begin;
    _currentMask = 128;
    _bytesRemaining = -1;
}

template<typename Iter>
uint32_t XBitsy<Iter>::GetBits(int bitCount)
{
    X_ASSERT(bitCount >= 0 && bitCount <= 32);
    uint32_t outputBits = 0;

    for(int i = 0; i < bitCount && _pos < _end; ++i)
    {
        outputBits = outputBits << 1;

        if((*_pos) & _currentMask)
            outputBits |= 1;

        if(_currentMask == 1)
        {
            _currentMask = 128;
            _IncPos();
        }
        else
            _currentMask = _currentMask >> 1;
    }

    return outputBits;
}

template<typename Iter>
uint32_t XBitsy<Iter>::GetExactBits(int bitCount)
{
    X_ASSERT(bitCount > 0);
    const size_t remaining = GetBitsRemaining();

    if(remaining < (size_t)bitCount)
        X_THROW(("Not enough bits remaining."));

    return GetBits(bitCount);
}

template<typename Iter>
size_t XBitsy<Iter>::GetBitsRemaining() const
{
    unsigned int bits = 0;

    switch(_currentMask)
    {
        case 1:
            bits = 1;
            break;
        case 2:
            bits = 2;
            break;
        case 4:
            bits = 3;
            break;
        case 8:
            bits = 4;
            break;
        case 16:
            bits = 5;
            break;
        case 32:
            bits = 6;
            break;
        case 64:
            bits = 7;
            break;
        case 128:
            bits = 8;
            break;
        default:
            X_ASSERT(false);
    }

    return _GetBytesRemaining() * 8 - (8 - bits);
}

// Returns the minimum number of bits needed to represent the value of dword
template<typename Iter>
int XBitsy<Iter>::NeededBits(uint32_t dword)
{
    uint32_t mask = 1;
    uint32_t val = 0;
    int bitsRequired = 0;

    while(val < dword)
    {
        val = val << 1;
        val |= mask;
        ++bitsRequired;
    }

    return bitsRequired;
}

template<typename Iter>
inline void XBitsy<Iter>::_IncPos()
{
    ++_pos;
    if(_bytesRemaining == -1)
        _bytesRemaining = _GetBytesRemaining();
    else
        --_bytesRemaining;
}

template<>
inline void XBitsy<const uint8_t*>::_IncPos()
{
    ++_pos;
}

template<typename Iter>
inline size_t XBitsy<Iter>::_GetBytesRemaining() const
{
    if(_bytesRemaining == -1)
    {
        _bytesRemaining = 0;
        Iter temp = _pos;
        while(temp != _end) ++temp, ++_bytesRemaining;
    }

    return _bytesRemaining;
}

template<>
inline size_t XBitsy<const uint8_t*>::_GetBytesRemaining() const
{
    return _end - _pos;
}

};

#endif
