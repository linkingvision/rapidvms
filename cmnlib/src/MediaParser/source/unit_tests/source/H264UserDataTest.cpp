
#include "H264UserDataTest.h"
#include "MediaParser/H264UserData.h"

#include <algorithm>

using namespace std;
using namespace MEDIA_PARSER;
using namespace XSDK;

CPPUNIT_TEST_SUITE_REGISTRATION(H264UserDataTest);

void H264UserDataTest::TestEmptySEI()
{
    printf("H264UserDataTest::TestEmptySEI()\n");
    fflush(stdout);

    XRef<SEIPayload> payload = new SEIPayload(XIRef<XMemory>(new XMemory));
    CPPUNIT_ASSERT(payload->GetData()->empty());
    CPPUNIT_ASSERT(payload->GetUUID() == XUuid("7e0858c4-38fe-48ea-852d-dace39badb30"));

    H264UserData before(payload);
    const std::vector<XRef<SEIPayload> >& beforePayloads = before.GetPayloads();
    CPPUNIT_ASSERT(beforePayloads.size() == 1);
    CPPUNIT_ASSERT(beforePayloads.front() == payload);
    XIRef<XMemory> sei = before.GenSEI();

    H264UserData after(sei->begin(), sei->GetDataSize());
    const std::vector<XRef<SEIPayload> >& afterPayloads = before.GetPayloads();
    CPPUNIT_ASSERT(afterPayloads.size() == 1);
    CPPUNIT_ASSERT(afterPayloads.front().Get() == payload.Get());
}


void H264UserDataTest::TestUUID()
{
    printf("H264UserDataTest::TestUUID()\n");
    fflush(stdout);

    XRef<SEIPayload> payload = new SEIPayload(XIRef<XMemory>(new XMemory), XUuid("00000000-1111-2222-3333-444444444444"));
    CPPUNIT_ASSERT(payload->GetData()->empty());
    CPPUNIT_ASSERT(payload->GetUUID() == XUuid("00000000-1111-2222-3333-444444444444"));
}


void H264UserDataTest::TestRawSEI()
{
    printf("H264UserDataTest::TestRawSEI()\n");
    fflush(stdout);

    XIRef<XMemory> data = new XMemory;

    for(uint8_t i = 1; i < 12; ++i)
        data->Append<uint8_t>(i);

    XRef<SEIPayload> payload = new SEIPayload(data);
    CPPUNIT_ASSERT(payload->GetData().Get() == data.Get());
    CPPUNIT_ASSERT(payload->GetUUID() == XUuid("7e0858c4-38fe-48ea-852d-dace39badb30"));

    H264UserData before(payload);
    const std::vector<XRef<SEIPayload> >& beforePayloads = before.GetPayloads();
    CPPUNIT_ASSERT(beforePayloads.size() == 1);
    CPPUNIT_ASSERT(beforePayloads.front().Get() == payload.Get());
    XIRef<XMemory> sei = before.GenSEI();

    H264UserData after(sei->begin(), sei->GetDataSize());
    const std::vector<XRef<SEIPayload> >& afterPayloads = before.GetPayloads();
    CPPUNIT_ASSERT(afterPayloads.size() == 1);
    CPPUNIT_ASSERT(afterPayloads.front().Get() == payload.Get());
}


void H264UserDataTest::TestZeroRawSEI()
{
    printf("H264UserDataTest::TestZeroRawSEI()\n");
    fflush(stdout);

    XIRef<XMemory> data = new XMemory;

    for(uint8_t i = 1; i < 77; ++i)
        data->Append<uint8_t>(0);

    XRef<SEIPayload> payload = new SEIPayload(data);
    CPPUNIT_ASSERT(payload->GetData().Get() == data.Get());
    CPPUNIT_ASSERT(payload->GetUUID() == XUuid("7e0858c4-38fe-48ea-852d-dace39badb30"));

    H264UserData before(payload);
    const std::vector<XRef<SEIPayload> >& beforePayloads = before.GetPayloads();
    CPPUNIT_ASSERT(beforePayloads.size() == 1);
    CPPUNIT_ASSERT(beforePayloads.front() == payload);
    XIRef<XMemory> sei = before.GenSEI();

    H264UserData after(sei->begin(), sei->GetDataSize());
    const std::vector<XRef<SEIPayload> >& afterPayloads = before.GetPayloads();
    CPPUNIT_ASSERT(afterPayloads.size() == 1);
    CPPUNIT_ASSERT(afterPayloads.front().Get() == payload.Get());
}


