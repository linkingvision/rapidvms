
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XMD5.h"
#include <string.h>

using namespace XSDK;

#define F(x, y, z)			((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)			((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z)			(((x) ^ (y)) ^ (z))
#define H2(x, y, z)			((x) ^ ((y) ^ (z)))
#define I(x, y, z)			((y) ^ ((x) | ~(z)))

#define STEP(f, a, b, c, d, x, t, s) \
	(a) += f((b), (c), (d)) + (x) + (t); \
	(a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s)))); \
	(a) += (b);

#define SET(n) (*(uint32_t *)&ptr[(n) * 4])
#define GET(n) SET(n)

XMD5::XMD5() :
    _lo( 0 ),
    _hi( 0 ),
    _a( 0x67452301 ),
    _b( 0xefcdab89 ),
    _c( 0x98badcfe ),
    _d( 0x10325476 ),
    _buffer(),
    _block(),
    _finalized( false ),
    _result()
{
}

XMD5::~XMD5() throw()
{
}

void XMD5::Update( const uint8_t* src, size_t size )

{
    uint32_t saved_lo;
    unsigned long used, available;

    saved_lo = _lo;
    if ((_lo = (saved_lo + size) & 0x1fffffff) < saved_lo)
        _hi++;
    _hi += size >> 29;

    used = saved_lo & 0x3f;

    if (used) {
        available = 64 - used;

        if (size < available) {
            memcpy(&_buffer[used], src, size);
            return;
        }

        memcpy(&_buffer[used], src, available);
        src = (const unsigned char *)src + available;
        size -= available;
        _body(_buffer, 64);
    }

    if (size >= 64) {
        src = _body(src, size & ~(unsigned long)0x3f);
        size &= 0x3f;
    }

    memcpy(_buffer, src, size);
}

void XMD5::Finalize()
{
    _finalized = true;

    unsigned long used, available;

    used = _lo & 0x3f;

    _buffer[used++] = 0x80;

    available = 64 - used;

    if (available < 8) {
        memset(&_buffer[used], 0, available);
        _body(_buffer, 64);
        used = 0;
        available = 64;
    }

    memset(&_buffer[used], 0, available - 8);

    _lo <<= 3;
    _buffer[56] = _lo;
    _buffer[57] = _lo >> 8;
    _buffer[58] = _lo >> 16;
    _buffer[59] = _lo >> 24;
    _buffer[60] = _hi;
    _buffer[61] = _hi >> 8;
    _buffer[62] = _hi >> 16;
    _buffer[63] = _hi >> 24;

    _body(_buffer, 64);

    _result[0] = _a;
    _result[1] = _a >> 8;
    _result[2] = _a >> 16;
    _result[3] = _a >> 24;
    _result[4] = _b;
    _result[5] = _b >> 8;
    _result[6] = _b >> 16;
    _result[7] = _b >> 24;
    _result[8] = _c;
    _result[9] = _c >> 8;
    _result[10] = _c >> 16;
    _result[11] = _c >> 24;
    _result[12] = _d;
    _result[13] = _d >> 8;
    _result[14] = _d >> 16;
    _result[15] = _d >> 24;
}

void XMD5::Get( uint8_t* output )
{
    if( !_finalized )
        X_THROW(( "Please Finalize() your XMD5 before calling a Get() method." ));

    memcpy( output, _result, 16 );
}

XString XMD5::GetAsString()
{
    if( !_finalized )
        X_THROW(( "Please Finalize() your XMD5 before calling a Get() method." ));

    return XString::Format( "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
                            _result[0], _result[1],_result[2], _result[3],
                            _result[4], _result[5], _result[6], _result[7],
                            _result[8], _result[9], _result[10], _result[11],
                            _result[12], _result[13], _result[14], _result[15] );
}

