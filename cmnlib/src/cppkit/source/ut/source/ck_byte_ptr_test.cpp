
#include "framework.h"
#include "ck_byte_ptr_test.h"
#include "cppkit/ck_byte_ptr.h"
#include "cppkit/ck_memory.h"

using namespace std;
using namespace cppkit;

REGISTER_TEST_FIXTURE(ck_byte_ptr_test);

static uint8_t test_buffer[]= {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
};

static const size_t test_buffer_len = sizeof(test_buffer)/sizeof(test_buffer[0]);

void ck_byte_ptr_test::setup()
{
}

void ck_byte_ptr_test::teardown()
{
}

void ck_byte_ptr_test::test_constructor()
{
    {
        // Default ctor
        UT_ASSERT_NO_THROW(ck_byte_ptr());
        ck_byte_ptr p;
        UT_ASSERT( p.length() == 0 );
        UT_ASSERT( p.get_ptr() == 0 );
        UT_ASSERT( p.original_ptr() == 0 );
    }
    {
        UT_ASSERT_NO_THROW( ck_byte_ptr(test_buffer, test_buffer_len) );
        ck_byte_ptr p(test_buffer, test_buffer_len);
        UT_ASSERT( p.length() == test_buffer_len );
        UT_ASSERT( p.get_ptr() == test_buffer );
        UT_ASSERT( p.original_ptr() == test_buffer );
    }
    {
        // Copy ctor
        ck_byte_ptr p(test_buffer, test_buffer_len);
        UT_ASSERT_NO_THROW( ++p );
        UT_ASSERT_NO_THROW( ck_byte_ptr(p) );
        ck_byte_ptr copy(p);
        UT_ASSERT( copy.length() == test_buffer_len );
        UT_ASSERT( copy.get_ptr() == test_buffer + 1 );
        UT_ASSERT( copy.original_ptr() == test_buffer );
    }
}

void ck_byte_ptr_test::test_get_set_ptr()
{
    {
        ck_byte_ptr p;
        UT_ASSERT_NO_THROW( p.set_ptr(test_buffer, test_buffer_len) );
        UT_ASSERT( p.length() == test_buffer_len );
        UT_ASSERT( p.get_ptr() == test_buffer );
        UT_ASSERT( p.original_ptr() == test_buffer );
    }
    {
        // Test the reference returned by set_ptr()
        ck_byte_ptr p;
        UT_ASSERT_NO_THROW( p.set_ptr(test_buffer, test_buffer_len)++ );
        UT_ASSERT( p.length() == test_buffer_len );
        UT_ASSERT( p.get_ptr() == test_buffer + 1 );
        UT_ASSERT( p.original_ptr() == test_buffer );
    }
}

void ck_byte_ptr_test::test_inc_dec()
{
    {
        // Test prefix/postfix
        ck_byte_ptr p(test_buffer, test_buffer_len);
        UT_ASSERT_NO_THROW( ++p );
        UT_ASSERT( p.length() == test_buffer_len );
        UT_ASSERT( p.get_ptr() == test_buffer + 1 );
        UT_ASSERT( p.original_ptr() == test_buffer );

        UT_ASSERT_NO_THROW( p++ );
        UT_ASSERT( p.length() == test_buffer_len );
        UT_ASSERT( p.get_ptr() == test_buffer + 2 );
        UT_ASSERT( p.original_ptr() == test_buffer );

        UT_ASSERT_NO_THROW( --p );
        UT_ASSERT( p.length() == test_buffer_len );
        UT_ASSERT( p.get_ptr() == test_buffer + 1 );
        UT_ASSERT( p.original_ptr() == test_buffer );

        UT_ASSERT_NO_THROW( p-- );
        UT_ASSERT( p.length() == test_buffer_len );
        UT_ASSERT( p.get_ptr() == test_buffer );
        UT_ASSERT( p.original_ptr() == test_buffer );

        // Pointer back to original position.

        // Make sure pointer operations occur in the correct order.
        uint8_t *curPtr = p.get_ptr();
        UT_ASSERT( ++p == curPtr + 1 );
        curPtr = p.get_ptr();
        UT_ASSERT( p++ == curPtr );
        curPtr = p.get_ptr();
        UT_ASSERT( --p == curPtr - 1 );
        curPtr = p.get_ptr();
        UT_ASSERT( p-- == curPtr );
    }
}

