
#include "XMemoryTest.h"
#include "XSDK/XMemory.h"
#include "XSDK/OS.h"
#include <cstring>

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XMemoryTest);


const static size_t MAX = 1073741824;//1GB
const static size_t HALF_MAX = MAX/2;

void XMemoryTest::setup()
{
}

void XMemoryTest::teardown()
{
}

void XMemoryTest::TestDefaultConstructor()
{
    const XMemory mem;

    UT_ASSERT( mem.GetBufferSize() == 1024 );
    UT_ASSERT( mem.GetDataSize() == 0 );
}
#ifndef ARM
void XMemoryTest::TestMPF_1023()
{
    {//This used to be an inifinite looop so this test just makes sure we will continue
        size_t value = (245320908);
        XMemory memory;
        UT_ASSERT_NO_THROW( memory.Extend(value));
    }
    {
        size_t value = HALF_MAX;
        XMemory memory;
        UT_ASSERT_NO_THROW( memory.Extend(value));
    }
    {
        size_t value = HALF_MAX+1;
        XMemory memory;
        UT_ASSERT_NO_THROW( memory.Extend(value));
    }
    {
        size_t value = HALF_MAX;
        XMemory memory;
        UT_ASSERT_NO_THROW( memory.Extend(value));
        UT_ASSERT_THROWS(memory.Extend(value+1),XException);
    }
}
#endif//ARM
void XMemoryTest::TestSizeConstructor()
{
    {
        const XMemory mem( 1024 );
        UT_ASSERT( mem.GetBufferSize() == 1024 );
        UT_ASSERT( mem.GetDataSize() == 0 );
    }
    {
        const XMemory mem( 4096 );
        UT_ASSERT( mem.GetBufferSize() == 4096 );
        UT_ASSERT( mem.GetDataSize() == 0 );
    }
}

void XMemoryTest::TestCopyConstructor()
{
    unsigned char data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    {

        XMemory memA;
        memcpy( &memA.Extend(10), data, 10 );

        XMemory memB = memA;
        UT_ASSERT( memcmp( memB.Map(), data, 10 ) == 0 );
    }

    ///MPF-948 test
    {
        XMemory memA;
        memcpy( &memA.Extend(10), data, 10 );

        UT_ASSERT( memA.GetDataSize() == sizeof(data) );

        XMemory memB(memA);

        UT_ASSERT( memB.GetDataSize() == sizeof(data) );
        UT_ASSERT( memcmp(memB.Map(),memA.Map(),sizeof(data)) == 0);
    }
}

void XMemoryTest::TestAssignmentOperator()
{
    unsigned char data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    {
        XMemory memA;
        memcpy( &memA.Extend(10), data, 10 );

        XMemory memB;
        memB = memA;
        UT_ASSERT( memcmp( memB.Map(), data, 10 ) == 0 );
    }

    ///MPF-948 test
    {
        XMemory memA;
        memcpy( &memA.Extend(10), data, 10 );

        UT_ASSERT( memA.GetDataSize() == sizeof(data) );

        XMemory memB;
        memB = memA;

        UT_ASSERT( memB.GetDataSize() == sizeof(data) );
        UT_ASSERT( memcmp(memB.Map(),memA.Map(),sizeof(data)) == 0);
    }
}

void XMemoryTest::TestExtend()
{
    {
        XIRef<XMemory> mem = new XMemory;
        UT_ASSERT( mem->GetBufferSize() == 1024 );
        UT_ASSERT( mem->GetDataSize() == 0 );

        char str[16] = "Hello World!";
        char* buf = (char*)&(mem->Extend( 16 ));
        strcpy( buf, str );

        UT_ASSERT( mem->Index(0) == 'H' );
        UT_ASSERT( mem->Index(1) == 'e' );
        UT_ASSERT( mem->Index(2) == 'l' );
        UT_ASSERT( mem->Index(3) == 'l' );
        UT_ASSERT( mem->Index(4) == 'o' );
        UT_ASSERT( mem->Index(5) == ' ' );
        UT_ASSERT( mem->Index(6) == 'W' );
        UT_ASSERT( mem->Index(7) == 'o' );
        UT_ASSERT( mem->Index(8) == 'r' );
        UT_ASSERT( mem->Index(9) == 'l' );
        UT_ASSERT( mem->Index(10) == 'd' );
        UT_ASSERT( mem->Index(11) == '!' );
        UT_ASSERT( mem->Index(12) == '\0' );
    }
}

