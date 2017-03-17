
#include "framework.h"
#include "ck_memory_test.h"
#include "cppkit/ck_memory.h"

using namespace std;
using namespace cppkit;

REGISTER_TEST_FIXTURE(ck_memory_test);

void ck_memory_test::setup()
{
}

void ck_memory_test::teardown()
{
}

void ck_memory_test::test_default_constructor()
{
    shared_ptr<ck_memory> p;
    UT_ASSERT_NO_THROW(p = make_shared<ck_memory>());
}

void ck_memory_test::test_sized_constructor()
{
    shared_ptr<ck_memory> p;
    UT_ASSERT_NO_THROW(p = make_shared<ck_memory>(16384));
}

void ck_memory_test::test_copy_constructor()
{
    ck_memory buffer;
    memset(buffer.extend_data(16384).get_ptr(), 42, 16384);
    ck_memory buffer2 = buffer;

    UT_ASSERT(memcmp(buffer2.map().get_ptr(), buffer.map().get_ptr(), buffer.size_data()) == 0);
}

void ck_memory_test::test_assignment_operator()
{
    ck_memory buffer;
    memset(buffer.extend_data(16384).get_ptr(), 42, 16384);
    ck_memory buffer2;
    memset(buffer2.extend_data(32768).get_ptr(), 69, 32768);

    buffer2 = buffer;

    UT_ASSERT(buffer2.size() == buffer.size());
    UT_ASSERT(buffer2.size_data() == buffer.size_data());

    UT_ASSERT(memcmp(buffer2.map().get_ptr(), buffer.map().get_ptr(), buffer.size()) == 0);
}

void ck_memory_test::test_map()
{
    ck_memory buffer(16384);

    ck_byte_ptr p = buffer.map();

    UT_ASSERT(p.length() == 16384);

    memset(p.get_ptr(), 42, 16384);

    for( ; p.remaining(); p++ )
    {
        UT_ASSERT(p.read<uint8_t>() == 42);
    }
}

void ck_memory_test::test_size()
{
    ck_memory buffer(16384);

    UT_ASSERT(buffer.size() == 16384);
}

void ck_memory_test::test_resize()
{
    ck_memory buffer(16384);

    UT_ASSERT(buffer.size() == 16384);

    buffer.set_data_size(16384);

    buffer.resize(8192);

    UT_ASSERT(buffer.size() == 8192);
    UT_ASSERT(buffer.size_data() == 8192);
}

void ck_memory_test::test_extend_data()
{
    ck_memory buffer(16384);

    UT_ASSERT(buffer.size() == 16384);
    UT_ASSERT(buffer.size_data() == 0);

    for( size_t i = 0; i < 2048; i++ )
    {
        ck_byte_ptr p = buffer.extend_data(8);
        memset(p.get_ptr(),42,8);
    }

    UT_ASSERT(buffer.size() == 16384);
    UT_ASSERT(buffer.size_data() == 16384);

    ck_byte_ptr p = buffer.map();

    for( ; p.remaining(); p++ )
    {
        UT_ASSERT(p.read<uint8_t>() == 42);
    }
}

void ck_memory_test::test_set_data_size()
{
    ck_memory buffer;
    memset(buffer.extend_data(16384).get_ptr(), 42, 16384);

    buffer.set_data_size(8192);

    UT_ASSERT(buffer.size_data() == 8192);

    ck_byte_ptr p = buffer.map_data();

    UT_ASSERT(p.length() == 8192);
}

void ck_memory_test::test_map_data()
{
    ck_memory buffer;
    memset(buffer.extend_data(16384).get_ptr(), 42, 16384);

    ck_byte_ptr p = buffer.map();

    UT_ASSERT(p.length() == 16384);

    for( ; p.remaining(); p++ )
    {
        UT_ASSERT(p.read<uint8_t>() == 42);
    }
}

void ck_memory_test::test_size_data()
{
    ck_memory buffer;
    memset(buffer.extend_data(16384).get_ptr(), 42, 16384);

    UT_ASSERT(buffer.size_data() == 16384);

    buffer.set_data_size(8192);

    UT_ASSERT(buffer.size_data() == 8192);
}
