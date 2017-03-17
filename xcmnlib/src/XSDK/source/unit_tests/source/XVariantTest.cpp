
#include <climits>
#include <cfloat>

#include "XSDK/UuidUtils.h"
#include "XSDK/XVariant.h"
#include "XSDK/XString.h"
#include "XSDK/XMemory.h"

#include "XVariantTest.h"

using namespace XSDK;

REGISTER_TEST_FIXTURE(XVariantTest);

void XVariantTest::setup()
{
}

void XVariantTest::teardown()
{
}

void XVariantTest::TestDefaultAndCopyConstructor()
{
    {
        XVariant a;
        UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);
    }

    {
        XVariant a((bool)true);
        UT_ASSERT((bool)a == true);
        UT_ASSERT(a.GetType() == XVARTYPE_BOOL);

        XVariant b(a);
        UT_ASSERT((bool)b == true);
        UT_ASSERT(b.GetType() == XVARTYPE_BOOL);
    }

    {
        XVariant a((bool)false);
        UT_ASSERT((bool)a == false);
        UT_ASSERT(a.GetType() == XVARTYPE_BOOL);

        XVariant b(a);
        UT_ASSERT((bool)b == false);
        UT_ASSERT(b.GetType() == XVARTYPE_BOOL);
    }

    {
        XVariant a((signed char)SCHAR_MAX);
        UT_ASSERT((signed char)a == SCHAR_MAX);
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_CHAR);

        XVariant b(a);
        UT_ASSERT((signed char)b == SCHAR_MAX);
        UT_ASSERT(b.GetType() == XVARTYPE_SIGNED_CHAR);
    }

    {
        XVariant a((char)SCHAR_MIN);
        UT_ASSERT((char)a == (char)SCHAR_MIN);
        UT_ASSERT(a.GetType() == XVARTYPE_CHAR);

        XVariant b(a);
        UT_ASSERT((char)b == (char)SCHAR_MIN);
        UT_ASSERT(b.GetType() == XVARTYPE_CHAR);
    }

    {
        XVariant a((unsigned char)UCHAR_MAX);
        UT_ASSERT((unsigned char)a == UCHAR_MAX);
        UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_CHAR);

        XVariant b(a);
        UT_ASSERT((unsigned char)b == UCHAR_MAX);
        UT_ASSERT(b.GetType() == XVARTYPE_UNSIGNED_CHAR);

    }

    {
        XVariant a((signed short)SHRT_MIN);
        UT_ASSERT((signed short)a == SHRT_MIN);
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_SHORT);

        XVariant b(a);
        UT_ASSERT((signed short)b == SHRT_MIN);
        UT_ASSERT(b.GetType() == XVARTYPE_SIGNED_SHORT);

    }

    {
        XVariant a((short)SHRT_MAX);
        UT_ASSERT((short)a == SHRT_MAX);
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_SHORT);

        XVariant b(a);
        UT_ASSERT((short)b == SHRT_MAX);
        UT_ASSERT(b.GetType() == XVARTYPE_SIGNED_SHORT);
    }

    {
        XVariant a((unsigned short)USHRT_MAX);
        UT_ASSERT((unsigned short)a == USHRT_MAX);
        UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_SHORT);

        XVariant b(a);
        UT_ASSERT((unsigned short)b == USHRT_MAX);
        UT_ASSERT(b.GetType() == XVARTYPE_UNSIGNED_SHORT);
    }

    {
        XVariant a((signed int)INT_MIN);
        UT_ASSERT((signed int)a == INT_MIN);
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_INT);

        XVariant b(a);
        UT_ASSERT((signed int)b == INT_MIN);
        UT_ASSERT(b.GetType() == XVARTYPE_SIGNED_INT);
    }

    {
        XVariant a((unsigned int)UINT_MAX);
        UT_ASSERT((unsigned int)a == UINT_MAX);
        UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_INT);

        XVariant b(a);
        UT_ASSERT((unsigned int)b == UINT_MAX);
        UT_ASSERT(b.GetType() == XVARTYPE_UNSIGNED_INT);
    }

    {
        XVariant a((int)INT_MAX);
        UT_ASSERT((int)a == INT_MAX);
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_INT);

        XVariant b(a);
        UT_ASSERT((int)b == INT_MAX);
        UT_ASSERT(b.GetType() == XVARTYPE_SIGNED_INT);
    }

    {
        XVariant a((signed long)LONG_MIN);
        UT_ASSERT((signed long)a == LONG_MIN);
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_LONG);

        XVariant b(a);
        UT_ASSERT((signed long)b == LONG_MIN);
        UT_ASSERT(b.GetType() == XVARTYPE_SIGNED_LONG);
    }

    {
        XVariant a((long)LONG_MAX);
        UT_ASSERT((long)a == LONG_MAX);
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_LONG);

        XVariant b(a);
        UT_ASSERT((long)b == LONG_MAX);
        UT_ASSERT(b.GetType() == XVARTYPE_SIGNED_LONG);
    }

    {
        XVariant a((unsigned long)ULONG_MAX);
        UT_ASSERT((unsigned long)a == ULONG_MAX);
        UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_LONG);

        XVariant b(a);
        UT_ASSERT((unsigned long)b == ULONG_MAX);
        UT_ASSERT(b.GetType() == XVARTYPE_UNSIGNED_LONG);
    }

    {
        XVariant a((signed long long)LLONG_MIN);
        UT_ASSERT((signed long long)a == LLONG_MIN);
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_LONG_LONG);

        XVariant b(a);
        UT_ASSERT((signed long long)b == LLONG_MIN);
        UT_ASSERT(b.GetType() == XVARTYPE_SIGNED_LONG_LONG);
    }

    {
        XVariant a((long long)LLONG_MAX);
        UT_ASSERT((long long)a == LLONG_MAX);
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_LONG_LONG);

        XVariant b(a);
        UT_ASSERT((long long)b == LLONG_MAX);
        UT_ASSERT(b.GetType() == XVARTYPE_SIGNED_LONG_LONG);
    }

    {
        XVariant a((unsigned long long)ULLONG_MAX);
        UT_ASSERT((unsigned long long)a == ULLONG_MAX);
        UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_LONG_LONG);

        XVariant b(a);
        UT_ASSERT((unsigned long long)b == ULLONG_MAX);
        UT_ASSERT(b.GetType() == XVARTYPE_UNSIGNED_LONG_LONG);
    }

    {
        XVariant a((float)FLT_MIN);
        UT_ASSERT((float)a == FLT_MIN);
        UT_ASSERT(a.GetType() == XVARTYPE_FLOAT);

        XVariant b(a);
        UT_ASSERT((float)b == FLT_MIN);
        UT_ASSERT(b.GetType() == XVARTYPE_FLOAT);
    }

    {
        XVariant a((float)FLT_MAX);
        UT_ASSERT((float)a == FLT_MAX);
        UT_ASSERT(a.GetType() == XVARTYPE_FLOAT);

        XVariant b(a);
        UT_ASSERT((float)b == FLT_MAX);
        UT_ASSERT(b.GetType() == XVARTYPE_FLOAT);
    }

    {
        XVariant a((double)DBL_MIN);
        UT_ASSERT((double)a == DBL_MIN);
        UT_ASSERT(a.GetType() == XVARTYPE_DOUBLE);

        XVariant b(a);
        UT_ASSERT((double)b == DBL_MIN);
        UT_ASSERT(b.GetType() == XVARTYPE_DOUBLE);
    }

    {
        XVariant a((double)DBL_MAX);
        UT_ASSERT((double)a == DBL_MAX);
        UT_ASSERT(a.GetType() == XVARTYPE_DOUBLE);

        XVariant b(a);
        UT_ASSERT((double)b == DBL_MAX);
        UT_ASSERT(b.GetType() == XVARTYPE_DOUBLE);
    }

    {
        XVariant a((long double)LDBL_MIN);
        UT_ASSERT((long double)a == LDBL_MIN);
        UT_ASSERT(a.GetType() == XVARTYPE_LONG_DOUBLE);

        XVariant b(a);
        UT_ASSERT((long double)b == LDBL_MIN);
        UT_ASSERT(b.GetType() == XVARTYPE_LONG_DOUBLE);
    }

    {
        XVariant a((long double)LDBL_MAX);
        UT_ASSERT((long double)a == LDBL_MAX);
        UT_ASSERT(a.GetType() == XVARTYPE_LONG_DOUBLE);

        XVariant b(a);
        UT_ASSERT((long double)b == LDBL_MAX);
        UT_ASSERT(b.GetType() == XVARTYPE_LONG_DOUBLE);
    }

    {
        XVariant a("this is a test");
        UT_ASSERT(a.GetType() == XVARTYPE_TEXT);
        XSDK::XString b = a;
        UT_ASSERT(b == "this is a test");

        XVariant c(a);
        UT_ASSERT(c.GetType() == XVARTYPE_TEXT);
        XSDK::XString d = c;
        UT_ASSERT(d == "this is a test");
    }

    {
        XSDK::XMemory mem;
        memset(&mem.Extend(mem.GetBufferSize()),0xff,mem.GetBufferSize());
        XVariant a(mem);
        UT_ASSERT(a.GetType() == XVARTYPE_BYTES);
        XSDK::XMemory mem2 = a;
        UT_ASSERT(memcmp(mem.Map(),mem2.Map(),mem.GetDataSize()) == 0);

        XVariant b(a);
        UT_ASSERT(b.GetType() == XVARTYPE_BYTES);
        XSDK::XMemory mem3 = b;
        UT_ASSERT(memcmp(mem.Map(),mem3.Map(),mem.GetDataSize()) == 0);
    }

    {
        int a = 10;
        XVariant b((void *)&a);
        void * c = (void*)b;
        int * d = (int*)c;
        UT_ASSERT(*d == a);
    }
}