void ck_byte_ptr_test::test_pointer()
{
    ck_byte_ptr p(test_buffer, test_buffer_len);
    for (size_t ii=0; ii<test_buffer_len; ++ii)
    {
        UT_ASSERT_NO_THROW( *p );
        UT_ASSERT_NO_THROW( p[ii] );
        UT_ASSERT( *p == test_buffer[ii] );
        UT_ASSERT( p[ii] == test_buffer[ii] );

        // Test all sorts of weird pointer usage
        UT_ASSERT( &p[ii] == &test_buffer[ii] );
        UT_ASSERT( memcmp(&p[ii], &test_buffer[ii], test_buffer_len-ii) == 0 );
        if (ii < test_buffer_len-1)
        {
            int16_t raw = *(int16_t*)&test_buffer[ii];
            int16_t ptr = *(int16_t*)&p[ii];
            UT_ASSERT( raw == ptr );
        }

        ++p;
    }
}

void ck_byte_ptr_test::test_add_sub()
{
    {
        ck_byte_ptr p(test_buffer, test_buffer_len);
        UT_ASSERT_NO_THROW( p + 3 );
        UT_ASSERT_NO_THROW( p - 7 );
        UT_ASSERT_NO_THROW( p += 5 );
        UT_ASSERT_NO_THROW( p -= 2 );
    }
    {
        // Test +
        ck_byte_ptr p(test_buffer, test_buffer_len);
        UT_ASSERT( p + 3 == test_buffer + 3 );
        UT_ASSERT( p.length() == test_buffer_len );
        UT_ASSERT( p.get_ptr() == test_buffer );
        UT_ASSERT( p.original_ptr() == test_buffer );
    }
    {
        // Test -
        ck_byte_ptr p(test_buffer, test_buffer_len);
        UT_ASSERT( p - 3 == test_buffer - 3 );
        UT_ASSERT( p.length() == test_buffer_len );
        UT_ASSERT( p.get_ptr() == test_buffer );
        UT_ASSERT( p.original_ptr() == test_buffer );
    }
    {
        // Test +=
        ck_byte_ptr p(test_buffer, test_buffer_len);
        UT_ASSERT_NO_THROW( p += 6 );
        UT_ASSERT( p.length() == test_buffer_len );
        UT_ASSERT( p.get_ptr() == test_buffer + 6 );
        UT_ASSERT( p.original_ptr() == test_buffer );
    }
    {
        // Test -=
        ck_byte_ptr p(test_buffer, test_buffer_len);
        UT_ASSERT_NO_THROW( p += 6 );
        UT_ASSERT_NO_THROW( p -= 3 );
        UT_ASSERT( p.length() == test_buffer_len );
        UT_ASSERT( p.get_ptr() == test_buffer + 3 );
        UT_ASSERT( p.original_ptr() == test_buffer );
    }
}

