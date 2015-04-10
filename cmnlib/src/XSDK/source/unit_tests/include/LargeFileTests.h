
#ifndef _LARGE_FILE_TEST_H_
#define _LARGE_FILE_TEST_H_

#include "framework.h"
#include "XSDK/OS.h"

class LargeFileTests: public test_fixture
{
public:
    //========================================================================
    // Test declarations
    //========================================================================
    TEST_SUITE(LargeFileTests);
        TEST(LargeFileTests::TestAsciiFile);
        TEST(LargeFileTests::TestUnicodeFile);
        TEST(LargeFileTests::TestSeekTell);
        TEST(LargeFileTests::TestPreAllocatedFile);
    TEST_SUITE_END();

    void setup();
    void teardown();

protected:
    void TestAsciiFile();
    void TestUnicodeFile();
    void TestSeekTell();
    void TestPreAllocatedFile();
};


#endif//_LARGE_FILE_TEST_H_
