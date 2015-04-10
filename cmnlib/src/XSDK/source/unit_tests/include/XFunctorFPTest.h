#ifndef _FUNCTOR_FP_TEST_H_
#define _FUNCTOR_FP_TEST_H_

#include "framework.h"

#include "XSDK/XString.h"

class XFunctorFPTest : public test_fixture
{
public:
    TEST_SUITE(XFunctorFPTest);
        TEST(XFunctorFPTest::TestFunctorFP1);
        TEST(XFunctorFPTest::TestFunctorFP2);
        TEST(XFunctorFPTest::TestFunctorFP3);
        TEST(XFunctorFPTest::TestFunctorFP4);
        TEST(XFunctorFPTest::TestFunctorFP5);
    TEST_SUITE_END();

    void setup();
    void teardown();

protected:
    void TestFunctorFP1(void);
    void TestFunctorFP2(void);
    void TestFunctorFP3(void);
    void TestFunctorFP4(void);
    void TestFunctorFP5(void);
};

class TestClass
{
public:
    TestClass(){};
    int TestFunction1_1(int);
    double TestFunction1_2(double);

    unsigned TestFunction2_1(unsigned,double);
    double TestFunction2_2(unsigned,double);

    XSDK::XString TestFunction3_1(int,XSDK::XString,const char *);
    int TestFunction3_2(int,XSDK::XString,const char *);

    char TestFunction4_1(const char *,double,XSDK::XString,char);
    double TestFunction4_2(const char *,double,XSDK::XString,char);

    bool TestFunction5_1(int,double,int,bool,int);
    void TestFunction5_2(int,double,int,bool,int);
};

#endif
