
#include "av_packet_test.h"
#include "ffkit/av_packet.h"

using namespace std;
using namespace cppkit;
using namespace ffkit;

REGISTER_TEST_FIXTURE(av_packet_test);

void av_packet_test::setup()
{
}

void av_packet_test::teardown()
{
}

void av_packet_test::test_size_ctor_owning()
{
    {
        shared_ptr<av_packet> pkt = make_shared<av_packet>(32768);
        UT_ASSERT( pkt->_bufferSize == 32768 );
        UT_ASSERT( pkt->_requestedSize == 32768 );
        UT_ASSERT( pkt->_owning == true );
        UT_ASSERT( pkt->_buffer != NULL );
        UT_ASSERT( pkt->_dataSize == 0 );
        UT_ASSERT( pkt->_ts == 0 );
        UT_ASSERT( pkt->_ticksInSecond == 90000 );

        pkt->set_data_size( 16384 );
        UT_ASSERT( pkt->get_data_size() == 16384 );

        uint8_t* writer = pkt->map();
        for( size_t i = 0; i < 32768; i++ )
            *writer++ = 1;

        size_t sum = 0;
        uint8_t* reader = pkt->map();
        for( size_t i = 0; i < 32768; i++ )
            sum += *reader++;

        UT_ASSERT( sum == 32768 );
    }

    {
        // test whether the buffer pads itself correctly
        shared_ptr<av_packet> pkt = make_shared<av_packet>(32766);
        UT_ASSERT( pkt->_bufferSize == 32768 );
        UT_ASSERT( pkt->_requestedSize == 32766 );
        UT_ASSERT( pkt->_owning == true );
        UT_ASSERT( pkt->_buffer != NULL );
        UT_ASSERT( pkt->_dataSize == 0 );
        UT_ASSERT( pkt->_ts == 0 );
        UT_ASSERT( pkt->_ticksInSecond == 90000 );
    }
}

void av_packet_test::test_foreign_buffer_ctor_owning()
{
    uint8_t foreignBuffer[16384];
    for( size_t i = 0; i < 16384; i++ )
        foreignBuffer[i] = 42;

    shared_ptr<av_packet> pkt = make_shared<av_packet>( foreignBuffer, 16384 );

    uint8_t* src = pkt->map();
    uint8_t* cmp = &foreignBuffer[0];
    for( size_t i = 0; i < 16384; i++ )
        UT_ASSERT( *src++ == *cmp++ );

    UT_ASSERT( pkt->_owning == true );
    UT_ASSERT( pkt->_buffer != &foreignBuffer[0] );
}

void av_packet_test::test_foreign_buffer_ctor_non_owning()
{
    uint8_t foreignBuffer[16384];
    for( size_t i = 0; i < 16384; i++ )
        foreignBuffer[i] = 42;

    shared_ptr<av_packet> pkt = make_shared<av_packet>( foreignBuffer, 16384, false );

    uint8_t* src = pkt->map();
    UT_ASSERT( pkt->_owning == false );
    UT_ASSERT( pkt->_buffer == &foreignBuffer[0] );
}

void av_packet_test::test_copy_ctor_owning()
{
    uint8_t foreignBuffer[16384];
    for( size_t i = 0; i < 16384; i++ )
        foreignBuffer[i] = 42;

    av_packet pktA( foreignBuffer, 16384 );
    av_packet pktB = pktA;

    UT_ASSERT( pktB._bufferSize == pktA._bufferSize );
    UT_ASSERT( pktB._requestedSize == pktA._requestedSize );
    UT_ASSERT( pktB._owning == pktA._owning );
    UT_ASSERT( pktB._buffer != pktA._buffer );
    UT_ASSERT( pktB._dataSize == pktA._dataSize );
    UT_ASSERT( pktB._ts == pktA._ts );
    UT_ASSERT( pktB._ticksInSecond == pktA._ticksInSecond );

    for( size_t i = 0; i < 16384; i++ )
        UT_ASSERT( pktB._buffer[i] == 42 );
}

void av_packet_test::test_copy_ctor_non_owning()
{
    uint8_t foreignBuffer[16384];
    for( size_t i = 0; i < 16384; i++ )
        foreignBuffer[i] = 42;

    av_packet pktA( foreignBuffer, 16384, false );
    av_packet pktB = pktA;

    UT_ASSERT( pktB._bufferSize == pktA._bufferSize );
    UT_ASSERT( pktB._requestedSize == pktA._requestedSize );
    UT_ASSERT( pktB._owning == pktA._owning );
    UT_ASSERT( pktB._buffer == pktA._buffer );
    UT_ASSERT( pktB._dataSize == pktA._dataSize );
    UT_ASSERT( pktB._ts == pktA._ts );
    UT_ASSERT( pktB._ticksInSecond == pktA._ticksInSecond );

    UT_ASSERT( pktA._buffer == &foreignBuffer[0] );
    UT_ASSERT( pktB._buffer == &foreignBuffer[0] );
}

void av_packet_test::test_move_ctor()
{
    uint8_t foreignBuffer[16384];
    for( size_t i = 0; i < 16384; i++ )
        foreignBuffer[i] = 42;

    av_packet pktA( foreignBuffer, 16384 );
    av_packet pktB = std::move( pktA );

    UT_ASSERT( pktA._bufferSize == 0 );
    UT_ASSERT( pktA._buffer == NULL );
}

void av_packet_test::test_copy_assignment()
{
    uint8_t foreignBuffer[16384];
    for( size_t i = 0; i < 16384; i++ )
        foreignBuffer[i] = 42;

    av_packet pktA( foreignBuffer, 16384 );
    av_packet pktB( 1024 );

    pktB = pktA;

    UT_ASSERT( pktB._bufferSize == pktA._bufferSize );
    UT_ASSERT( pktB._requestedSize == pktA._requestedSize );
    UT_ASSERT( pktB._owning == pktA._owning );
    UT_ASSERT( pktB._buffer != pktA._buffer );
    UT_ASSERT( pktB._dataSize == pktA._dataSize );
    UT_ASSERT( pktB._ts == pktA._ts );
    UT_ASSERT( pktB._ticksInSecond == pktA._ticksInSecond );

    for( size_t i = 0; i < 16384; i++ )
        UT_ASSERT( pktB._buffer[i] == 42 );
}

void av_packet_test::test_move_assignment()
{
    uint8_t foreignBuffer[16384];
    for( size_t i = 0; i < 16384; i++ )
        foreignBuffer[i] = 42;

    av_packet pktA( foreignBuffer, 16384 );
    av_packet pktB( 1024 );

    pktB = std::move( pktA );

    UT_ASSERT( pktA._bufferSize == 0 );
    UT_ASSERT( pktA._buffer == NULL );
}