template<class Type>
XSDK::XString GetType();

template<>
XSDK::XString GetType<bool>()
{
    return "bool";
}

template<>
XSDK::XString GetType<char>()
{
    return "char";
}

template<>
XSDK::XString GetType<signed char>()
{
    return "signed char";
}

template<>
XSDK::XString GetType<unsigned char>()
{
    return "unsigned char";
}

template<>
XSDK::XString GetType<unsigned short>()
{
    return "unsigned short";
}

template<>
XSDK::XString GetType<signed short>()
{
    return "signed short";
}

template<>
XSDK::XString GetType<unsigned int>()
{
    return "unsigned int";
}

template<>
XSDK::XString GetType<signed int>()
{
    return "signed int";
}

template<>
XSDK::XString GetType<signed long>()
{
    return "signed long";
}

template<>
XSDK::XString GetType<unsigned long>()
{
    return "unsigned long";
}

template<>
XSDK::XString GetType<signed long long>()
{
    return "signed long long";
}

template<>
XSDK::XString GetType<unsigned long long>()
{
    return "unsigned long long";
}

template<>
XSDK::XString GetType<float>()
{
    return "float";
}

template<>
XSDK::XString GetType<double>()
{
    return "double";
}

template<>
XSDK::XString GetType<long double>()
{
    return "long double";
}

