
#include "XBitsyTest.h"

#include <vector>
#include "XSDK/XBitsy.h"
#include "XSDK/XException.h"

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XBitsyTest);

void XBitsyTest::TestConstructor()
{
    vector<uint8_t> buffer(12, 7);

    {
        const uint8_t* begin = &buffer.front();
        const uint8_t* end = &buffer.front() + buffer.size();
        XBitsy<const uint8_t*> bitsy(begin, end);
        UT_ASSERT(bitsy._begin == begin);
        UT_ASSERT(bitsy._end == end);
        UT_ASSERT(bitsy._pos == begin);
        UT_ASSERT(bitsy._currentMask == 128);

        bitsy.GetBits(19);
        XBitsy<const uint8_t*> copy = bitsy;
        UT_ASSERT(copy._begin == begin);
        UT_ASSERT(copy._end == end);
        UT_ASSERT(copy._pos == begin + 2);
        UT_ASSERT(bitsy._currentMask == 16);

        copy.GetBits(5);
        UT_ASSERT(bitsy._begin == begin);
        UT_ASSERT(bitsy._end == end);
        UT_ASSERT(bitsy._pos == begin + 2);
        UT_ASSERT(bitsy._currentMask == 16);

        bitsy = copy;
        UT_ASSERT(bitsy._begin == begin);
        UT_ASSERT(bitsy._end == end);
        UT_ASSERT(bitsy._pos == begin + 3);
        UT_ASSERT(bitsy._currentMask == 128);
    }

    {
        vector<uint8_t>::const_iterator begin = buffer.begin();
        vector<uint8_t>::const_iterator end = buffer.end();
        XBitsy<vector<uint8_t>::const_iterator> bitsy(begin, end);
        UT_ASSERT(bitsy._begin == begin);
        UT_ASSERT(bitsy._end == end);
        UT_ASSERT(bitsy._pos == begin);
        UT_ASSERT(bitsy._currentMask == 128);

        bitsy.GetBits(19);
        XBitsy<vector<uint8_t>::const_iterator> copy = bitsy;
        UT_ASSERT(copy._begin == begin);
        UT_ASSERT(copy._end == end);
        UT_ASSERT(copy._pos == begin + 2);
        UT_ASSERT(bitsy._currentMask == 16);

        copy.GetBits(5);
        UT_ASSERT(bitsy._begin == begin);
        UT_ASSERT(bitsy._end == end);
        UT_ASSERT(bitsy._pos == begin + 2);
        UT_ASSERT(bitsy._currentMask == 16);

        bitsy = copy;
        UT_ASSERT(bitsy._begin == begin);
        UT_ASSERT(bitsy._end == end);
        UT_ASSERT(bitsy._pos == begin + 3);
        UT_ASSERT(bitsy._currentMask == 128);
    }
}

void XBitsyTest::TestSetBuffer()
{
    {
        vector<uint8_t> buffer(12, 7);
        XBitsy<const uint8_t*> bitsy(&buffer.front(), &buffer.front() + buffer.size());
        bitsy.GetBits(19);

        vector<uint8_t> other(5, 2);
        bitsy.SetBuffer(&other.front(), &other.front() + other.size());
        UT_ASSERT(bitsy._begin == &other.front());
        UT_ASSERT(bitsy._end == &other.front() + other.size());
        UT_ASSERT(bitsy._pos == &other.front());
        UT_ASSERT(bitsy._currentMask == 128);
    }

    {
        vector<uint8_t> buffer(12, 7);
        XBitsy<vector<uint8_t>::const_iterator> bitsy(buffer.begin(), buffer.end());
        bitsy.GetBits(19);

        vector<uint8_t> other(5, 2);
        bitsy.SetBuffer(other.begin(), other.end());
        UT_ASSERT(bitsy._begin == other.begin());
        UT_ASSERT(bitsy._end == other.end());
        UT_ASSERT(bitsy._pos == other.begin());
        UT_ASSERT(bitsy._currentMask == 128);
    }
}

