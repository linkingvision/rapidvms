
#ifndef ck_bitsy_h
#define ck_bitsy_h

#include "cppkit/os/ck_exports.h"
#include "cppkit/ck_exception.h"
#include "cppkit/ck_byte_ptr.h"

class ck_bitsy_test;

namespace cppkit
{

/// ck_bitsy provides a simple way to read bits from a memory buffer.
/// One way to think about ck_bitsy is as a bit aware iterator that allows a reader
/// to read any number of bits (up to 32 at a time) from a buffer. ck_bitsy
/// internally keeps track of any pointers (and bitmasks) necessary. A user of
/// ck_bitsy need only write code like this:
///
/// ck_bitsy bitsy(p1, p2);
///
/// val = bitsy.get_bits(12);
/// val2 = bitsy.get_bits(16);
///
/// Where val and val2 were previously declared to be of type uint32_t.

class ck_bitsy
{
    friend class ::ck_bitsy_test;
public:

    CK_API ck_bitsy(const uint8_t* begin, size_t size);
    CK_API ck_bitsy(const ck_byte_ptr& src );
    CK_API ck_bitsy(const ck_bitsy& obj);

    CK_API virtual ~ck_bitsy() {}

    CK_API ck_bitsy& operator=(const ck_bitsy& obj);

    CK_API void set_buffer(const uint8_t* begin, size_t size);

    CK_API void reset();

    /// Returns the next bitCount bits as uint32_t.
    ///
    /// If there aren't enough bits remaining, then all of the remaining
    /// bits are returned. If there are no bits remaining, then 0 is
    /// returned.
    CK_API uint32_t get_bits(int bitCount);

    /// Same as get_bits but throws if there aren't enough bits left.
    CK_API uint32_t get_exact_bits(int bitCount);

    /// Returns the number of bits remaining to be read.
    CK_API size_t get_bits_remaining() const;

    /// Skip the specified number of bits
    CK_API void skip_bits(int bitCount);

    /// Returns the minimum number of bits needed to represent the value of dword
    CK_API static int needed_bits(uint32_t dword);

private:

    inline void _inc_pos();
    inline size_t _get_bytes_remaining() const;

    const uint8_t* _begin;
    size_t _length;
    const uint8_t* _pos;
    uint8_t _currentMask;
    mutable size_t _bytesRemaining;
};

ck_bitsy::ck_bitsy(const uint8_t* begin, size_t length) :
    _begin(begin),
    _length(length),
    _pos(begin),
    _currentMask(128),
    _bytesRemaining(-1)
{
}

ck_bitsy::ck_bitsy(const ck_byte_ptr& src ) :
    _begin( src.get_ptr() ),
    _length( src.length() ),
    _pos( _begin ),
    _currentMask(128),
    _bytesRemaining(-1)
{
}

ck_bitsy::ck_bitsy(const ck_bitsy& obj) :
    _begin(obj._begin),
    _length(obj._length),
    _pos(obj._pos),
    _currentMask(obj._currentMask),
    _bytesRemaining(obj._bytesRemaining)
{
}

ck_bitsy& ck_bitsy::operator=(const ck_bitsy& obj)
{
    _begin = obj._begin;
    _length = obj._length;
    _pos = obj._pos;
    _currentMask = obj._currentMask;
    _bytesRemaining = obj._bytesRemaining;
    return *this;
}

void ck_bitsy::set_buffer(const uint8_t* begin, size_t length)
{
    _begin = begin;
    _length = length;
    reset();
}

void ck_bitsy::reset()
{
    _pos = _begin;
    _currentMask = 128;
    _bytesRemaining = -1;
}

uint32_t ck_bitsy::get_bits(int bitCount)
{
    uint32_t outputBits = 0;

    for(int i = 0; i < bitCount && _pos < (_begin + _length); ++i)
    {
        outputBits = outputBits << 1;

        if((*_pos) & _currentMask)
            outputBits |= 1;

        if(_currentMask == 1)
        {
            _currentMask = 128;
            _inc_pos();
        }
        else _currentMask = _currentMask >> 1;
    }

    return outputBits;
}

uint32_t ck_bitsy::get_exact_bits(int bitCount)
{
    const size_t remaining = get_bits_remaining();

    if(remaining < (size_t)bitCount)
        CK_THROW(("Not enough bits remaining."));

    return get_bits(bitCount);
}

size_t ck_bitsy::get_bits_remaining() const
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
            break;
    }

    return _get_bytes_remaining() * 8 - (8 - bits);
}

void ck_bitsy::skip_bits(int bitCount)
{
    for(int i = 0; i < bitCount && _pos < (_begin + _length); ++i)
    {
        if(_currentMask == 1)
        {
            _currentMask = 128;
            _inc_pos();
        }
        else _currentMask = _currentMask >> 1;
    }
}

// Returns the minimum number of bits needed to represent the value of dword
int ck_bitsy::needed_bits(uint32_t dword)
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

inline void ck_bitsy::_inc_pos()
{
    ++_pos;
    if(_bytesRemaining == -1)
        _bytesRemaining = _get_bytes_remaining();
    else
        --_bytesRemaining;
}

inline size_t ck_bitsy::_get_bytes_remaining() const
{
    if(_bytesRemaining == -1)
    {
        _bytesRemaining = 0;
        const uint8_t* temp = _pos;
        while(temp != (_begin + _length)) ++temp, ++_bytesRemaining;
    }

    return _bytesRemaining;
}

};

#endif