template<class Type>
struct Tester
{
    static void Test(XVariant & a,Type toTestWith)
    {
        a = toTestWith;

        UT_ASSERT((bool)a == (toTestWith != 0));
        UT_ASSERT((char)a == (char)toTestWith);
        UT_ASSERT((signed char)a == (signed char)toTestWith);
        UT_ASSERT((unsigned char)a == (unsigned char)toTestWith);
        UT_ASSERT((signed short)a == (signed short)toTestWith);
        UT_ASSERT((unsigned short)a == (unsigned short)toTestWith);
        UT_ASSERT((int)a == (int)toTestWith);
        UT_ASSERT((unsigned int)a == (unsigned int)toTestWith);
        UT_ASSERT((long)a == (long)toTestWith);
        UT_ASSERT((unsigned long)a == (unsigned long)toTestWith);
        UT_ASSERT((long long)a == (long long)toTestWith);
        UT_ASSERT((unsigned long long)a == (unsigned long long)toTestWith);
        UT_ASSERT((float)a == (float)toTestWith);
        UT_ASSERT((double)a == (double)toTestWith);
        UT_ASSERT((long double)a == (long double)toTestWith);
        UT_ASSERT((void*)a == NULL);

        UT_ASSERT(a.Get<bool>() == (toTestWith != 0));
        UT_ASSERT(a.Get<char>() == (char)toTestWith);
        UT_ASSERT(a.Get<signed char>() == (signed char)toTestWith);
        UT_ASSERT(a.Get<unsigned char>() == (unsigned char)toTestWith);
        UT_ASSERT(a.Get<signed short>() == (signed short)toTestWith);
        UT_ASSERT(a.Get<unsigned short>() == (unsigned short)toTestWith);
        UT_ASSERT(a.Get<int>() == (int)toTestWith);
        UT_ASSERT(a.Get<unsigned int>() == (unsigned int)toTestWith);
        UT_ASSERT(a.Get<long>() == (long)toTestWith);
        UT_ASSERT(a.Get<unsigned long>() == (unsigned long)toTestWith);
        UT_ASSERT(a.Get<long long>() == (long long)toTestWith);
        UT_ASSERT(a.Get<unsigned long long>() == (unsigned long long)toTestWith);
        UT_ASSERT(a.Get<float>() == (float)toTestWith);
        UT_ASSERT(a.Get<double>() == (double)toTestWith);
        UT_ASSERT(a.Get<long double>() == (long double)toTestWith);
        UT_ASSERT(a.Get<void *>() == NULL);

        {
            X_UUID e = a;

            UT_ASSERT(memcmp(&e,&X_EMPTY_UUID,sizeof(X_UUID)) == 0);

            XSDK::XMemory c = a;

            UT_ASSERT(c.GetDataSize() == sizeof(Type));
            Type d = toTestWith;
            UT_ASSERT(memcmp(c.Map(),&d,sizeof(d)) == 0);
        }

        {
            X_UUID e;

            e = a.Get<X_UUID>();

            UT_ASSERT(memcmp(&e,&X_EMPTY_UUID,sizeof(X_UUID)) == 0);

            XSDK::XMemory c;

            c = a.Get<XSDK::XMemory>();

            UT_ASSERT(c.GetDataSize() == sizeof(Type));
            Type d = toTestWith;
            UT_ASSERT(memcmp(c.Map(),&d,sizeof(d)) == 0);
        }
    }
};