const uint8_t* XMD5::_body( const uint8_t* data, size_t size )
{
    const uint8_t* ptr;
    uint32_t la, lb, lc, ld;
    uint32_t saved_a, saved_b, saved_c, saved_d;

    ptr = data;

    la = _a;
    lb = _b;
    lc = _c;
    ld = _d;

    do {
        saved_a = la;
        saved_b = lb;
        saved_c = lc;
        saved_d = ld;

/* Round 1 */
            STEP(F, la, lb, lc, ld, SET(0), 0xd76aa478, 7)
            STEP(F, ld, la, lb, lc, SET(1), 0xe8c7b756, 12)
            STEP(F, lc, ld, la, lb, SET(2), 0x242070db, 17)
            STEP(F, lb, lc, ld, la, SET(3), 0xc1bdceee, 22)
            STEP(F, la, lb, lc, ld, SET(4), 0xf57c0faf, 7)
            STEP(F, ld, la, lb, lc, SET(5), 0x4787c62a, 12)
            STEP(F, lc, ld, la, lb, SET(6), 0xa8304613, 17)
            STEP(F, lb, lc, ld, la, SET(7), 0xfd469501, 22)
            STEP(F, la, lb, lc, ld, SET(8), 0x698098d8, 7)
            STEP(F, ld, la, lb, lc, SET(9), 0x8b44f7af, 12)
            STEP(F, lc, ld, la, lb, SET(10), 0xffff5bb1, 17)
            STEP(F, lb, lc, ld, la, SET(11), 0x895cd7be, 22)
            STEP(F, la, lb, lc, ld, SET(12), 0x6b901122, 7)
            STEP(F, ld, la, lb, lc, SET(13), 0xfd987193, 12)
            STEP(F, lc, ld, la, lb, SET(14), 0xa679438e, 17)
            STEP(F, lb, lc, ld, la, SET(15), 0x49b40821, 22)

/* Round 2 */
            STEP(G, la, lb, lc, ld, GET(1), 0xf61e2562, 5)
            STEP(G, ld, la, lb, lc, GET(6), 0xc040b340, 9)
            STEP(G, lc, ld, la, lb, GET(11), 0x265e5a51, 14)
            STEP(G, lb, lc, ld, la, GET(0), 0xe9b6c7aa, 20)
            STEP(G, la, lb, lc, ld, GET(5), 0xd62f105d, 5)
            STEP(G, ld, la, lb, lc, GET(10), 0x02441453, 9)
            STEP(G, lc, ld, la, lb, GET(15), 0xd8a1e681, 14)
            STEP(G, lb, lc, ld, la, GET(4), 0xe7d3fbc8, 20)
            STEP(G, la, lb, lc, ld, GET(9), 0x21e1cde6, 5)
            STEP(G, ld, la, lb, lc, GET(14), 0xc33707d6, 9)
            STEP(G, lc, ld, la, lb, GET(3), 0xf4d50d87, 14)
            STEP(G, lb, lc, ld, la, GET(8), 0x455a14ed, 20)
            STEP(G, la, lb, lc, ld, GET(13), 0xa9e3e905, 5)
            STEP(G, ld, la, lb, lc, GET(2), 0xfcefa3f8, 9)
            STEP(G, lc, ld, la, lb, GET(7), 0x676f02d9, 14)
            STEP(G, lb, lc, ld, la, GET(12), 0x8d2a4c8a, 20)

/* Round 3 */
            STEP(H, la, lb, lc, ld, GET(5), 0xfffa3942, 4)
            STEP(H2, ld, la, lb, lc, GET(8), 0x8771f681, 11)
            STEP(H, lc, ld, la, lb, GET(11), 0x6d9d6122, 16)
            STEP(H2, lb, lc, ld, la, GET(14), 0xfde5380c, 23)
            STEP(H, la, lb, lc, ld, GET(1), 0xa4beea44, 4)
            STEP(H2, ld, la, lb, lc, GET(4), 0x4bdecfa9, 11)
            STEP(H, lc, ld, la, lb, GET(7), 0xf6bb4b60, 16)
            STEP(H2, lb, lc, ld, la, GET(10), 0xbebfbc70, 23)
            STEP(H, la, lb, lc, ld, GET(13), 0x289b7ec6, 4)
            STEP(H2, ld, la, lb, lc, GET(0), 0xeaa127fa, 11)
            STEP(H, lc, ld, la, lb, GET(3), 0xd4ef3085, 16)
            STEP(H2, lb, lc, ld, la, GET(6), 0x04881d05, 23)
            STEP(H, la, lb, lc, ld, GET(9), 0xd9d4d039, 4)
            STEP(H2, ld, la, lb, lc, GET(12), 0xe6db99e5, 11)
            STEP(H, lc, ld, la, lb, GET(15), 0x1fa27cf8, 16)
            STEP(H2, lb, lc, ld, la, GET(2), 0xc4ac5665, 23)

/* Round 4 */
            STEP(I, la, lb, lc, ld, GET(0), 0xf4292244, 6)
            STEP(I, ld, la, lb, lc, GET(7), 0x432aff97, 10)
            STEP(I, lc, ld, la, lb, GET(14), 0xab9423a7, 15)
            STEP(I, lb, lc, ld, la, GET(5), 0xfc93a039, 21)
            STEP(I, la, lb, lc, ld, GET(12), 0x655b59c3, 6)
            STEP(I, ld, la, lb, lc, GET(3), 0x8f0ccc92, 10)
            STEP(I, lc, ld, la, lb, GET(10), 0xffeff47d, 15)
            STEP(I, lb, lc, ld, la, GET(1), 0x85845dd1, 21)
            STEP(I, la, lb, lc, ld, GET(8), 0x6fa87e4f, 6)
            STEP(I, ld, la, lb, lc, GET(15), 0xfe2ce6e0, 10)
            STEP(I, lc, ld, la, lb, GET(6), 0xa3014314, 15)
            STEP(I, lb, lc, ld, la, GET(13), 0x4e0811a1, 21)
            STEP(I, la, lb, lc, ld, GET(4), 0xf7537e82, 6)
            STEP(I, ld, la, lb, lc, GET(11), 0xbd3af235, 10)
            STEP(I, lc, ld, la, lb, GET(2), 0x2ad7d2bb, 15)
            STEP(I, lb, lc, ld, la, GET(9), 0xeb86d391, 21)

            la += saved_a;
        lb += saved_b;
        lc += saved_c;
        ld += saved_d;

        ptr += 64;
    } while (size -= 64);

    _a = la;
    _b = lb;
    _c = lc;
    _d = ld;

    return ptr;
}