void H264UserDataTest::TestMultiplePayloads()
{
    printf("H264UserDataTest::TestMultiplePayloads()\n");
    fflush(stdout);

    XIRef<XMemory> data1 = new XMemory;

    for(uint8_t i = 1; i < 77; ++i)
        data1->Append<uint8_t>(0);

    XIRef<XMemory> data2 = new XMemory;

    for(uint8_t i = 219; i > 59; --i)
        data2->Append<uint8_t>(i);

    XRef<SEIPayload> payload1 = new SEIPayload(data1, XUuid("4ae62deb-d9a5-417e-8981-2379b0a756f6"));
    CPPUNIT_ASSERT(payload1->GetData().Get() == data1.Get());
    CPPUNIT_ASSERT(payload1->GetUUID() == XUuid("4ae62deb-d9a5-417e-8981-2379b0a756f6"));

    XRef<SEIPayload> payload2 = new SEIPayload(data2, XUuid("9fc80724-4208-40f7-8d13-b03bfa4140df"));
    CPPUNIT_ASSERT(payload2->GetData() == data2);
    CPPUNIT_ASSERT(payload2->GetUUID() == XUuid("9fc80724-4208-40f7-8d13-b03bfa4140df"));

    vector<XRef<SEIPayload> > payloads;
    payloads.push_back(payload2);
    payloads.push_back(payload1);
    payloads.push_back(payload1);
    payloads.push_back(payload2);

    H264UserData before(payloads);
    const std::vector<XRef<SEIPayload> >& beforePayloads = before.GetPayloads();
    CPPUNIT_ASSERT(beforePayloads == payloads);
    XIRef<XMemory> sei = before.GenSEI();

    H264UserData after(sei->begin(), sei->GetDataSize());
    const std::vector<XRef<SEIPayload> >& afterPayloads = before.GetPayloads();
    CPPUNIT_ASSERT(afterPayloads == payloads);
}


void H264UserDataTest::TestNalify()
{
    printf("H264UserDataTest::TestNalify()\n");
    fflush(stdout);

    XIRef<XMemory> data = new XMemory;
    data->Append<uint8_t>(0);
    data->Append<uint8_t>(0);
    data->Append<uint8_t>(3);
    data->Append<uint8_t>(0);
    data->Append<uint8_t>(0);
    data->Append<uint8_t>(0);
    data->Append<uint8_t>(3);
    data->Append<uint8_t>(0);
    data->Append<uint8_t>(0);
    data->Append<uint8_t>(0);
    data->Append<uint8_t>(0);

    const size_t expectedSize = 19;
    const uint8_t expected[expectedSize] = { 0, 0, 1, 0x06, 0, 0, 3, 3, 0, 0, 3, 0, 3, 0, 0, 3, 0, 0, 3 };
    XIRef<XMemory> nalified = H264UserData::_Nalify(data);
    CPPUNIT_ASSERT_EQUAL(nalified->GetDataSize(), expectedSize);
    CPPUNIT_ASSERT(memcmp(nalified->Map(), expected, nalified->GetDataSize()) == 0);

    XIRef<XMemory> denalified = H264UserData::_Denalify(nalified->Map(), nalified->GetDataSize());
    CPPUNIT_ASSERT_EQUAL(data->GetDataSize(), denalified->GetDataSize());
    CPPUNIT_ASSERT(memcmp(data->Map(), denalified->Map(), data->GetDataSize()) == 0);
}