void XMemoryTest::TestMap()
{
    {
        XIRef<XMemory> mem = new XMemory;
        UT_ASSERT( mem->GetBufferSize() == 1024 );
        UT_ASSERT( mem->GetDataSize() == 0 );

        char str[16] = "Hello World!";
        char* buf = (char*)&(mem->Extend( 16 ));
        strcpy( buf, str );

        const unsigned char* data = mem->Map();

        UT_ASSERT( data[0] == 'H' );
        UT_ASSERT( data[1] == 'e' );
        UT_ASSERT( data[2] == 'l' );
        UT_ASSERT( data[3] == 'l' );
        UT_ASSERT( data[4] == 'o' );
        UT_ASSERT( data[5] == ' ' );
        UT_ASSERT( data[6] == 'W' );
        UT_ASSERT( data[7] == 'o' );
        UT_ASSERT( data[8] == 'r' );
        UT_ASSERT( data[9] == 'l' );
        UT_ASSERT( data[10] == 'd' );
        UT_ASSERT( data[11] == '!' );
        UT_ASSERT( data[12] == '\0' );
    }
}

void XMemoryTest::TestClone()
{
    {
        XIRef<XMemory> mem = new XMemory;
        UT_ASSERT( mem->GetBufferSize() == 1024 );
        UT_ASSERT( mem->GetDataSize() == 0 );

        char str[16] = "Hello World!";
        char* buf = (char*)&(mem->Extend( 16 ));
        strcpy( buf, str );

        const unsigned char* data = mem->Map();

        UT_ASSERT( data[0] == 'H' );
        UT_ASSERT( data[1] == 'e' );
        UT_ASSERT( data[2] == 'l' );
        UT_ASSERT( data[3] == 'l' );
        UT_ASSERT( data[4] == 'o' );
        UT_ASSERT( data[5] == ' ' );
        UT_ASSERT( data[6] == 'W' );
        UT_ASSERT( data[7] == 'o' );
        UT_ASSERT( data[8] == 'r' );
        UT_ASSERT( data[9] == 'l' );
        UT_ASSERT( data[10] == 'd' );
        UT_ASSERT( data[11] == '!' );
        UT_ASSERT( data[12] == '\0' );

        XIRef<XMemory> mem2 = mem->Clone();

        const unsigned char* data2 = mem2->Map();

        UT_ASSERT( data2[0] == 'H' );
        UT_ASSERT( data2[1] == 'e' );
        UT_ASSERT( data2[2] == 'l' );
        UT_ASSERT( data2[3] == 'l' );
        UT_ASSERT( data2[4] == 'o' );
        UT_ASSERT( data2[5] == ' ' );
        UT_ASSERT( data2[6] == 'W' );
        UT_ASSERT( data2[7] == 'o' );
        UT_ASSERT( data2[8] == 'r' );
        UT_ASSERT( data2[9] == 'l' );
        UT_ASSERT( data2[10] == 'd' );
        UT_ASSERT( data2[11] == '!' );
        UT_ASSERT( data2[12] == '\0' );
    }
}

void XMemoryTest::TestResizeBuffer()
{
    XIRef<XMemory> m = new XMemory( 4096 );

    m->Extend( 8192 );

    UT_ASSERT( m->GetDataSize() == 8192 );

    m->Clear();

    UT_ASSERT( m->GetDataSize() == 0 );
}

