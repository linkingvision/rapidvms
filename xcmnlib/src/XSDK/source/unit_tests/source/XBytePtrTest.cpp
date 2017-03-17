
#include "XBytePtrTest.h"

#include "XSDK/XBytePtr.h"
#include "XSDK/XMemory.h"

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XBytePtrTest);

static uint8_t g_buffer[]= {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
};
static const size_t g_len = sizeof(g_buffer)/sizeof(g_buffer[0]);


void XBytePtrTest::TestConstructor()
{
    {
        // Default ctor
        UT_ASSERT_NO_THROW(XBytePtr());
        XBytePtr p;
        UT_ASSERT( p.Length() == 0 );
        UT_ASSERT( p.GetPtr() == 0 );
        UT_ASSERT( p.OriginalPtr() == 0 );
    }
    {
        UT_ASSERT_NO_THROW( XBytePtr(g_buffer, g_len) );
        XBytePtr p(g_buffer, g_len);
        UT_ASSERT( p.Length() == g_len );
        UT_ASSERT( p.GetPtr() == g_buffer );
        UT_ASSERT( p.OriginalPtr() == g_buffer );
    }
    {
        // Copy ctor
        XBytePtr p(g_buffer, g_len);
        UT_ASSERT_NO_THROW( ++p );
        UT_ASSERT_NO_THROW( XBytePtr(p) );
        XBytePtr copy(p);
        UT_ASSERT( copy.Length() == g_len );
        UT_ASSERT( copy.GetPtr() == g_buffer + 1 );
        UT_ASSERT( copy.OriginalPtr() == g_buffer );
    }
}

void XBytePtrTest::TestGetSetPtr()
{
    {
        XBytePtr p;
        UT_ASSERT_NO_THROW( p.SetPtr(g_buffer, g_len) );
        UT_ASSERT( p.Length() == g_len );
        UT_ASSERT( p.GetPtr() == g_buffer );
        UT_ASSERT( p.OriginalPtr() == g_buffer );
    }
    {
        // Test the reference returned by SetPtr()
        XBytePtr p;
        UT_ASSERT_NO_THROW( p.SetPtr(g_buffer, g_len)++ );
        UT_ASSERT( p.Length() == g_len );
        UT_ASSERT( p.GetPtr() == g_buffer + 1 );
        UT_ASSERT( p.OriginalPtr() == g_buffer );
    }
}

void XBytePtrTest::TestIncDec()
{
    {
        // Test prefix/postfix
        XBytePtr p(g_buffer, g_len);
        UT_ASSERT_NO_THROW( ++p );
        UT_ASSERT( p.Length() == g_len );
        UT_ASSERT( p.GetPtr() == g_buffer + 1 );
        UT_ASSERT( p.OriginalPtr() == g_buffer );

        UT_ASSERT_NO_THROW( p++ );
        UT_ASSERT( p.Length() == g_len );
        UT_ASSERT( p.GetPtr() == g_buffer + 2 );
        UT_ASSERT( p.OriginalPtr() == g_buffer );

        UT_ASSERT_NO_THROW( --p );
        UT_ASSERT( p.Length() == g_len );
        UT_ASSERT( p.GetPtr() == g_buffer + 1 );
        UT_ASSERT( p.OriginalPtr() == g_buffer );

        UT_ASSERT_NO_THROW( p-- );
        UT_ASSERT( p.Length() == g_len );
        UT_ASSERT( p.GetPtr() == g_buffer );
        UT_ASSERT( p.OriginalPtr() == g_buffer );

        // Pointer back to original position.

        // Make sure pointer operations occur in the correct order.
        uint8_t *curPtr = p.GetPtr();
        UT_ASSERT( ++p == curPtr + 1 );
        curPtr = p.GetPtr();
        UT_ASSERT( p++ == curPtr );
        curPtr = p.GetPtr();
        UT_ASSERT( --p == curPtr - 1 );
        curPtr = p.GetPtr();
        UT_ASSERT( p-- == curPtr );
    }
}

