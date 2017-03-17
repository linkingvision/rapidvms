#include "XSDK/XFunctorFP.h"
#include "XFunctorFPTest.h"

#include <list>

using namespace std;
using namespace XSDK;

#define INT -21
#define UNSIGNED 323
#define DOUBLE 31.441
#define STRING "HELLO"
#define CHAR 'a'
#define CONST_CHAR "HELLO CONST CHAR"

REGISTER_TEST_FIXTURE(XFunctorFPTest);

void XFunctorFPTest::setup(){}
void XFunctorFPTest::teardown(){}


void XFunctorFPTest::TestFunctorFP1(void)
{
    try
    {
        TestClass a;
        list<IXFunctorFP1<int,int> *> functorFP1List_1;
        list<IXFunctorFP1<int,int> *>::iterator functorFP1ListIter_1;

        list<IXFunctorFP1<double,double> *> functorFP1List_2;
        list<IXFunctorFP1<double,double> *>::iterator functorFP1ListIter_2;


        XFunctorFP1<TestClass,int,int> b(&a,&TestClass::TestFunction1_1);
        XFunctorFP1<TestClass,double,double> c(&a,&TestClass::TestFunction1_2);
        XFunctorFP1<TestClass,int,int> d(&a,&TestClass::TestFunction1_1);
        XFunctorFP1<TestClass,double,double> e(&a,&TestClass::TestFunction1_2);
        functorFP1List_1.push_back(&b);
        functorFP1List_2.push_back(&c);
        functorFP1List_1.push_back(&d);
        functorFP1List_2.push_back(&e);

        functorFP1ListIter_1 = functorFP1List_1.begin();
        functorFP1ListIter_2 = functorFP1List_2.begin();

        UT_ASSERT((**functorFP1ListIter_1)(INT) == INT);

        UT_ASSERT((**functorFP1ListIter_2)(DOUBLE) == DOUBLE);

        functorFP1ListIter_1++;
        UT_ASSERT((**functorFP1ListIter_1)(INT+4) == INT+4);

        functorFP1ListIter_2++;
        UT_ASSERT((**functorFP1ListIter_2)(DOUBLE+21.2) == DOUBLE+21.2);
    }
    catch(...)
    {
        UT_ASSERT(!"TestFunctorFP1");
    }
}

void XFunctorFPTest::TestFunctorFP2(void)
{
    try
    {
        TestClass a;
        list<IXFunctorFP2<unsigned,unsigned,double> *> functorFP2List_1;
        list<IXFunctorFP2<unsigned,unsigned,double> *>::iterator functorFP2ListIter_1;

        list<IXFunctorFP2<double,unsigned,double> *> functorFP2List_2;
        list<IXFunctorFP2<double,unsigned,double> *>::iterator functorFP2ListIter_2;

        XFunctorFP2<TestClass,unsigned,unsigned,double> b(&a,&TestClass::TestFunction2_1);
        XFunctorFP2<TestClass,double,unsigned,double> c(&a,&TestClass::TestFunction2_2);
        XFunctorFP2<TestClass,unsigned,unsigned,double> d(&a,&TestClass::TestFunction2_1);
        XFunctorFP2<TestClass,double,unsigned,double> e(&a,&TestClass::TestFunction2_2);
        functorFP2List_1.push_back(&b);
        functorFP2List_2.push_back(&c);
        functorFP2List_1.push_back(&d);
        functorFP2List_2.push_back(&e);

        functorFP2ListIter_1 = functorFP2List_1.begin();
        functorFP2ListIter_2 = functorFP2List_2.begin();

        UT_ASSERT((**functorFP2ListIter_1)(UNSIGNED,13.2) == UNSIGNED);

        UT_ASSERT((**functorFP2ListIter_2)(14,DOUBLE) == DOUBLE);

        functorFP2ListIter_1++;
        UT_ASSERT((**functorFP2ListIter_1)(UNSIGNED+16,41.5) == UNSIGNED+16);

        functorFP2ListIter_2++;
        UT_ASSERT((**functorFP2ListIter_2)(1,DOUBLE+91.233) == DOUBLE+91.233);
    }
    catch(...)
    {
        UT_ASSERT(!"TestFunctorFP2");
    }
}

void XFunctorFPTest::TestFunctorFP3(void)
{
    try
    {
        TestClass a;
        list<IXFunctorFP3<XString,int,XString,const char *> *> functorFP3List_1;
        list<IXFunctorFP3<XString,int,XString,const char *> *>::iterator functorFP3ListIter_1;

        list<IXFunctorFP3<int,int,XString,const char *> *> functorFP3List_2;
        list<IXFunctorFP3<int,int,XString,const char *> *>::iterator functorFP3ListIter_2;


        XFunctorFP3<TestClass,XString,int,XString,const char *> b(&a,&TestClass::TestFunction3_1);
        XFunctorFP3<TestClass,int,int,XString,const char *> c(&a,&TestClass::TestFunction3_2);
        XFunctorFP3<TestClass,XString,int,XString,const char *> d(&a,&TestClass::TestFunction3_1);
        XFunctorFP3<TestClass,int,int,XString,const char *> e(&a,&TestClass::TestFunction3_2);
        functorFP3List_1.push_back(&b);
        functorFP3List_2.push_back(&c);
        functorFP3List_1.push_back(&d);
        functorFP3List_2.push_back(&e);

        functorFP3ListIter_1 = functorFP3List_1.begin();
        functorFP3ListIter_2 = functorFP3List_2.begin();

        UT_ASSERT((**functorFP3ListIter_1)(INT,STRING,CONST_CHAR) == STRING);

        UT_ASSERT((**functorFP3ListIter_2)(INT,STRING,CONST_CHAR) == INT);

        functorFP3ListIter_1++;
        UT_ASSERT((**functorFP3ListIter_1)(INT,"BLAH",CONST_CHAR) == "BLAH");

        functorFP3ListIter_2++;
        UT_ASSERT((**functorFP3ListIter_2)(INT-15,STRING,CONST_CHAR) == INT-15);
    }
    catch(...)
    {
        UT_ASSERT(!"TestFunctorFP3");
    }
}