void XBitsyTest::TestReset()
{
    vector<uint8_t> buffer(12, 7);

    {
        const uint8_t* begin = &buffer.front();
        const uint8_t* end = &buffer.front() + buffer.size();
        XBitsy<const uint8_t*> bitsy(begin, end);
        bitsy.GetBits(19);
        UT_ASSERT(bitsy._begin == begin);
        UT_ASSERT(bitsy._end == end);
        UT_ASSERT(bitsy._pos == begin + 2);
        UT_ASSERT(bitsy._currentMask == 16);

        bitsy.Reset();
        UT_ASSERT(bitsy._begin == begin);
        UT_ASSERT(bitsy._end == end);
        UT_ASSERT(bitsy._pos == &buffer.front());
        UT_ASSERT(bitsy._currentMask == 128);
        UT_ASSERT(bitsy._bytesRemaining == -1);
    }

    {
        vector<uint8_t>::const_iterator begin = buffer.begin();
        vector<uint8_t>::const_iterator end = buffer.end();
        XBitsy<vector<uint8_t>::const_iterator> bitsy(begin, end);
        bitsy.GetBits(19);
        UT_ASSERT(bitsy._begin == begin);
        UT_ASSERT(bitsy._end == end);
        UT_ASSERT(bitsy._pos == begin + 2);
        UT_ASSERT(bitsy._currentMask == 16);

        bitsy.Reset();
        UT_ASSERT(bitsy._begin == begin);
        UT_ASSERT(bitsy._end == end);
        UT_ASSERT(bitsy._pos == buffer.begin());
        UT_ASSERT(bitsy._currentMask == 128);
        UT_ASSERT(bitsy._bytesRemaining == -1);
    }
}

void XBitsyTest::TestGetBits()
{
    vector<uint8_t> buffer(14, 0);
    buffer[0] = 7;
    buffer[1] = 0;
    buffer[2] = 255;
    buffer[3] = 47;
    buffer[4] = 255;
    buffer[5] = 0;
    buffer[6] = 0;
    buffer[7] = 0;
    buffer[8] = 0;
    buffer[9] = 255;
    buffer[10] = 255;
    buffer[11] = 255;
    buffer[12] = 255;
    buffer[13] = 5;

    {
        XBitsy<const uint8_t*> bitsy(&buffer.front(), &buffer.front() + buffer.size());
        UT_ASSERT(bitsy.GetBitsRemaining() == 112);
        UT_ASSERT(bitsy.GetBits(8) == 7);
        UT_ASSERT(bitsy.GetBitsRemaining() == 104);
        UT_ASSERT(bitsy.GetBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 96);
        UT_ASSERT(bitsy.GetBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 88);
        UT_ASSERT(bitsy.GetBits(8) == 47);
        UT_ASSERT(bitsy.GetBitsRemaining() == 80);
        UT_ASSERT(bitsy.GetBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 72);
        UT_ASSERT(bitsy.GetBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 64);
        UT_ASSERT(bitsy.GetBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 56);
        UT_ASSERT(bitsy.GetBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 48);
        UT_ASSERT(bitsy.GetBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 40);
        UT_ASSERT(bitsy.GetBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 32);
        UT_ASSERT(bitsy.GetBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 24);
        UT_ASSERT(bitsy.GetBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 16);
        UT_ASSERT(bitsy.GetBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 8);
        UT_ASSERT(bitsy.GetBits(8) == 5);
        UT_ASSERT(bitsy.GetBitsRemaining() == 0);
    }

    {
        XBitsy<vector<uint8_t>::const_iterator> bitsy(buffer.begin(), buffer.end());
        UT_ASSERT(bitsy.GetBitsRemaining() == 112);
        UT_ASSERT(bitsy.GetBits(8) == 7);
        UT_ASSERT(bitsy.GetBitsRemaining() == 104);
        UT_ASSERT(bitsy.GetBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 96);
        UT_ASSERT(bitsy.GetBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 88);
        UT_ASSERT(bitsy.GetBits(8) == 47);
        UT_ASSERT(bitsy.GetBitsRemaining() == 80);
        UT_ASSERT(bitsy.GetBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 72);
        UT_ASSERT(bitsy.GetBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 64);
        UT_ASSERT(bitsy.GetBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 56);
        UT_ASSERT(bitsy.GetBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 48);
        UT_ASSERT(bitsy.GetBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 40);
        UT_ASSERT(bitsy.GetBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 32);
        UT_ASSERT(bitsy.GetBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 24);
        UT_ASSERT(bitsy.GetBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 16);
        UT_ASSERT(bitsy.GetBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 8);
        UT_ASSERT(bitsy.GetBits(8) == 5);
        UT_ASSERT(bitsy.GetBitsRemaining() == 0);
    }

    {
        XBitsy<const uint8_t*> bitsy(&buffer.front(), &buffer.front() + buffer.size());
        UT_ASSERT(bitsy.GetBitsRemaining() == 112);
        UT_ASSERT(bitsy.GetBits(4) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 108);
        UT_ASSERT(bitsy.GetBits(5) == 14);
        UT_ASSERT(bitsy.GetBitsRemaining() == 103);
        UT_ASSERT(bitsy.GetBits(18) == 2041);
        UT_ASSERT(bitsy.GetBitsRemaining() == 85);
        UT_ASSERT(bitsy.GetBits(0) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 85);
        UT_ASSERT(bitsy.GetBits(32) == 2146959360);
        UT_ASSERT(bitsy.GetBitsRemaining() == 53);
        UT_ASSERT(bitsy.GetBits(13) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 40);
        UT_ASSERT(bitsy.GetBits(32) == (uint32_t)-1);
        UT_ASSERT(bitsy.GetBitsRemaining() == 8);
        UT_ASSERT(bitsy.GetBits(3) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 5);
        UT_ASSERT(bitsy.GetBits(8) == 5);
        UT_ASSERT(bitsy.GetBitsRemaining() == 0);
        UT_ASSERT(bitsy.GetBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 0);
        UT_ASSERT(bitsy.GetBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 0);
    }

    {
        XBitsy<vector<uint8_t>::const_iterator> bitsy(buffer.begin(), buffer.end());
        UT_ASSERT(bitsy.GetBitsRemaining() == 112);
        UT_ASSERT(bitsy.GetBits(4) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 108);
        UT_ASSERT(bitsy.GetBits(5) == 14);
        UT_ASSERT(bitsy.GetBitsRemaining() == 103);
        UT_ASSERT(bitsy.GetBits(18) == 2041);
        UT_ASSERT(bitsy.GetBitsRemaining() == 85);
        UT_ASSERT(bitsy.GetBits(0) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 85);
        UT_ASSERT(bitsy.GetBits(32) == 2146959360);
        UT_ASSERT(bitsy.GetBitsRemaining() == 53);
        UT_ASSERT(bitsy.GetBits(13) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 40);
        UT_ASSERT(bitsy.GetBits(32) == (uint32_t)-1);
        UT_ASSERT(bitsy.GetBitsRemaining() == 8);
        UT_ASSERT(bitsy.GetBits(3) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 5);
        UT_ASSERT(bitsy.GetBits(8) == 5);
        UT_ASSERT(bitsy.GetBitsRemaining() == 0);
        UT_ASSERT(bitsy.GetBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 0);
        UT_ASSERT(bitsy.GetBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 0);
    }
}