void XVariantTest::TestConversions()
{
    const unsigned int bigNum = 0x00ffffff;
    const int smallNum = 0x00ffffff;

    {
        XVariant a;

        UT_ASSERT_THROWS((bool)a,XSDK::XException);
        UT_ASSERT_THROWS((char)a,XSDK::XException);
        UT_ASSERT_THROWS((signed char)a,XSDK::XException);
        UT_ASSERT_THROWS((unsigned char)a,XSDK::XException);
        UT_ASSERT_THROWS((signed short)a,XSDK::XException);
        UT_ASSERT_THROWS((unsigned short)a,XSDK::XException);
        UT_ASSERT_THROWS((int)a,XSDK::XException);
        UT_ASSERT_THROWS((unsigned int)a,XSDK::XException);
        UT_ASSERT_THROWS((long)a,XSDK::XException);
        UT_ASSERT_THROWS((unsigned long)a,XSDK::XException);
        UT_ASSERT_THROWS((long long)a,XSDK::XException);
        UT_ASSERT_THROWS((unsigned long long)a,XSDK::XException);
        UT_ASSERT_THROWS((float)a,XSDK::XException);
        UT_ASSERT_THROWS((double)a,XSDK::XException);
        UT_ASSERT_THROWS((long double)a,XSDK::XException);
        UT_ASSERT_THROWS((X_UUID)a,XSDK::XException);
        UT_ASSERT_THROWS(XSDK::XMemory b = a,XSDK::XException);
        UT_ASSERT_THROWS(XSDK::XString b = a,XSDK::XException);
    }

    {
        XVariant a;
        Tester<bool>::Test(a,true);
        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_BOOL);
        UT_ASSERT(b == "true");
        a.Get<bool>();
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<bool>::Test(a,false);
        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_BOOL);
        UT_ASSERT(b == "false");
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<char>::Test(a,(char)bigNum);
        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_CHAR);
        UT_ASSERT(b == XSDK::XString::Format("%c",(char)bigNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<signed char>::Test(a,(signed char)smallNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_CHAR);
        UT_ASSERT(b == XSDK::XString::Format("%c",(signed char)smallNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<unsigned char>::Test(a,(unsigned char)bigNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_CHAR);
        UT_ASSERT(b == XSDK::XString::Format("%c",(unsigned char)bigNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<signed short>::Test(a,(signed short)smallNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_SHORT);
        UT_ASSERT(b == XSDK::XString::Format("%hd",(signed short)smallNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<short>::Test(a,(short)bigNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_SHORT);
        UT_ASSERT(b == XSDK::XString::Format("%hd",(short)bigNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<unsigned short>::Test(a,(unsigned short)bigNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_SHORT);
        UT_ASSERT(b == XSDK::XString::Format("%hu",(unsigned short)bigNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<signed int>::Test(a,(signed int)smallNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_INT);
        UT_ASSERT(b == XSDK::XString::Format("%d",(signed int)smallNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<int>::Test(a,(int)bigNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_INT);
        UT_ASSERT(b == XSDK::XString::Format("%d",(int)bigNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<unsigned int>::Test(a,(unsigned int)bigNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_INT);
        UT_ASSERT(b == XSDK::XString::Format("%u",(unsigned int)bigNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<signed long>::Test(a,(signed long)smallNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_LONG);
        UT_ASSERT(b == XSDK::XString::Format("%ld",(signed long)smallNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<long>::Test(a,(long)bigNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_LONG);
        UT_ASSERT(b == XSDK::XString::Format("%ld",(long)bigNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<unsigned long>::Test(a,(unsigned long)bigNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_LONG);
        UT_ASSERT(b == XSDK::XString::Format("%lu",(unsigned long)bigNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<signed long long>::Test(a,(signed long long)bigNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_LONG_LONG);
        UT_ASSERT(b == XSDK::XString::Format("%lld",(signed long long)bigNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<signed long long>::Test(a,(signed long long)smallNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_LONG_LONG);
        UT_ASSERT(b == XSDK::XString::Format("%lld",(signed long long)smallNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<unsigned long long>::Test(a,(unsigned long long)bigNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_LONG_LONG);
        UT_ASSERT(b == XSDK::XString::Format("%llu",(unsigned long long)bigNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<float>::Test(a,(float)smallNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_FLOAT);
        UT_ASSERT(b == XSDK::XString::Format("%f",(float)smallNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<float>::Test(a,(float)bigNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_FLOAT);
        UT_ASSERT(b == XSDK::XString::Format("%f",(float)bigNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        XVariant a;
        Tester<double>::Test(a,(double)smallNum);

        XSDK::XString b = a;
        UT_ASSERT(a.GetType() == XVARTYPE_DOUBLE);
        UT_ASSERT(b == XSDK::XString::Format("%f",(double)smallNum));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        long double testLongDouble = 13841.0;
        XVariant a((long double)testLongDouble);
        UT_ASSERT(a.GetType() == XVARTYPE_LONG_DOUBLE);
        //Tester<long double>::Test(a,testLongDouble);

        XSDK::XString b = a;
        UT_ASSERT(b == XSDK::XString::Format("%Lf",(long double)testLongDouble));
        UT_ASSERT(a.Get<XSDK::XString>() == b);
    }

    {
        const X_UUID testUuid = {{0xee,0xa2,0x52,0xe1,0xe2,0x4a,0x44,0x4e,0x95,0xa,0x11,0xf8,0xbe,0xce,0x60,0xd9}};

        XVariant a(testUuid);

        UT_ASSERT(a.GetType() == XVARTYPE_UUID);
        UT_ASSERT((bool)a == false);
        UT_ASSERT((char)a == 0);
        UT_ASSERT((signed char)a == 0);
        UT_ASSERT((unsigned char)a == 0);
        UT_ASSERT((signed short)a == 0);
        UT_ASSERT((unsigned short)a == 0);
        UT_ASSERT((int)a == 0);
        UT_ASSERT((unsigned int)a == 0);
        UT_ASSERT((long)a == 0);
        UT_ASSERT((unsigned long)a == 0);
        UT_ASSERT((long long)a == 0);
        UT_ASSERT((unsigned long long)a == 0);
        UT_ASSERT((float)a == 0);
        UT_ASSERT((double)a == 0);
        UT_ASSERT((long double)a == 0);

        XSDK::XString b = a;
        b.ToUpperInPlace();
        UT_ASSERT(b == "EEA252E1-E24A-444E-950A-11F8BECE60D9");

        XSDK::XMemory c = a;
        UT_ASSERT(c.GetDataSize() == sizeof(X_UUID));
        UT_ASSERT(memcmp(c.Map(),&testUuid,sizeof(X_UUID)) == 0);
    }

    {
        XVariant a("10283");

        UT_ASSERT(a.GetType() == XVARTYPE_TEXT);

        UT_ASSERT((bool)a == true);
        UT_ASSERT((char)a == (char)10283);
        UT_ASSERT((signed char)a == (signed char)10283);
        UT_ASSERT((unsigned char)a == (unsigned char)10283);
        UT_ASSERT((signed short)a == (signed short)10283);
        UT_ASSERT((unsigned short)a == (unsigned short)10283);
        UT_ASSERT((int)a == (int)10283);
        UT_ASSERT((unsigned int)a == (unsigned int)10283);
        UT_ASSERT((long)a == (long)10283);
        UT_ASSERT((unsigned long)a == (unsigned long)10283);
        UT_ASSERT((long long)a == (long long)10283);
        UT_ASSERT((unsigned long long)a == (unsigned long long)10283);
        UT_ASSERT((float)a == (float)10283);
        UT_ASSERT((double)a == (double)10283);
        UT_ASSERT((long double)a == (long double)10283);
        X_UUID uuidTest = a;

        XSDK::XString b = a;
        UT_ASSERT(b == "10283");

        XSDK::XMemory c = a;
        UT_ASSERT(c.GetDataSize() == strlen("10283"));

        UT_ASSERT(memcmp(c.Map(),b.data(),strlen("10283")) == 0);
    }

    {
        XVariant a("this is a string");

        UT_ASSERT(a.GetType() == XVARTYPE_TEXT);

        UT_ASSERT_THROWS((bool)a,XSDK::XException);
        UT_ASSERT_THROWS((signed char)a,XSDK::XException);
        UT_ASSERT_THROWS((char)a,XSDK::XException);
        UT_ASSERT_THROWS((unsigned char)a,XSDK::XException);
        UT_ASSERT_THROWS((signed short)a,XSDK::XException);
        UT_ASSERT_THROWS((unsigned short)a,XSDK::XException);
        UT_ASSERT_THROWS((signed int)a,XSDK::XException);
        UT_ASSERT_THROWS((unsigned int)a,XSDK::XException);
        UT_ASSERT_THROWS((signed long)a,XSDK::XException);
        UT_ASSERT_THROWS((unsigned long)a,XSDK::XException);
        UT_ASSERT_THROWS((float)a,XSDK::XException);
        UT_ASSERT_THROWS((double)a,XSDK::XException);
        UT_ASSERT_THROWS((long double)a,XSDK::XException);
    }

    {
        XVariant a("EEA252E1-E24A-444E-950A-11F8BECE60D9");
        const X_UUID testUuid = {{0xee,0xa2,0x52,0xe1,0xe2,0x4a,0x44,0x4e,0x95,0xa,0x11,0xf8,0xbe,0xce,0x60,0xd9}};

        UT_ASSERT(a.GetType() == XVARTYPE_TEXT);

        X_UUID b = a;

        UT_ASSERT(XUuidIsEqual(&b,&testUuid));
    }

    {
        char testMem [25];
        memset(testMem,0x4f,sizeof(testMem));

        XSDK::XMemory mem;
        memcpy(&mem.Extend(sizeof(testMem)),testMem,sizeof(testMem));

        XVariant a(mem);

        UT_ASSERT(a.GetType() == XVARTYPE_BYTES);

        {
            bool b;
            memcpy(&b,testMem,sizeof(b));
            UT_ASSERT((bool)a == (bool)b);
        }

        {
            signed char b;
            memcpy(&b,testMem,sizeof(b));
            UT_ASSERT((signed char)a == (signed char)b);
        }

        {
            char b;
            memcpy(&b,testMem,sizeof(b));
            UT_ASSERT((char)a == (char)b);
        }

        {
            unsigned char b;
            memcpy(&b,testMem,sizeof(b));
            UT_ASSERT((unsigned char)a == (unsigned char)b);
        }

        {
            signed short b;
            memcpy(&b,testMem,sizeof(b));
            UT_ASSERT((signed short)a == (signed short)b);
        }

        {
            unsigned short b;
            memcpy(&b,testMem,sizeof(b));
            UT_ASSERT((unsigned short)a == (unsigned short)b);
        }

        {
            signed int b;
            memcpy(&b,testMem,sizeof(b));
            UT_ASSERT((signed int)a == (signed int)b);
        }

        {
            unsigned int b;
            memcpy(&b,testMem,sizeof(b));
            UT_ASSERT((unsigned int)a == (unsigned int)b);
        }

        {
            signed long b;
            memcpy(&b,testMem,sizeof(b));
            UT_ASSERT((signed long)a == (signed long)b);
        }

        {
            unsigned long b;
            memcpy(&b,testMem,sizeof(b));
            UT_ASSERT((unsigned long)a == (unsigned long)b);
        }

        {
            signed long long b;
            memcpy(&b,testMem,sizeof(b));
            UT_ASSERT((signed long long)a == (signed long long)b);
        }

        {
            unsigned long long b;
            memcpy(&b,testMem,sizeof(b));
            UT_ASSERT((unsigned long long)a == (unsigned long long)b);
        }

        {
            float b;
            memcpy(&b,testMem,sizeof(b));
            UT_ASSERT((float)a == (float)b);
        }

        {
            double b;
            memcpy(&b,testMem,sizeof(b));
            UT_ASSERT((double)a == (double)b);
        }

        {
            XSDK::XString b((const char *)testMem,sizeof(testMem));
            XSDK::XString testString = a;
            UT_ASSERT(testString == b);
        }
    }
}

void XVariantTest::TestAssignmentOperators()
{
    bool testBool = false;
    signed char testSignedChar = -12;
    char testChar = 50;
    unsigned char testUnsignedChar = 255;
    signed short testSignedShort = -4452;
    unsigned short testUnsignedShort = 4452;
    signed int testSignedInt = -113294;
    unsigned int testUnsignedInt = 113294;
    signed long testSignedLong = -80833;
    unsigned long testUnsignedLong = 80833;
    signed long long testSignedLongLong = -1993144;
    unsigned long long testUnsignedLongLong = 1993144;
    float testFloat = 1031.8f;
    double testDouble = -93.55;
    long double testLongDouble = 0.0;

    const char mem[] = {0x01,0x04,0x08};
    XSDK::XMemory testMem;
    memcpy(&testMem.Extend(sizeof(mem)),mem,sizeof(mem));

    XSDK::XString testString("blah blah");
    const X_UUID testUuid = {{0x1e,0xa2,0x52,0x91,0xe2,0x4a,0x34,0x4e,0x95,0x1a,0x11,0xed,0xbe,0xce,0x99,0x09}};

    {
        XVariant a;

        UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

        UT_ASSERT_NO_THROW(a = testBool);
        UT_ASSERT(a.GetType() == XVARTYPE_BOOL);
        UT_ASSERT((bool)a == testBool);

        UT_ASSERT_NO_THROW(a = testSignedChar);
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_CHAR);
        UT_ASSERT((signed char)a == testSignedChar);

        UT_ASSERT_NO_THROW(a = testChar);
        UT_ASSERT(a.GetType() == XVARTYPE_CHAR);
        UT_ASSERT((char)a == testChar);

        UT_ASSERT_NO_THROW(a = testUnsignedChar);
        UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_CHAR);
        UT_ASSERT((unsigned char)a == testUnsignedChar);

        UT_ASSERT_NO_THROW(a = testSignedShort);
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_SHORT);
        UT_ASSERT((signed short)a == testSignedShort);

        UT_ASSERT_NO_THROW(a = testUnsignedShort);
        UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_SHORT);
        UT_ASSERT((unsigned short)a == testUnsignedShort);

        UT_ASSERT_NO_THROW(a = testSignedInt);
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_INT);
        UT_ASSERT((signed int)a == testSignedInt);

        UT_ASSERT_NO_THROW(a = testUnsignedInt);
        UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_INT);
        UT_ASSERT((unsigned int)a == testUnsignedInt);

        UT_ASSERT_NO_THROW(a = testSignedLong);
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_LONG);
        UT_ASSERT((signed long)a == testSignedLong);

        UT_ASSERT_NO_THROW(a = testUnsignedLong);
        UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_LONG);
        UT_ASSERT((unsigned long)a == testUnsignedLong);

        UT_ASSERT_NO_THROW(a = testSignedLongLong);
        UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_LONG_LONG);
        UT_ASSERT((signed long long)a == testSignedLongLong);

        UT_ASSERT_NO_THROW(a = testUnsignedLongLong);
        UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_LONG_LONG);
        UT_ASSERT((unsigned long long)a == testUnsignedLongLong);

        UT_ASSERT_NO_THROW(a = testFloat);
        UT_ASSERT(a.GetType() == XVARTYPE_FLOAT);
        UT_ASSERT((float)a == testFloat);

        UT_ASSERT_NO_THROW(a = testDouble);
        UT_ASSERT(a.GetType() == XVARTYPE_DOUBLE);
        UT_ASSERT((double)a == testDouble);

        UT_ASSERT_NO_THROW(a = testLongDouble);
        UT_ASSERT(a.GetType() == XVARTYPE_LONG_DOUBLE);
        UT_ASSERT((long double)a == testLongDouble);

        UT_ASSERT_NO_THROW(a = testMem);
        UT_ASSERT(a.GetType() == XVARTYPE_BYTES);
        XSDK::XMemory moreMem = a;
        UT_ASSERT(memcmp(moreMem.Map(),testMem.Map(),testMem.GetDataSize()) == 0);

        UT_ASSERT_NO_THROW(a = testUuid);
        UT_ASSERT(a.GetType() == XVARTYPE_UUID);
        X_UUID moreUuid = a;
        UT_ASSERT(memcmp(&moreUuid,&testUuid,sizeof(testUuid)) == 0);

        UT_ASSERT_NO_THROW(a = testString);
        UT_ASSERT(a.GetType() == XVARTYPE_TEXT);
        XSDK::XString moreString = a;
        UT_ASSERT(moreString == testString);
    }
}

void XVariantTest::TestCastOperators()
{
    bool testBool = true;
    signed char testSignedChar = -42;
    char testChar = 100;
    unsigned char testUnsignedChar = 225;
    signed short testSignedShort = -452;
    unsigned short testUnsignedShort = 445;
    signed int testSignedInt = -11394;
    unsigned int testUnsignedInt = 13294;
    signed long testSignedLong = -8833;
    unsigned long testUnsignedLong = 8833;
    signed long long testSignedLongLong = -193144;
    unsigned long long testUnsignedLongLong = 993144;
    float testFloat = 131.8f;
    double testDouble = -3.55;
    long double testLongDouble = 1000333.4423;

    const char mem[] = {0x11,0x04,0x08,0x10};
    XSDK::XMemory testMem;
    memcpy(&testMem.Extend(sizeof(mem)),mem,sizeof(mem));

    XSDK::XString testString("blah blah blaskndadn");
    const X_UUID testUuid = {{0x10,0x12,0x82,0x99,0xe2,0x4a,0x34,0x4e,0x95,0x1a,0x11,0xed,0xbe,0xcc,0xc9,0x09}};

    {
        XVariant a(testBool);
        bool test = false;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testBool);
    }

    {
        XVariant a(testSignedChar);
        signed char test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testSignedChar);
    }

    {
        XVariant a(testChar);
        char test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testChar);
    }

    {
        XVariant a(testUnsignedChar);
        unsigned char test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testUnsignedChar);
    }

    {
        XVariant a(testSignedShort);
        signed short test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testSignedShort);
    }

    {
        XVariant a(testUnsignedShort);
        unsigned short test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testUnsignedShort);
    }

    {
        XVariant a(testSignedInt);
        signed int test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testSignedInt);
    }

    {
        XVariant a(testUnsignedInt);
        unsigned int test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testUnsignedInt);
    }

    {
        XVariant a(testSignedLong);
        signed long test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testSignedLong);
    }

    {
        XVariant a(testUnsignedLong);
        unsigned long test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testUnsignedLong);
    }

    {
        XVariant a(testSignedLongLong);
        signed long long test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testSignedLongLong);
    }

    {
        XVariant a(testUnsignedLongLong);
        unsigned long long test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testUnsignedLongLong);
    }

    {
        XVariant a(testFloat);
        float test = 0.0f;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testFloat);
    }

    {
        XVariant a(testDouble);
        double test = 0.0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testDouble);
    }

    {
        XVariant a(testLongDouble);
        long double test = 0.0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testLongDouble);
    }

    {
        XVariant a(testString);
        XSDK::XString test = a;
        UT_ASSERT(test == testString);
    }

    {
        XVariant a(testMem);
        XSDK::XMemory test;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(memcmp(test.Map(),testMem.Map(),testMem.GetDataSize()) == 0);
    }

    {
        XVariant a(testUuid);
        X_UUID test;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(memcmp(&test,&testUuid,sizeof(X_UUID)) == 0);
    }
}

void XVariantTest::TestClear()
{
    bool testBool = true;
    signed char testSignedChar = -42;
    char testChar = 100;
    unsigned char testUnsignedChar = 225;
    signed short testSignedShort = -452;
    unsigned short testUnsignedShort = 445;
    signed int testSignedInt = -11394;
    unsigned int testUnsignedInt = 13294;
    signed long testSignedLong = -8833;
    unsigned long testUnsignedLong = 8833;
    signed long long testSignedLongLong = -193144;
    unsigned long long testUnsignedLongLong = 993144;
    float testFloat = 131.8f;
    double testDouble = -3.55;
    long double testLongDouble = 1000333.4423;

    const char mem[] = {0x11,0x04,0x08,0x10};
    XSDK::XMemory testMem;
    memcpy(&testMem.Extend(sizeof(mem)),mem,sizeof(mem));

    XSDK::XString testString("blah blah blaskndadn");
    const X_UUID testUuid = {{0x10,0x12,0x82,0x99,0xe2,0x4a,0x34,0x4e,0x95,0x1a,0x11,0xed,0xbe,0xcc,0xc9,0x09}};

    XVariant a;

    UT_ASSERT(a.IsEmpty() == true);

    UT_ASSERT_NO_THROW(a = testBool);
    UT_ASSERT(a.GetType() == XVARTYPE_BOOL);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testSignedChar);
    UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_CHAR);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testChar);
    UT_ASSERT(a.GetType() == XVARTYPE_CHAR);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testUnsignedChar);
    UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_CHAR);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testSignedShort);
    UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_SHORT);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testUnsignedShort);
    UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_SHORT);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testUnsignedInt);
    UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_INT);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testSignedInt);
    UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_INT);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testSignedLong);
    UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_LONG);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testUnsignedLong);
    UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_LONG);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testSignedLongLong);
    UT_ASSERT(a.GetType() == XVARTYPE_SIGNED_LONG_LONG);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testUnsignedLongLong);
    UT_ASSERT(a.GetType() == XVARTYPE_UNSIGNED_LONG_LONG);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testMem);
    UT_ASSERT(a.GetType() == XVARTYPE_BYTES);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testString);
    UT_ASSERT(a.GetType() == XVARTYPE_TEXT);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testUuid);
    UT_ASSERT(a.GetType() == XVARTYPE_UUID);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testFloat);
    UT_ASSERT(a.GetType() == XVARTYPE_FLOAT);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testDouble);
    UT_ASSERT(a.GetType() == XVARTYPE_DOUBLE);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testLongDouble);
    UT_ASSERT(a.GetType() == XVARTYPE_LONG_DOUBLE);
    UT_ASSERT_NO_THROW(a.Clear());
    UT_ASSERT(a.IsEmpty() == true);
    UT_ASSERT(a.GetType() == XVARTYPE_EMPTY);
}
