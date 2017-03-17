
#include "RBSPIterTest.h"
#include "MediaParser/RBSPIter.h"
#include <vector>

using namespace std;
using namespace MEDIA_PARSER;
using namespace XSDK;

CPPUNIT_TEST_SUITE_REGISTRATION(RBSPIterTest);


void RBSPIterTest::TestComparison()
{
    printf("RBSPIterTest::TestComparison()\n");
    fflush(stdout);

    vector<uint8_t> vec;
    vec.push_back(0);
    vec.push_back(1);
    vec.push_back(0);

    RBSPIter one(&vec.front(), &vec.back() + 1);
    RBSPIter two(&vec.front(), &vec.back() + 1);

    CPPUNIT_ASSERT(one == two);
    CPPUNIT_ASSERT(!(one != two));
    CPPUNIT_ASSERT(!(one < two));
    CPPUNIT_ASSERT(one <= two);
    CPPUNIT_ASSERT(one >= two);
    CPPUNIT_ASSERT(!(one > two));

    RBSPIter three(&vec.front() + 1, &vec.back() + 1);

    CPPUNIT_ASSERT(!(one == three));
    CPPUNIT_ASSERT(one != three);
    CPPUNIT_ASSERT(one < three);
    CPPUNIT_ASSERT(one <= three);
    CPPUNIT_ASSERT(!(one >= three));
    CPPUNIT_ASSERT(!(one > three));

    CPPUNIT_ASSERT(!(three == one));
    CPPUNIT_ASSERT(three != one);
    CPPUNIT_ASSERT(!(three < one));
    CPPUNIT_ASSERT(!(three <= one));
    CPPUNIT_ASSERT(three >= one);
    CPPUNIT_ASSERT(three > one);

    one = three;

    CPPUNIT_ASSERT(!(two == one));
    CPPUNIT_ASSERT(two != one);
    CPPUNIT_ASSERT(two < one);
    CPPUNIT_ASSERT(two <= one);
    CPPUNIT_ASSERT(!(two >= one));
    CPPUNIT_ASSERT(!(two > one));

    CPPUNIT_ASSERT(!(one == two));
    CPPUNIT_ASSERT(one != two);
    CPPUNIT_ASSERT(!(one < two));
    CPPUNIT_ASSERT(!(one <= two));
    CPPUNIT_ASSERT(one >= two);
    CPPUNIT_ASSERT(one > two);

    CPPUNIT_ASSERT(one == three);
    CPPUNIT_ASSERT(!(one != three));
    CPPUNIT_ASSERT(!(one < three));
    CPPUNIT_ASSERT(one <= three);
    CPPUNIT_ASSERT(one >= three);
    CPPUNIT_ASSERT(!(one > three));
}


void RBSPIterTest::TestIteration()
{
    printf("RBSPIterTest::TestIteration()\n");
    fflush(stdout);

    for(uint8_t i = 0; i < 4; ++i)
    {
        const uint8_t bytes[] = { 0, 0, 3, i };
        const size_t length = sizeof(bytes);
        RBSPIter iter(bytes, bytes + length);
        RBSPIter end(bytes + length);

        CPPUNIT_ASSERT(*iter == 0);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 0);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == i);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(iter == end);
    }

    for(uint8_t i = 4; i < 10; ++i)
    {
        const uint8_t bytes[] = { 0, 0, 3, i };
        const size_t length = sizeof(bytes);
        RBSPIter iter(bytes, bytes + length);

        CPPUNIT_ASSERT(*iter == 0);
        ++iter;
        CPPUNIT_ASSERT(*iter == 0);
        CPPUNIT_ASSERT_THROW(++iter, XException);
    }

    for(uint8_t i = 4; i < 10; ++i)
    {
        const uint8_t bytes[] = { 0, 0, i };
        const size_t length = sizeof(bytes);
        RBSPIter iter(bytes, bytes + length);
        RBSPIter end(bytes + length);

        CPPUNIT_ASSERT(*iter == 0);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 0);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == i);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(iter == end);
    }

    for(uint8_t i = 0; i < 4; ++i)
    {
        const uint8_t bytes[] = { 2, 5, 255, 0, 0, 3, i, 27, 9, 0, 0, 3, 2 };
        const size_t length = sizeof(bytes);
        RBSPIter iter(bytes, bytes + length);
        RBSPIter end(bytes + length);

        CPPUNIT_ASSERT(*iter == 2);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 5);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 255);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 0);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 0);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == i);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 27);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 9);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 0);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 0);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 2);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(iter == end);
    }

    {
        const uint8_t bytes[] = { 0, 0, 3, 0, 0, 3, 0, 0, 3, 1 };
        const size_t length = sizeof(bytes);
        RBSPIter iter(bytes, bytes + length);
        RBSPIter end(bytes + length);

        CPPUNIT_ASSERT(*iter == 0);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 0);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 0);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 0);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 0);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 0);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(*iter == 1);
        CPPUNIT_ASSERT(iter != end);
        ++iter;
        CPPUNIT_ASSERT(iter == end);
    }
}


void RBSPIterTest::TestGetRBSPFromNAL()
{
    printf("RBSPIterTest::TestGetRBSPFromNAL()\n");
    fflush(stdout);

    {
        const uint8_t bytes[] = { 0, 0, 1 };
        const size_t length = sizeof(bytes);
        RBSPIter rbspEnd;
        RBSPIter result = GetRBSPFromNAL(bytes, length, &rbspEnd);
        CPPUNIT_ASSERT(rbspEnd._iter == bytes);
        CPPUNIT_ASSERT(result._iter == bytes);
    }

    {
        const uint8_t bytes[] = { 1, 2, 3, 0, 0, 1, 4, 5, 6 };
        const size_t length = sizeof(bytes);
        RBSPIter rbspEnd;
        RBSPIter result = GetRBSPFromNAL(bytes, length, &rbspEnd);
        CPPUNIT_ASSERT(rbspEnd._iter == bytes + 3);
        CPPUNIT_ASSERT(result._iter == bytes);
    }

    {
        const uint8_t bytes[] = { 1, 2, 3, 0, 0, 3, 0, 0, 1, 4, 5, 6 };
        const size_t length = sizeof(bytes);
        RBSPIter rbspEnd;
        RBSPIter result = GetRBSPFromNAL(bytes, length, &rbspEnd);
        CPPUNIT_ASSERT(rbspEnd._iter == bytes + 6);
        CPPUNIT_ASSERT(result._iter == bytes);
    }

    {
        const uint8_t bytes[] = { 0, 1 };
        const size_t length = sizeof(bytes);
        RBSPIter rbspEnd;
        RBSPIter result = GetRBSPFromNAL(bytes, length, &rbspEnd);
        CPPUNIT_ASSERT(rbspEnd._iter == bytes + length);
        CPPUNIT_ASSERT(result._iter == bytes);
    }

    {
        const uint8_t bytes[] = { 0, 0, 0, 3, 2, 1, 0, 0, 0 };
        const size_t length = sizeof(bytes);
        RBSPIter rbspEnd;
        RBSPIter result = GetRBSPFromNAL(bytes, length, &rbspEnd);
        CPPUNIT_ASSERT(rbspEnd._iter == bytes + length);
        CPPUNIT_ASSERT(result._iter == bytes);
    }
}