void XBytePtrTest::TestPointer()
{
    XBytePtr p(g_buffer, g_len);
    for (size_t ii=0; ii<g_len; ++ii)
    {
        UT_ASSERT_NO_THROW( *p );
        UT_ASSERT_NO_THROW( p[ii] );
        UT_ASSERT( *p == g_buffer[ii] );
        UT_ASSERT( p[ii] == g_buffer[ii] );

        // Test all sorts of weird pointer usage
        UT_ASSERT( &p[ii] == &g_buffer[ii] );
        UT_ASSERT( memcmp(&p[ii], &g_buffer[ii], g_len-ii) == 0 );
        if (ii < g_len-1)
        {
            int16_t raw = *(int16_t*)&g_buffer[ii];
            int16_t ptr = *(int16_t*)&p[ii];
            UT_ASSERT( raw == ptr );
        }

        ++p;
    }
}

void XBytePtrTest::TestAddSub()
{
    {
        XBytePtr p(g_buffer, g_len);
        UT_ASSERT_NO_THROW( p + 3 );
        UT_ASSERT_NO_THROW( p - 7 );
        UT_ASSERT_NO_THROW( p += 5 );
        UT_ASSERT_NO_THROW( p -= 2 );
    }
    {
        // Test +
        XBytePtr p(g_buffer, g_len);
        UT_ASSERT( p + 3 == g_buffer + 3 );
        UT_ASSERT( p.Length() == g_len );
        UT_ASSERT( p.GetPtr() == g_buffer );
        UT_ASSERT( p.OriginalPtr() == g_buffer );
    }
    {
        // Test -
        XBytePtr p(g_buffer, g_len);
        UT_ASSERT( p - 3 == g_buffer - 3 );
        UT_ASSERT( p.Length() == g_len );
        UT_ASSERT( p.GetPtr() == g_buffer );
        UT_ASSERT( p.OriginalPtr() == g_buffer );
    }
    {
        // Test +=
        XBytePtr p(g_buffer, g_len);
        UT_ASSERT_NO_THROW( p += 6 );
        UT_ASSERT( p.Length() == g_len );
        UT_ASSERT( p.GetPtr() == g_buffer + 6 );
        UT_ASSERT( p.OriginalPtr() == g_buffer );
    }
    {
        // Test -=
        XBytePtr p(g_buffer, g_len);
        UT_ASSERT_NO_THROW( p += 6 );
        UT_ASSERT_NO_THROW( p -= 3 );
        UT_ASSERT( p.Length() == g_len );
        UT_ASSERT( p.GetPtr() == g_buffer + 3 );
        UT_ASSERT( p.OriginalPtr() == g_buffer );
    }
}

void XBytePtrTest::TestComparisons()
{
    XBytePtr begin(g_buffer, g_len);
    XBytePtr middle(g_buffer + (g_len / 2), g_len / 2);
    XBytePtr end(g_buffer + g_len - 1, 1);

    {
        UT_ASSERT_NO_THROW( begin == begin );
        UT_ASSERT_NO_THROW( begin == XBytePtr(g_buffer, g_len) );
        UT_ASSERT_NO_THROW( begin != begin );
        UT_ASSERT_NO_THROW( begin != XBytePtr(g_buffer, g_len) );
        UT_ASSERT_NO_THROW( begin <= begin );
        UT_ASSERT_NO_THROW( begin <= XBytePtr(g_buffer, g_len) );
        UT_ASSERT_NO_THROW( begin >= begin );
        UT_ASSERT_NO_THROW( begin >= XBytePtr(g_buffer, g_len) );
        UT_ASSERT_NO_THROW( begin < begin );
        UT_ASSERT_NO_THROW( begin < XBytePtr(g_buffer, g_len) );
        UT_ASSERT_NO_THROW( begin > begin );
        UT_ASSERT_NO_THROW( begin > XBytePtr(g_buffer, g_len) );
    }
    {
        // Test ==
        UT_ASSERT( begin == begin );
        UT_ASSERT( (begin == end) == false );
    }
    {
        // Test !=
        UT_ASSERT( begin != end );
        UT_ASSERT( (begin != begin) == false );
    }
    {
        // Test <=
        UT_ASSERT( begin <= end );
        UT_ASSERT( begin <= begin );
        UT_ASSERT( (end <= begin) == false );
    }
    {
        // Test >=
        UT_ASSERT( end >= begin );
        UT_ASSERT( begin >= begin );
        UT_ASSERT( (begin >= end) == false );
    }
    {
        // Test <
        UT_ASSERT( begin < end );
        UT_ASSERT( (end < begin) == false );
    }
    {
        // Test >
        UT_ASSERT( end > begin );
        UT_ASSERT( (begin > end) == false );
    }
}