void XMemoryTest::TestResizeData()
{
    XIRef<XMemory> mem = new XMemory(72);
    UT_ASSERT(mem->GetDataSize() == 0);
    UT_ASSERT(mem->GetBufferSize() == 72);
    mem->ResizeData(42);
    UT_ASSERT(mem->GetDataSize() == 42);
    UT_ASSERT(mem->GetBufferSize() == 72);
    mem->ResizeData(12);
    UT_ASSERT(mem->GetDataSize() == 12);
    UT_ASSERT(mem->GetBufferSize() == 72);
    mem->ResizeData(80);
    UT_ASSERT(mem->GetDataSize() == 80);
    UT_ASSERT(mem->GetBufferSize() == 80);
}

void XMemoryTest::TestRead()
{
    XIRef<XMemory> mem = new XMemory;
    UT_ASSERT(mem->GetDataSize() == 0);

    mem->Extend(19) = 146;
    mem->Index(1) = 120;
    mem->Index(2) = 42;
    mem->Index(3) = 0;
    mem->Index(4) = 1;
    mem->Index(5) = 128;
    mem->Index(6) = 223;
    mem->Index(7) = 67;
    mem->Index(8) = 223;
    mem->Index(9) = 41;
    mem->Index(10) = 110;
    mem->Index(11) = 245;
    mem->Index(12) = 78;
    mem->Index(13) = 0;
    mem->Index(14) = 110;
    mem->Index(15) = 135;
    mem->Index(16)  = 213;
    mem->Index(17) = 255;
    mem->Index(18) = 17;

    UT_ASSERT_EQUAL((uint32_t)2783378, mem->Read<uint32_t>(0));
    UT_ASSERT_EQUAL((int16_t)-32767, mem->Read<int16_t>(4));
    UT_ASSERT_EQUAL((uint64_t)22224901681136607LL, mem->Read<uint64_t>(6));
    UT_ASSERT_EQUAL((int32_t)-2783378, mem->Read<int32_t>(14));
    UT_ASSERT_EQUAL((int8_t)17, mem->Read<int8_t>(18));

    size_t index = 0;
    UT_ASSERT_EQUAL((uint32_t)2783378, mem->Read<uint32_t>(index, &index));
    UT_ASSERT_EQUAL((int16_t)-32767, mem->Read<int16_t>(index, &index));
    UT_ASSERT_EQUAL((uint64_t)22224901681136607LL, mem->Read<uint64_t>(index, &index));
    UT_ASSERT_EQUAL((int32_t)-2783378, mem->Read<int32_t>(index, &index));
    UT_ASSERT_EQUAL((int8_t)17, mem->Read<int8_t>(index, &index));
    UT_ASSERT_EQUAL((size_t)19, index);
}

void XMemoryTest::TestWrite()
{
    XIRef<XMemory> mem = new XMemory;
    UT_ASSERT(mem->GetDataSize() == 0);
    mem->Extend(19) = 255;

    UT_ASSERT_EQUAL((size_t)1, mem->Write<uint8_t>(0, 42));
    UT_ASSERT_EQUAL((size_t)3, mem->Write<uint16_t>(1, 517));
    UT_ASSERT_EQUAL((size_t)7, mem->Write<uint32_t>(3, 306783378));
    UT_ASSERT_EQUAL((size_t)11, mem->Write<int32_t>(7, 306783378));
    UT_ASSERT_EQUAL((size_t)19, mem->Write<int64_t>(11, 709490156681136600LL));

    UT_ASSERT_EQUAL((uint8_t)42, mem->Index(0));
    UT_ASSERT_EQUAL((uint8_t)5, mem->Index(1));
    UT_ASSERT_EQUAL((uint8_t)2, mem->Index(2));
    UT_ASSERT_EQUAL((uint8_t)146, mem->Index(3));
    UT_ASSERT_EQUAL((uint8_t)36, mem->Index(4));
    UT_ASSERT_EQUAL((uint8_t)73, mem->Index(5));
    UT_ASSERT_EQUAL((uint8_t)18, mem->Index(6));
    UT_ASSERT_EQUAL((uint8_t)146, mem->Index(7));
    UT_ASSERT_EQUAL((uint8_t)36, mem->Index(8));
    UT_ASSERT_EQUAL((uint8_t)73, mem->Index(9));
    UT_ASSERT_EQUAL((uint8_t)18, mem->Index(10));
    UT_ASSERT_EQUAL((uint8_t)216, mem->Index(11));
    UT_ASSERT_EQUAL((uint8_t)137, mem->Index(12));
    UT_ASSERT_EQUAL((uint8_t)157, mem->Index(13));
    UT_ASSERT_EQUAL((uint8_t)216, mem->Index(14));
    UT_ASSERT_EQUAL((uint8_t)137, mem->Index(15));
    UT_ASSERT_EQUAL((uint8_t)157, mem->Index(16));
    UT_ASSERT_EQUAL((uint8_t)216, mem->Index(17));
    UT_ASSERT_EQUAL((uint8_t)9, mem->Index(18));
}