void XFunctorFPTest::TestFunctorFP4(void)
{
    try
    {
        TestClass a;
        list<IXFunctorFP4<char,const char *,double,XString,char> *> functorFP4List_1;
        list<IXFunctorFP4<char,const char *,double,XString,char> *>::iterator functorFP4ListIter_1;

        list<IXFunctorFP4<double,const char *,double,XString,char> *> functorFP4List_2;
        list<IXFunctorFP4<double,const char *,double,XString,char> *>::iterator functorFP4ListIter_2;


        XFunctorFP4<TestClass,char,const char *,double,XString,char> b(&a,&TestClass::TestFunction4_1);
        XFunctorFP4<TestClass,double,const char *,double,XString,char> c(&a,&TestClass::TestFunction4_2);
        XFunctorFP4<TestClass,char,const char *,double,XString,char> d(&a,&TestClass::TestFunction4_1);
        XFunctorFP4<TestClass,double,const char *,double,XString,char> e(&a,&TestClass::TestFunction4_2);
        functorFP4List_1.push_back(&b);
        functorFP4List_2.push_back(&c);
        functorFP4List_1.push_back(&d);
        functorFP4List_2.push_back(&e);

        functorFP4ListIter_1 = functorFP4List_1.begin();
        functorFP4ListIter_2 = functorFP4List_2.begin();

        UT_ASSERT((**functorFP4ListIter_1)(CONST_CHAR,DOUBLE,STRING,CHAR) == CHAR);

        UT_ASSERT((**functorFP4ListIter_2)(CONST_CHAR,DOUBLE,STRING,CHAR) == DOUBLE);

        functorFP4ListIter_1++;
        UT_ASSERT((**functorFP4ListIter_1)(CONST_CHAR,DOUBLE,STRING,'b') == 'b');

        functorFP4ListIter_2++;
        UT_ASSERT((**functorFP4ListIter_2)(CONST_CHAR,DOUBLE+4.1,STRING,CHAR) == DOUBLE+4.1);
    }
    catch(...)
    {
        UT_ASSERT(!"TestFunctorFP4");
    }
}

void XFunctorFPTest::TestFunctorFP5(void)
{
    try
    {
        TestClass a;
        list<IXFunctorFP5<bool,int,double,int,bool,int> *> functorFP5List_1;
        list<IXFunctorFP5<bool,int,double,int,bool,int> *>::iterator functorFP5ListIter_1;

        list<IXFunctorFP5<void,int,double,int,bool,int> *> functorFP5List_2;
        list<IXFunctorFP5<void,int,double,int,bool,int> *>::iterator functorFP5ListIter_2;


        XFunctorFP5<TestClass,bool,int,double,int,bool,int> b(&a,&TestClass::TestFunction5_1);
        XFunctorFP5<TestClass,void,int,double,int,bool,int> c(&a,&TestClass::TestFunction5_2);
        XFunctorFP5<TestClass,bool,int,double,int,bool,int> d(&a,&TestClass::TestFunction5_1);
        XFunctorFP5<TestClass,void,int,double,int,bool,int> e(&a,&TestClass::TestFunction5_2);
        functorFP5List_1.push_back(&b);
        functorFP5List_2.push_back(&c);
        functorFP5List_1.push_back(&d);
        functorFP5List_2.push_back(&e);

        functorFP5ListIter_1 = functorFP5List_1.begin();

        UT_ASSERT((**functorFP5ListIter_1)(INT,DOUBLE,INT,true,INT) == true);

        functorFP5ListIter_1++;
        UT_ASSERT((**functorFP5ListIter_1)(INT,DOUBLE,INT,false,INT) == false);

    }
    catch(...)
    {
        UT_ASSERT(!"TestFunctorFP5");
    }
}

int TestClass::TestFunction1_1(int a)
{
    return a;
}

double TestClass::TestFunction1_2(double a)
{
    return a;
}

unsigned TestClass::TestFunction2_1(unsigned a,double b)
{
    return a;
}

double TestClass::TestFunction2_2(unsigned a,double b)
{
    return b;
}

XString TestClass::TestFunction3_1(int a,XString b,const char * c)
{
    return b;
}

int TestClass::TestFunction3_2(int a,XString b,const char * c)
{
    return a;
}

char TestClass::TestFunction4_1(const char * a,double b,XString c,char d)
{
    return d;
}

double TestClass::TestFunction4_2(const char * a,double b,XString c,char d)
{
    return b;
}

bool TestClass::TestFunction5_1(int a,double b,int c,bool d,int e)
{
    return d;
}

void TestClass::TestFunction5_2(int a,double b,int c,bool d,int e)
{}