void XBytePtrTest::TestCasting()
{
    {
        XBytePtr p(g_buffer, g_len);
        UT_ASSERT_NO_THROW( (char*)p );
        UT_ASSERT_NO_THROW( (unsigned char*)p );
        UT_ASSERT_NO_THROW( (short*)p );
        UT_ASSERT_NO_THROW( (unsigned short*)p );
        UT_ASSERT_NO_THROW( (int*)p );
        UT_ASSERT_NO_THROW( (unsigned int*)p );
        UT_ASSERT_NO_THROW( (long*)p );
        UT_ASSERT_NO_THROW( (unsigned long*)p );
        UT_ASSERT_NO_THROW( (long long*)p );
        UT_ASSERT_NO_THROW( (unsigned long long*)p );
        UT_ASSERT_NO_THROW( (float*)p );
        UT_ASSERT_NO_THROW( (double*)p );

        UT_ASSERT( *(char*)p == (char)g_buffer[0] );
        UT_ASSERT( *(unsigned char*)p == (unsigned char)g_buffer[0] );
        UT_ASSERT( *(short*)p == *(short*)&g_buffer );
        UT_ASSERT( *(unsigned short*)p == *(unsigned short*)&g_buffer );
        UT_ASSERT( *(int*)p == *(int*)&g_buffer );
        UT_ASSERT( *(unsigned int*)p == *(unsigned int*)&g_buffer );
        UT_ASSERT( *(long*)p == *(long*)&g_buffer );
        UT_ASSERT( *(unsigned long*)p == *(unsigned long*)&g_buffer );
        UT_ASSERT( *(long long*)p == *(long long*)&g_buffer );
        UT_ASSERT( *(unsigned long long*)p == *(unsigned long long*)&g_buffer );
        UT_ASSERT( *(float*)p == *(float*)&g_buffer );
        UT_ASSERT( *(double*)p == *(double*)&g_buffer );

        // Test bounds checking
        p += g_len - 1; // Point to last byte in buffer
        UT_ASSERT_NO_THROW( (char*)p );
        UT_ASSERT_NO_THROW( (unsigned char*)p );
        UT_ASSERT_THROWS( (short*)p, XException ); // Casting and dereferencing would cause out-of-bounds access.
        UT_ASSERT_THROWS( (unsigned short*)p, XException );
        UT_ASSERT_THROWS( (int*)p, XException );
        UT_ASSERT_THROWS( (unsigned int*)p, XException );
        UT_ASSERT_THROWS( (long*)p, XException );
        UT_ASSERT_THROWS( (unsigned long*)p, XException );
        UT_ASSERT_THROWS( (long long*)p, XException );
        UT_ASSERT_THROWS( (unsigned long long*)p, XException );
        UT_ASSERT_THROWS( (float*)p, XException );
        UT_ASSERT_THROWS( (double*)p, XException );
    }
    {
        // Test bounds checking when pointer points BEFORE start of buffer
        XBytePtr p(g_buffer, g_len);
        UT_ASSERT_NO_THROW( --p ); // pointer before head of buffer
        UT_ASSERT_THROWS( (char*)p, XException );
        UT_ASSERT_THROWS( (unsigned char*)p, XException );
        UT_ASSERT_THROWS( (short*)p, XException );
        UT_ASSERT_THROWS( (unsigned short*)p, XException );
        UT_ASSERT_THROWS( (int*)p, XException );
        UT_ASSERT_THROWS( (unsigned int*)p, XException );
        UT_ASSERT_THROWS( (long*)p, XException );
        UT_ASSERT_THROWS( (unsigned long*)p, XException );
        UT_ASSERT_THROWS( (long long*)p, XException );
        UT_ASSERT_THROWS( (unsigned long long*)p, XException );
        UT_ASSERT_THROWS( (float*)p, XException );
        UT_ASSERT_THROWS( (double*)p, XException );
    }
    {
        // Test specific cases Tony had problems with.
        XBytePtr p(g_buffer, g_len);
        uint32_t a = *(uint32_t*)p;
        a++; // Prevents 'unused variable' warning
        uint16_t b;
        b = *(uint16_t*)p;
    }
}