void XBitsyTest::TestGetExactBits()
{
    vector<uint8_t> buffer(14, 0);
    buffer[0] = 7;
    buffer[1] = 0;
    buffer[2] = 255;
    buffer[3] = 47;
    buffer[4] = 255;
    buffer[5] = 0;
    buffer[6] = 0;
    buffer[7] = 0;
    buffer[8] = 0;
    buffer[9] = 255;
    buffer[10] = 255;
    buffer[11] = 255;
    buffer[12] = 255;
    buffer[13] = 5;

    {
        XBitsy<const uint8_t*> bitsy(&buffer.front(), &buffer.front() + buffer.size());
        UT_ASSERT(bitsy.GetBitsRemaining() == 112);
        UT_ASSERT(bitsy.GetExactBits(8) == 7);
        UT_ASSERT(bitsy.GetBitsRemaining() == 104);
        UT_ASSERT(bitsy.GetExactBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 96);
        UT_ASSERT(bitsy.GetExactBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 88);
        UT_ASSERT(bitsy.GetExactBits(8) == 47);
        UT_ASSERT(bitsy.GetBitsRemaining() == 80);
        UT_ASSERT(bitsy.GetExactBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 72);
        UT_ASSERT(bitsy.GetExactBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 64);
        UT_ASSERT(bitsy.GetExactBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 56);
        UT_ASSERT(bitsy.GetExactBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 48);
        UT_ASSERT(bitsy.GetExactBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 40);
        UT_ASSERT(bitsy.GetExactBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 32);
        UT_ASSERT(bitsy.GetExactBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 24);
        UT_ASSERT(bitsy.GetExactBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 16);
        UT_ASSERT(bitsy.GetExactBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 8);
        UT_ASSERT(bitsy.GetExactBits(8) == 5);
        UT_ASSERT(bitsy.GetBitsRemaining() == 0);
    }

    {
        XBitsy<vector<uint8_t>::const_iterator> bitsy(buffer.begin(), buffer.end());
        UT_ASSERT(bitsy.GetBitsRemaining() == 112);
        UT_ASSERT(bitsy.GetExactBits(8) == 7);
        UT_ASSERT(bitsy.GetBitsRemaining() == 104);
        UT_ASSERT(bitsy.GetExactBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 96);
        UT_ASSERT(bitsy.GetExactBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 88);
        UT_ASSERT(bitsy.GetExactBits(8) == 47);
        UT_ASSERT(bitsy.GetBitsRemaining() == 80);
        UT_ASSERT(bitsy.GetExactBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 72);
        UT_ASSERT(bitsy.GetExactBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 64);
        UT_ASSERT(bitsy.GetExactBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 56);
        UT_ASSERT(bitsy.GetExactBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 48);
        UT_ASSERT(bitsy.GetExactBits(8) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 40);
        UT_ASSERT(bitsy.GetExactBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 32);
        UT_ASSERT(bitsy.GetExactBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 24);
        UT_ASSERT(bitsy.GetExactBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 16);
        UT_ASSERT(bitsy.GetExactBits(8) == 255);
        UT_ASSERT(bitsy.GetBitsRemaining() == 8);
        UT_ASSERT(bitsy.GetExactBits(8) == 5);
        UT_ASSERT(bitsy.GetBitsRemaining() == 0);
    }

    {
        XBitsy<const uint8_t*> bitsy(&buffer.front(), &buffer.front() + buffer.size());
        UT_ASSERT(bitsy.GetBitsRemaining() == 112);
        UT_ASSERT(bitsy.GetExactBits(4) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 108);
        UT_ASSERT(bitsy.GetExactBits(5) == 14);
        UT_ASSERT(bitsy.GetBitsRemaining() == 103);
        UT_ASSERT(bitsy.GetExactBits(18) == 2041);
        UT_ASSERT(bitsy.GetBitsRemaining() == 85);
        UT_ASSERT(bitsy.GetBitsRemaining() == 85);
        UT_ASSERT(bitsy.GetExactBits(32) == 2146959360);
        UT_ASSERT(bitsy.GetBitsRemaining() == 53);
        UT_ASSERT(bitsy.GetExactBits(13) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 40);
        UT_ASSERT(bitsy.GetExactBits(32) == (uint32_t)-1);
        UT_ASSERT(bitsy.GetBitsRemaining() == 8);
        UT_ASSERT(bitsy.GetExactBits(5) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 3);
        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", bitsy.GetExactBits(4), XSDK::XException);
        UT_ASSERT(bitsy.GetBitsRemaining() == 3);
        UT_ASSERT(bitsy.GetExactBits(3) == 5);
        UT_ASSERT(bitsy.GetBitsRemaining() == 0);
        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", bitsy.GetExactBits(8), XSDK::XException);
        UT_ASSERT(bitsy.GetBitsRemaining() == 0);
    }

    {
        XBitsy<vector<uint8_t>::const_iterator> bitsy(buffer.begin(), buffer.end());
        UT_ASSERT(bitsy.GetBitsRemaining() == 112);
        UT_ASSERT(bitsy.GetExactBits(4) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 108);
        UT_ASSERT(bitsy.GetExactBits(5) == 14);
        UT_ASSERT(bitsy.GetBitsRemaining() == 103);
        UT_ASSERT(bitsy.GetExactBits(18) == 2041);
        UT_ASSERT(bitsy.GetBitsRemaining() == 85);
        UT_ASSERT(bitsy.GetBitsRemaining() == 85);
        UT_ASSERT(bitsy.GetExactBits(32) == 2146959360);
        UT_ASSERT(bitsy.GetBitsRemaining() == 53);
        UT_ASSERT(bitsy.GetExactBits(13) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 40);
        UT_ASSERT(bitsy.GetExactBits(32) == (uint32_t)-1);
        UT_ASSERT(bitsy.GetBitsRemaining() == 8);
        UT_ASSERT(bitsy.GetExactBits(5) == 0);
        UT_ASSERT(bitsy.GetBitsRemaining() == 3);
        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", bitsy.GetExactBits(4), XSDK::XException);
        UT_ASSERT(bitsy.GetBitsRemaining() == 3);
        UT_ASSERT(bitsy.GetExactBits(3) == 5);
        UT_ASSERT(bitsy.GetBitsRemaining() == 0);
        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", bitsy.GetExactBits(8), XSDK::XException);
        UT_ASSERT(bitsy.GetBitsRemaining() == 0);
    }
}

