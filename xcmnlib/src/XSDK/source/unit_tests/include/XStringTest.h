//=============================================================================
//Copyright (c) 2013 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XStringTest.h $
// $Revision:$
// $Date:$
// $Author: Jonathan M Davis $
//=============================================================================

#ifndef _XStringTest_h_
#define _XStringTest_h_

#include "framework.h"

class XStringTest : public test_fixture
{
public:
    TEST_SUITE(XStringTest);
        TEST(XStringTest::TestIsSpace);
        TEST(XStringTest::TestConstructor);
        TEST(XStringTest::TestCopyConstructor);
        TEST(XStringTest::TestConversionOperator);
        TEST(XStringTest::TestCString);
        TEST(XStringTest::TestAssignmentOperator);
        TEST(XStringTest::TestEqualityOperator);
        TEST(XStringTest::TestInequalityOperator);
        TEST(XStringTest::TestEndsWith);
        TEST(XStringTest::TestStartsWith);
        TEST(XStringTest::TestContains);
        TEST(XStringTest::TestIsInteger);
        TEST(XStringTest::TestIsDouble);
        TEST(XStringTest::TestLStripInPlace);
        TEST(XStringTest::TestRStripInPlace);
        TEST(XStringTest::TestStripInPlace);
        TEST(XStringTest::TestLStrip);
        TEST(XStringTest::TestRStrip);
        TEST(XStringTest::TestStrip);
        TEST(XStringTest::TestStripEOL);
        TEST(XStringTest::TestFormat);
        TEST(XStringTest::TestOperatorPlus);
        TEST(XStringTest::TestPlusEquals);
        TEST(XStringTest::TestSubString);
        TEST(XStringTest::TestSplit);
        TEST(XStringTest::TestReplaceAll);
        TEST(XStringTest::TestEraseAll);
        TEST(XStringTest::TestFromInt);
        TEST(XStringTest::TestFromUInt);
        TEST(XStringTest::TestFromInt8);
        TEST(XStringTest::TestFromUInt8);
        TEST(XStringTest::TestFromInt16);
        TEST(XStringTest::TestFromUInt16);
        TEST(XStringTest::TestFromInt32);
        TEST(XStringTest::TestFromUInt32);
        TEST(XStringTest::TestFromInt64);
        TEST(XStringTest::TestFromUInt64);
        TEST(XStringTest::TestFromDouble);
        TEST(XStringTest::TestToInt);
        TEST(XStringTest::TestToUInt);
        TEST(XStringTest::TestToInt8);
        TEST(XStringTest::TestToUInt8);
        TEST(XStringTest::TestToInt16);
        TEST(XStringTest::TestToUInt16);
        TEST(XStringTest::TestToInt32);
        TEST(XStringTest::TestToUInt32);
        TEST(XStringTest::TestToInt64);
        TEST(XStringTest::TestToUInt64);
        TEST(XStringTest::TestToDouble);
        TEST(XStringTest::TestToLower);
        TEST(XStringTest::TestToUpper);
        TEST(XStringTest::TestBase64Encode);
        TEST(XStringTest::TestBase64Decode);
        TEST(XStringTest::TestFindFirst);
        TEST(XStringTest::TestFindLast);
        TEST(XStringTest::TestURIEncode);
        TEST(XStringTest::TestURIDecode);
        TEST(XStringTest::TestConstructorUnicode);
        TEST(XStringTest::TestCopyConstructorUnicode);
        TEST(XStringTest::TestWCString);
        TEST(XStringTest::TestAssignmentOperatorUnicode);
        TEST(XStringTest::TestEndsWithUnicodeVersion);
        TEST(XStringTest::TestStartsWithUnicodeVersion);
        TEST(XStringTest::TestContainsUnicodeVersion);
        TEST(XStringTest::TestOperatorPlusUnicodeVersion);
        TEST(XStringTest::TestOperatorPlusEqualsUnicodeVersion);
        TEST(XStringTest::TestEraseAllUnicode);
        TEST(XStringTest::TestReplaceAllUnicode);
        TEST(XStringTest::TestSplitUnicode);
        TEST(XStringTest::TestFindFirstUnicode);
        TEST(XStringTest::TestFindLastUnicode);
        TEST(XStringTest::TestConversions);
    TEST_SUITE_END();

public:

    virtual ~XStringTest() throw()
    {}

    void setup();
    void teardown();

protected:
    void TestIsSpace();
    void TestConstructor();
    void TestCopyConstructor();
    void TestConversionOperator();
    void TestCString();
    void TestAssignmentOperator();
    void TestEqualityOperator();
    void TestInequalityOperator();
    void TestEndsWith();
    void TestStartsWith();
    void TestContains();
    void TestIsInteger();
    void TestIsDouble();
    void TestLStripInPlace();
    void TestRStripInPlace();
    void TestStripInPlace();
    void TestLStrip();
    void TestRStrip();
    void TestStrip();
    void TestStripEOL();
    void TestFormat();
    void TestOperatorPlus();
    void TestPlusEquals();
    void TestSubString();
    void TestSplit();
    void TestReplaceAll();
    void TestEraseAll();
    void TestFromInt();
    void TestFromUInt();
    void TestFromInt32();
    void TestFromUInt32();
    void TestFromInt64();
    void TestFromUInt64();
    void TestFromDouble();
    void TestToInt();
    void TestToUInt();

    void TestToInt8();
    void TestToUInt8();
    void TestToInt16();
    void TestToUInt16();

    void TestFromInt8();
    void TestFromUInt8();
    void TestFromInt16();
    void TestFromUInt16();
    void TestToInt32();
    void TestToUInt32();
    void TestToInt64();
    void TestToUInt64();
    void TestToDouble();
    void TestToLower();
    void TestToUpper();
    void TestBase64Encode();
    void TestBase64Decode();
    void TestFindFirst();
    void TestFindLast();
    // URL encode & decode tests
    void TestURIEncode();
    void TestURIDecode();
    //Unicode Specific Tests
    void TestConstructorUnicode();
    void TestCopyConstructorUnicode();
    void TestWCString();
    void TestAssignmentOperatorUnicode();
    void TestEndsWithUnicodeVersion();
    void TestStartsWithUnicodeVersion();
    void TestContainsUnicodeVersion();
    void TestOperatorPlusUnicodeVersion();
    void TestOperatorPlusEqualsUnicodeVersion();
    void TestEraseAllUnicode();
    void TestReplaceAllUnicode();
    void TestSplitUnicode();
    void TestFindFirstUnicode();
    void TestFindLastUnicode();
    void TestConversions();
};

#endif