void XMemoryTest::TestAppend()
{
    XIRef<XMemory> mem = new XMemory;
    UT_ASSERT(mem->GetDataSize() == 0);

    UT_ASSERT_EQUAL((size_t)4, mem->Append<uint32_t>(2783378));
    UT_ASSERT_EQUAL((size_t)4, mem->GetDataSize());

    UT_ASSERT_EQUAL((size_t)6, mem->Append<int16_t>(-32767));
    UT_ASSERT_EQUAL((size_t)6, mem->GetDataSize());

    UT_ASSERT_EQUAL((size_t)14, mem->Append<uint64_t>(22224901681136607LL));
    UT_ASSERT_EQUAL((size_t)14, mem->GetDataSize());

    UT_ASSERT_EQUAL((size_t)18, mem->Append<int32_t>(-2783378));
    UT_ASSERT_EQUAL((size_t)18, mem->GetDataSize());

    UT_ASSERT_EQUAL((size_t)19, mem->Append<int8_t>(17));
    UT_ASSERT_EQUAL((size_t)19, mem->GetDataSize());

    UT_ASSERT_EQUAL((uint8_t)146, mem->Index(0));
    UT_ASSERT_EQUAL((uint8_t)120, mem->Index(1));
    UT_ASSERT_EQUAL((uint8_t)42, mem->Index(2));
    UT_ASSERT_EQUAL((uint8_t)0, mem->Index(3));
    UT_ASSERT_EQUAL((uint8_t)1, mem->Index(4));
    UT_ASSERT_EQUAL((uint8_t)128, mem->Index(5));
    UT_ASSERT_EQUAL((uint8_t)223, mem->Index(6));
    UT_ASSERT_EQUAL((uint8_t)67, mem->Index(7));
    UT_ASSERT_EQUAL((uint8_t)223, mem->Index(8));
    UT_ASSERT_EQUAL((uint8_t)41, mem->Index(9));
    UT_ASSERT_EQUAL((uint8_t)110, mem->Index(10));
    UT_ASSERT_EQUAL((uint8_t)245, mem->Index(11));
    UT_ASSERT_EQUAL((uint8_t)78, mem->Index(12));
    UT_ASSERT_EQUAL((uint8_t)0, mem->Index(13));
    UT_ASSERT_EQUAL((uint8_t)110, mem->Index(14));
    UT_ASSERT_EQUAL((uint8_t)135, mem->Index(15));
    UT_ASSERT_EQUAL((uint8_t)213, mem->Index(16));
    UT_ASSERT_EQUAL((uint8_t)255, mem->Index(17));
    UT_ASSERT_EQUAL((uint8_t)17, mem->Index(18));
}

void XMemoryTest::TestPoolConstructor()
{
    XIRef<XMemoryPool> memoryPool = new XMemoryPool( 1500, 30 );

    UT_ASSERT( memoryPool->GetNumFreeBuffers() == 30 );
}

void XMemoryTest::TestPoolGetAndReturn()
{
    XIRef<XMemoryPool> memoryPool = new XMemoryPool( 1500, 30 );

    UT_ASSERT( memoryPool->GetNumFreeBuffers() == 30 );

    XIRef<XMemory> buffer = memoryPool->GetBuffer();

    UT_ASSERT( memoryPool->GetNumFreeBuffers() == 29 );

    memoryPool->ReturnBuffer( buffer );

    UT_ASSERT( memoryPool->GetNumFreeBuffers() == 30 );
}