void XBytePtrTest::TestOffset()
{
    {
        XBytePtr p(g_buffer, g_len);
        UT_ASSERT_NO_THROW( p.Offset() );
        UT_ASSERT_NO_THROW( p.Remaining() );
    }
    {
        XBytePtr p(g_buffer, g_len);
        p += 5;
        UT_ASSERT( p.Offset() == 5 );
        UT_ASSERT( p.Remaining() == 3 );
    }
    {
        XBytePtr p(g_buffer, g_len);
        p--;
        UT_ASSERT( p.Offset() == -1 );
        UT_ASSERT( p.InBounds() == false );
        p++;
        p += g_len;
        UT_ASSERT( p.Remaining() == 0 );
        UT_ASSERT( p.InBounds() == false );
        p++;
        UT_ASSERT( p.Remaining() == -1 );
        UT_ASSERT( p.InBounds() == false );
    }
}

void XBytePtrTest::TestSeek()
{
    {
        XBytePtr p(g_buffer, g_len);
        UT_ASSERT_NO_THROW( p.Seek(1) );
        UT_ASSERT_THROWS( p.Seek(g_len), XException );
        UT_ASSERT_THROWS( p.Seek(-1), XException );
    }
    {
        XBytePtr p(g_buffer, g_len);
        UT_ASSERT( p.Seek(3).Offset() == 3 );
        UT_ASSERT( p.Remaining() == (ptrdiff_t)g_len - p.Offset() );
    }
}

void XBytePtrTest::TestReadWrite()
{
    {
        // Read
        XBytePtr p(g_buffer, g_len);
        UT_ASSERT( p.Read<uint32_t>() == *(uint32_t*)&g_buffer[0] );
        UT_ASSERT( p.Consume<uint32_t>() == *(uint32_t*)&g_buffer[0] ); // Advances pointer
        UT_ASSERT( p.Consume<uint32_t>() == *(uint32_t*)&g_buffer[sizeof(uint32_t)] );

        // Test bounds
        p.Seek(g_len-1);
        UT_ASSERT_NO_THROW( p.Read<uint8_t>() );
        UT_ASSERT_THROWS( p.Read<uint16_t>(), XException );
    }
    {
        // Write

        // Make a copy of the buffer for validation later
        XIRef<XMemory> cpy = new XMemory(g_len);
        memcpy(&cpy->Extend(g_len), g_buffer, g_len);

        XBytePtr p(g_buffer, g_len);
        int16_t temp = *(int16_t*)p;
        UT_ASSERT( p.Write<int16_t>(-42) == -42 );
        UT_ASSERT( p.Offset() == sizeof(int16_t) ); // Make sure ptr moved forward

        // Make sure we wrote the data
        p.Seek(0);
        UT_ASSERT( memcmp(g_buffer, p, g_len) == 0 );
        UT_ASSERT( memcmp(g_buffer, cpy->Map(), g_len) != 0 );
        UT_ASSERT( *(int16_t*)p == -42 );
        UT_ASSERT( *(int16_t*)g_buffer == -42 );

        // Now put the data back
        p.Write<int16_t>(temp);
        p.Seek(0);
        UT_ASSERT( memcmp(g_buffer, p, g_len) == 0 );
        UT_ASSERT( memcmp(g_buffer, cpy->Map(), g_len) == 0 );

        // Test bounds
        p.Seek(g_len-1);
        UT_ASSERT_NO_THROW( p.Write<uint8_t>( g_buffer[g_len-1] ) );
        p.Seek(g_len-1);
        UT_ASSERT_THROWS( p.Write<uint16_t>( 0xa5f0 ), XException );
    }

    {
        uint8_t buffer[] = {59, 42, 209, 123, 65, 1, 0, 0};
        XBytePtr ptr = XBytePtr(buffer, sizeof(buffer));
        UT_ASSERT(ptr.Read<uint64_t>() == 1380761807419);
        UT_ASSERT(ptr.Remaining() == 8);
        UT_ASSERT(ptr.Consume<uint64_t>() == 1380761807419);
        UT_ASSERT(ptr.Remaining() == 0);
    }
}

void XBytePtrTest::TestBulkWrite()
{
    // Make a copy of the buffer for validation later
    XIRef<XMemory> newBuffer = new XMemory( g_len );
    memset( &newBuffer->Extend(g_len), 0, g_len );

    {
        XBytePtr p( newBuffer->Map(), newBuffer->GetDataSize() );
        UT_ASSERT_NO_THROW( p.BulkWrite( g_buffer, g_len ) );
        UT_ASSERT_THROWS( p.BulkWrite( g_buffer, g_len ), XException );
    }

    UT_ASSERT( memcmp( newBuffer->Map(), g_buffer, g_len ) == 0 );
}