void ck_byte_ptr_test::test_comparisons()
{
    ck_byte_ptr begin(test_buffer, test_buffer_len);
    ck_byte_ptr middle(test_buffer + (test_buffer_len / 2), test_buffer_len / 2);
    ck_byte_ptr end(test_buffer + test_buffer_len - 1, 1);

    {
        UT_ASSERT_NO_THROW( begin == begin );
        UT_ASSERT_NO_THROW( begin == ck_byte_ptr(test_buffer, test_buffer_len) );
        UT_ASSERT_NO_THROW( begin != begin );
        UT_ASSERT_NO_THROW( begin != ck_byte_ptr(test_buffer, test_buffer_len) );
        UT_ASSERT_NO_THROW( begin <= begin );
        UT_ASSERT_NO_THROW( begin <= ck_byte_ptr(test_buffer, test_buffer_len) );
        UT_ASSERT_NO_THROW( begin >= begin );
        UT_ASSERT_NO_THROW( begin >= ck_byte_ptr(test_buffer, test_buffer_len) );
        UT_ASSERT_NO_THROW( begin < begin );
        UT_ASSERT_NO_THROW( begin < ck_byte_ptr(test_buffer, test_buffer_len) );
        UT_ASSERT_NO_THROW( begin > begin );
        UT_ASSERT_NO_THROW( begin > ck_byte_ptr(test_buffer, test_buffer_len) );
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

void ck_byte_ptr_test::test_casting()
{
    {
        ck_byte_ptr p(test_buffer, test_buffer_len);
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

        UT_ASSERT( *(char*)p == (char)test_buffer[0] );
        UT_ASSERT( *(unsigned char*)p == (unsigned char)test_buffer[0] );
        UT_ASSERT( *(short*)p == *(short*)&test_buffer );
        UT_ASSERT( *(unsigned short*)p == *(unsigned short*)&test_buffer );
        UT_ASSERT( *(int*)p == *(int*)&test_buffer );
        UT_ASSERT( *(unsigned int*)p == *(unsigned int*)&test_buffer );
        UT_ASSERT( *(long*)p == *(long*)&test_buffer );
        UT_ASSERT( *(unsigned long*)p == *(unsigned long*)&test_buffer );
        UT_ASSERT( *(long long*)p == *(long long*)&test_buffer );
        UT_ASSERT( *(unsigned long long*)p == *(unsigned long long*)&test_buffer );
        UT_ASSERT( *(float*)p == *(float*)&test_buffer );
        UT_ASSERT( *(double*)p == *(double*)&test_buffer );

        // Test bounds checking
        p += test_buffer_len - 1; // Point to last byte in buffer
        UT_ASSERT_NO_THROW( (char*)p );
        UT_ASSERT_NO_THROW( (unsigned char*)p );
        UT_ASSERT_THROWS( (short*)p, ck_exception ); // Casting and dereferencing would cause out-of-bounds access.
        UT_ASSERT_THROWS( (unsigned short*)p, ck_exception );
        UT_ASSERT_THROWS( (int*)p, ck_exception );
        UT_ASSERT_THROWS( (unsigned int*)p, ck_exception );
        UT_ASSERT_THROWS( (long*)p, ck_exception );
        UT_ASSERT_THROWS( (unsigned long*)p, ck_exception );
        UT_ASSERT_THROWS( (long long*)p, ck_exception );
        UT_ASSERT_THROWS( (unsigned long long*)p, ck_exception );
        UT_ASSERT_THROWS( (float*)p, ck_exception );
        UT_ASSERT_THROWS( (double*)p, ck_exception );
    }
    {
        // Test bounds checking when pointer points BEFORE start of buffer
        ck_byte_ptr p(test_buffer, test_buffer_len);
        UT_ASSERT_NO_THROW( --p ); // pointer before head of buffer
        UT_ASSERT_THROWS( (char*)p, ck_exception );
        UT_ASSERT_THROWS( (unsigned char*)p, ck_exception );
        UT_ASSERT_THROWS( (short*)p, ck_exception );
        UT_ASSERT_THROWS( (unsigned short*)p, ck_exception );
        UT_ASSERT_THROWS( (int*)p, ck_exception );
        UT_ASSERT_THROWS( (unsigned int*)p, ck_exception );
        UT_ASSERT_THROWS( (long*)p, ck_exception );
        UT_ASSERT_THROWS( (unsigned long*)p, ck_exception );
        UT_ASSERT_THROWS( (long long*)p, ck_exception );
        UT_ASSERT_THROWS( (unsigned long long*)p, ck_exception );
        UT_ASSERT_THROWS( (float*)p, ck_exception );
        UT_ASSERT_THROWS( (double*)p, ck_exception );
    }
    {
        // Test specific cases Tony had problems with.
        ck_byte_ptr p(test_buffer, test_buffer_len);
        uint32_t a = *(uint32_t*)p;
        a++; // Prevents 'unused variable' warning
        uint16_t b;
        b = *(uint16_t*)p;
    }
}

void ck_byte_ptr_test::test_offset()
{
    {
        ck_byte_ptr p(test_buffer, test_buffer_len);
        UT_ASSERT_NO_THROW( p.offset() );
        UT_ASSERT_NO_THROW( p.remaining() );
    }
    {
        ck_byte_ptr p(test_buffer, test_buffer_len);
        p += 5;
        UT_ASSERT( p.offset() == 5 );
        UT_ASSERT( p.remaining() == 3 );
    }
    {
        ck_byte_ptr p(test_buffer, test_buffer_len);
        p--;
        UT_ASSERT( p.offset() == -1 );
        UT_ASSERT( p.in_bounds() == false );
        p++;
        p += test_buffer_len;
        UT_ASSERT( p.remaining() == 0 );
        UT_ASSERT( p.in_bounds() == false );
        p++;
        UT_ASSERT( p.remaining() == -1 );
        UT_ASSERT( p.in_bounds() == false );
    }
}

void ck_byte_ptr_test::test_seek()
{
    {
        ck_byte_ptr p(test_buffer, test_buffer_len);
        UT_ASSERT_NO_THROW( p.seek(1) );
        UT_ASSERT_THROWS( p.seek(test_buffer_len), ck_exception );
        UT_ASSERT_THROWS( p.seek(-1), ck_exception );
    }
    {
        ck_byte_ptr p(test_buffer, test_buffer_len);
        UT_ASSERT( p.seek(3).offset() == 3 );
        UT_ASSERT( p.remaining() == (ptrdiff_t)test_buffer_len - p.offset() );
    }
}

void ck_byte_ptr_test::test_read_write()
{
    {
        // read
        ck_byte_ptr p(test_buffer, test_buffer_len);
        UT_ASSERT( p.read<uint32_t>() == *(uint32_t*)&test_buffer[0] );
        UT_ASSERT( p.consume<uint32_t>() == *(uint32_t*)&test_buffer[0] ); // Advances pointer
        UT_ASSERT( p.consume<uint32_t>() == *(uint32_t*)&test_buffer[sizeof(uint32_t)] );

        // Test bounds
        p.seek(test_buffer_len-1);
        UT_ASSERT_NO_THROW( p.read<uint8_t>() );
        UT_ASSERT_THROWS( p.read<uint16_t>(), ck_exception );
    }
    {
        // write

        // Make a copy of the buffer for validation later
        shared_ptr<ck_memory> cpy = make_shared<ck_memory>(test_buffer_len);
        memcpy(cpy->extend_data(test_buffer_len), test_buffer, test_buffer_len);

        ck_byte_ptr p(test_buffer, test_buffer_len);
        int16_t temp = *(int16_t*)p;
        UT_ASSERT( p.write<int16_t>(-42) == -42 );
        UT_ASSERT( p.offset() == sizeof(int16_t) ); // Make sure ptr moved forward

        // Make sure we wrote the data
        p.seek(0);
        UT_ASSERT( memcmp(test_buffer, p, test_buffer_len) == 0 );
        UT_ASSERT( memcmp(test_buffer, cpy->map().get_ptr(), test_buffer_len) != 0 );
        UT_ASSERT( *(int16_t*)p == -42 );
        UT_ASSERT( *(int16_t*)test_buffer == -42 );

        // Now put the data back
        p.write<int16_t>(temp);
        p.seek(0);
        UT_ASSERT( memcmp(test_buffer, p, test_buffer_len) == 0 );
        UT_ASSERT( memcmp(test_buffer, cpy->map().get_ptr(), test_buffer_len) == 0 );

        // Test bounds
        p.seek(test_buffer_len-1);
        UT_ASSERT_NO_THROW( p.write<uint8_t>( test_buffer[test_buffer_len-1] ) );
        p.seek(test_buffer_len-1);
        UT_ASSERT_THROWS( p.write<uint16_t>( 0xa5f0 ), ck_exception );
    }
}

void ck_byte_ptr_test::test_bulk_write()
{
    // Make a copy of the buffer for validation later
    shared_ptr<ck_memory> new_buffer = make_shared<ck_memory>(test_buffer_len);
    memset( new_buffer->extend_data(test_buffer_len), 0, test_buffer_len );

    {
        ck_byte_ptr p = new_buffer->map();
        UT_ASSERT_NO_THROW(p.bulk_write( test_buffer, test_buffer_len ));
        UT_ASSERT_THROWS(p.bulk_write( test_buffer, test_buffer_len ), ck_exception);
    }

    UT_ASSERT(memcmp(new_buffer->map().get_ptr(), test_buffer, test_buffer_len) == 0);
}
