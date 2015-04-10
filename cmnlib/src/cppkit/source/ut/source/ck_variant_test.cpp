
#include "framework.h"

#include <climits>
#include <cfloat>

#include "cppkit/ck_uuid_utils.h"
#include "cppkit/ck_variant.h"
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"

#include "ck_variant_test.h"

using namespace std;
using namespace cppkit;

REGISTER_TEST_FIXTURE(ck_variant_test);

void ck_variant_test::setup()
{
}

void ck_variant_test::teardown()
{
}

void ck_variant_test::test_default_and_copy_constructor()
{

    {
        ck_variant a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);
    }

    {
        ck_variant a((bool)true);
        UT_ASSERT((bool)a == true);
        UT_ASSERT(a.get_type() == CK_VARTYPE_BOOL);

        ck_variant b(a);
        UT_ASSERT((bool)b == true);
        UT_ASSERT(b.get_type() == CK_VARTYPE_BOOL);
    }

    {
        ck_variant a((bool)false);
        UT_ASSERT((bool)a == false);
        UT_ASSERT(a.get_type() == CK_VARTYPE_BOOL);

        ck_variant b(a);
        UT_ASSERT((bool)b == false);
        UT_ASSERT(b.get_type() == CK_VARTYPE_BOOL);
    }

    {
        ck_variant a((signed char)SCHAR_MAX);
        UT_ASSERT((signed char)a == SCHAR_MAX);
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_CHAR);

        ck_variant b(a);
        UT_ASSERT((signed char)b == SCHAR_MAX);
        UT_ASSERT(b.get_type() == CK_VARTYPE_SIGNED_CHAR);
    }

    {
        ck_variant a((char)SCHAR_MIN);
        UT_ASSERT((char)a == (char)SCHAR_MIN);
        UT_ASSERT(a.get_type() == CK_VARTYPE_CHAR);

        ck_variant b(a);
        UT_ASSERT((char)b == (char)SCHAR_MIN);
        UT_ASSERT(b.get_type() == CK_VARTYPE_CHAR);
    }

    {
        ck_variant a((unsigned char)UCHAR_MAX);
        UT_ASSERT((unsigned char)a == UCHAR_MAX);
        UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_CHAR);

        ck_variant b(a);
        UT_ASSERT((unsigned char)b == UCHAR_MAX);
        UT_ASSERT(b.get_type() == CK_VARTYPE_UNSIGNED_CHAR);

    }

    {
        ck_variant a((signed short)SHRT_MIN);
        UT_ASSERT((signed short)a == SHRT_MIN);
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_SHORT);

        ck_variant b(a);
        UT_ASSERT((signed short)b == SHRT_MIN);
        UT_ASSERT(b.get_type() == CK_VARTYPE_SIGNED_SHORT);

    }

    {
        ck_variant a((short)SHRT_MAX);
        UT_ASSERT((short)a == SHRT_MAX);
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_SHORT);

        ck_variant b(a);
        UT_ASSERT((short)b == SHRT_MAX);
        UT_ASSERT(b.get_type() == CK_VARTYPE_SIGNED_SHORT);
    }

    {
        ck_variant a((unsigned short)USHRT_MAX);
        UT_ASSERT((unsigned short)a == USHRT_MAX);
        UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_SHORT);

        ck_variant b(a);
        UT_ASSERT((unsigned short)b == USHRT_MAX);
        UT_ASSERT(b.get_type() == CK_VARTYPE_UNSIGNED_SHORT);
    }

    {
        ck_variant a((signed int)INT_MIN);
        UT_ASSERT((signed int)a == INT_MIN);
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_INT);

        ck_variant b(a);
        UT_ASSERT((signed int)b == INT_MIN);
        UT_ASSERT(b.get_type() == CK_VARTYPE_SIGNED_INT);
    }

    {
        ck_variant a((unsigned int)UINT_MAX);
        UT_ASSERT((unsigned int)a == UINT_MAX);
        UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_INT);

        ck_variant b(a);
        UT_ASSERT((unsigned int)b == UINT_MAX);
        UT_ASSERT(b.get_type() == CK_VARTYPE_UNSIGNED_INT);
    }

    {
        ck_variant a((int)INT_MAX);
        UT_ASSERT((int)a == INT_MAX);
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_INT);

        ck_variant b(a);
        UT_ASSERT((int)b == INT_MAX);
        UT_ASSERT(b.get_type() == CK_VARTYPE_SIGNED_INT);
    }

    {
        ck_variant a((signed long)LONG_MIN);
        UT_ASSERT((signed long)a == LONG_MIN);
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_LONG);

        ck_variant b(a);
        UT_ASSERT((signed long)b == LONG_MIN);
        UT_ASSERT(b.get_type() == CK_VARTYPE_SIGNED_LONG);
    }

    {
        ck_variant a((long)LONG_MAX);
        UT_ASSERT((long)a == LONG_MAX);
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_LONG);

        ck_variant b(a);
        UT_ASSERT((long)b == LONG_MAX);
        UT_ASSERT(b.get_type() == CK_VARTYPE_SIGNED_LONG);
    }

    {
        ck_variant a((unsigned long)ULONG_MAX);
        UT_ASSERT((unsigned long)a == ULONG_MAX);
        UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_LONG);

        ck_variant b(a);
        UT_ASSERT((unsigned long)b == ULONG_MAX);
        UT_ASSERT(b.get_type() == CK_VARTYPE_UNSIGNED_LONG);
    }

    {
        ck_variant a((signed long long)LLONG_MIN);
        UT_ASSERT((signed long long)a == LLONG_MIN);
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_LONG_LONG);

        ck_variant b(a);
        UT_ASSERT((signed long long)b == LLONG_MIN);
        UT_ASSERT(b.get_type() == CK_VARTYPE_SIGNED_LONG_LONG);
    }

    {
        ck_variant a((long long)LLONG_MAX);
        UT_ASSERT((long long)a == LLONG_MAX);
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_LONG_LONG);

        ck_variant b(a);
        UT_ASSERT((long long)b == LLONG_MAX);
        UT_ASSERT(b.get_type() == CK_VARTYPE_SIGNED_LONG_LONG);
    }

    {
        ck_variant a((unsigned long long)ULLONG_MAX);
        UT_ASSERT((unsigned long long)a == ULLONG_MAX);
        UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_LONG_LONG);

        ck_variant b(a);
        UT_ASSERT((unsigned long long)b == ULLONG_MAX);
        UT_ASSERT(b.get_type() == CK_VARTYPE_UNSIGNED_LONG_LONG);
    }

    {
        ck_variant a((float)FLT_MIN);
        UT_ASSERT((float)a == FLT_MIN);
        UT_ASSERT(a.get_type() == CK_VARTYPE_FLOAT);

        ck_variant b(a);
        UT_ASSERT((float)b == FLT_MIN);
        UT_ASSERT(b.get_type() == CK_VARTYPE_FLOAT);
    }

    {
        ck_variant a((float)FLT_MAX);
        UT_ASSERT((float)a == FLT_MAX);
        UT_ASSERT(a.get_type() == CK_VARTYPE_FLOAT);

        ck_variant b(a);
        UT_ASSERT((float)b == FLT_MAX);
        UT_ASSERT(b.get_type() == CK_VARTYPE_FLOAT);
    }

    {
        ck_variant a((double)DBL_MIN);
        UT_ASSERT((double)a == DBL_MIN);
        UT_ASSERT(a.get_type() == CK_VARTYPE_DOUBLE);

        ck_variant b(a);
        UT_ASSERT((double)b == DBL_MIN);
        UT_ASSERT(b.get_type() == CK_VARTYPE_DOUBLE);
    }

    {
        ck_variant a((double)DBL_MAX);
        UT_ASSERT((double)a == DBL_MAX);
        UT_ASSERT(a.get_type() == CK_VARTYPE_DOUBLE);

        ck_variant b(a);
        UT_ASSERT((double)b == DBL_MAX);
        UT_ASSERT(b.get_type() == CK_VARTYPE_DOUBLE);
    }

    {
        ck_variant a((long double)LDBL_MIN);
        UT_ASSERT((long double)a == LDBL_MIN);
        UT_ASSERT(a.get_type() == CK_VARTYPE_LONG_DOUBLE);

        ck_variant b(a);
        UT_ASSERT((long double)b == LDBL_MIN);
        UT_ASSERT(b.get_type() == CK_VARTYPE_LONG_DOUBLE);
    }

    {
        ck_variant a((long double)LDBL_MAX);
        UT_ASSERT((long double)a == LDBL_MAX);
        UT_ASSERT(a.get_type() == CK_VARTYPE_LONG_DOUBLE);

        ck_variant b(a);
        UT_ASSERT((long double)b == LDBL_MAX);
        UT_ASSERT(b.get_type() == CK_VARTYPE_LONG_DOUBLE);
    }

    {
        ck_variant a("this is a test");
        UT_ASSERT(a.get_type() == CK_VARTYPE_TEXT);
        ck_string b = a;
        UT_ASSERT(b == "this is a test");

        ck_variant c(a);
        UT_ASSERT(c.get_type() == CK_VARTYPE_TEXT);
        ck_string d = c;
        UT_ASSERT(d == "this is a test");
    }

    {
        int a = 10;
        ck_variant b((void *)&a);
        void * c = (void*)b;
        int * d = (int*)c;
        UT_ASSERT(*d == a);
    }
}

template<class Type>
ck_string get_type();

template<>
ck_string get_type<bool>()
{
    return "bool";
}

template<>
ck_string get_type<char>()
{
    return "char";
}

template<>
ck_string get_type<signed char>()
{
    return "signed char";
}

template<>
ck_string get_type<unsigned char>()
{
    return "unsigned char";
}

template<>
ck_string get_type<unsigned short>()
{
    return "unsigned short";
}

template<>
ck_string get_type<signed short>()
{
    return "signed short";
}

template<>
ck_string get_type<unsigned int>()
{
    return "unsigned int";
}

template<>
ck_string get_type<signed int>()
{
    return "signed int";
}

template<>
ck_string get_type<signed long>()
{
    return "signed long";
}

template<>
ck_string get_type<unsigned long>()
{
    return "unsigned long";
}

template<>
ck_string get_type<signed long long>()
{
    return "signed long long";
}

template<>
ck_string get_type<unsigned long long>()
{
    return "unsigned long long";
}

template<>
ck_string get_type<float>()
{
    return "float";
}

template<>
ck_string get_type<double>()
{
    return "double";
}

template<>
ck_string get_type<long double>()
{
    return "long double";
}

template<class Type>
struct Tester
{
    static void Test(ck_variant & a,Type toTestWith)
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

        UT_ASSERT(a.get<bool>() == (toTestWith != 0));
        UT_ASSERT(a.get<char>() == (char)toTestWith);
        UT_ASSERT(a.get<signed char>() == (signed char)toTestWith);
        UT_ASSERT(a.get<unsigned char>() == (unsigned char)toTestWith);
        UT_ASSERT(a.get<signed short>() == (signed short)toTestWith);
        UT_ASSERT(a.get<unsigned short>() == (unsigned short)toTestWith);
        UT_ASSERT(a.get<int>() == (int)toTestWith);
        UT_ASSERT(a.get<unsigned int>() == (unsigned int)toTestWith);
        UT_ASSERT(a.get<long>() == (long)toTestWith);
        UT_ASSERT(a.get<unsigned long>() == (unsigned long)toTestWith);
        UT_ASSERT(a.get<long long>() == (long long)toTestWith);
        UT_ASSERT(a.get<unsigned long long>() == (unsigned long long)toTestWith);
        UT_ASSERT(a.get<float>() == (float)toTestWith);
        UT_ASSERT(a.get<double>() == (double)toTestWith);
        UT_ASSERT(a.get<long double>() == (long double)toTestWith);
        UT_ASSERT(a.get<void *>() == NULL);

        {
            ck_uuid_bits e = a;

            UT_ASSERT(memcmp(&e,&CK_EMPTY_UUID,sizeof(ck_uuid_bits)) == 0);

            ck_memory c = a;

            UT_ASSERT(c.size_data() == sizeof(Type));
            Type d = toTestWith;
            UT_ASSERT(memcmp(c.map().get_ptr(),&d,sizeof(d)) == 0);
        }

        {
            ck_uuid_bits e;

            e = a.get<ck_uuid_bits>();

            UT_ASSERT(memcmp(&e,&CK_EMPTY_UUID,sizeof(ck_uuid_bits)) == 0);

            ck_memory c;

            c = a.get<ck_memory>();

            UT_ASSERT(c.size_data() == sizeof(Type));
            Type d = toTestWith;
            UT_ASSERT(memcmp(c.map().get_ptr(),&d,sizeof(d)) == 0);
        }
    }
};

void ck_variant_test::test_conversions()
{
    const unsigned int bigNum = 0x00ffffff;
    const int smallNum = 0x00ffffff;

    {
        ck_variant a;

        UT_ASSERT_THROWS((bool)a,ck_exception);
        UT_ASSERT_THROWS((char)a,ck_exception);
        UT_ASSERT_THROWS((signed char)a,ck_exception);
        UT_ASSERT_THROWS((unsigned char)a,ck_exception);
        UT_ASSERT_THROWS((signed short)a,ck_exception);
        UT_ASSERT_THROWS((unsigned short)a,ck_exception);
        UT_ASSERT_THROWS((int)a,ck_exception);
        UT_ASSERT_THROWS((unsigned int)a,ck_exception);
        UT_ASSERT_THROWS((long)a,ck_exception);
        UT_ASSERT_THROWS((unsigned long)a,ck_exception);
        UT_ASSERT_THROWS((long long)a,ck_exception);
        UT_ASSERT_THROWS((unsigned long long)a,ck_exception);
        UT_ASSERT_THROWS((float)a,ck_exception);
        UT_ASSERT_THROWS((double)a,ck_exception);
        UT_ASSERT_THROWS((long double)a,ck_exception);
        UT_ASSERT_THROWS((ck_uuid_bits)a,ck_exception);
        //UT_ASSERT_THROWS(ck_memory b = a,ck_exception);
        //UT_ASSERT_THROWS(ck_string b = a,ck_exception);
    }

    {
        ck_variant a;
        Tester<bool>::Test(a,true);
        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_BOOL);
        UT_ASSERT(b == "true");
        a.get<bool>();
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<bool>::Test(a,false);
        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_BOOL);
        UT_ASSERT(b == "false");
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<char>::Test(a,(char)bigNum);
        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_CHAR);
        UT_ASSERT(b == ck_string::format("%c",(char)bigNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<signed char>::Test(a,(signed char)smallNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_CHAR);
        UT_ASSERT(b == ck_string::format("%c",(signed char)smallNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<unsigned char>::Test(a,(unsigned char)bigNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_CHAR);
        UT_ASSERT(b == ck_string::format("%c",(unsigned char)bigNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<signed short>::Test(a,(signed short)smallNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_SHORT);
        UT_ASSERT(b == ck_string::format("%hd",(signed short)smallNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<short>::Test(a,(short)bigNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_SHORT);
        UT_ASSERT(b == ck_string::format("%hd",(short)bigNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<unsigned short>::Test(a,(unsigned short)bigNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_SHORT);
        UT_ASSERT(b == ck_string::format("%hu",(unsigned short)bigNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<signed int>::Test(a,(signed int)smallNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_INT);
        UT_ASSERT(b == ck_string::format("%d",(signed int)smallNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<int>::Test(a,(int)bigNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_INT);
        UT_ASSERT(b == ck_string::format("%d",(int)bigNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<unsigned int>::Test(a,(unsigned int)bigNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_INT);
        UT_ASSERT(b == ck_string::format("%u",(unsigned int)bigNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<signed long>::Test(a,(signed long)smallNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_LONG);
        UT_ASSERT(b == ck_string::format("%ld",(signed long)smallNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<long>::Test(a,(long)bigNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_LONG);
        UT_ASSERT(b == ck_string::format("%ld",(long)bigNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<unsigned long>::Test(a,(unsigned long)bigNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_LONG);
        UT_ASSERT(b == ck_string::format("%lu",(unsigned long)bigNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<signed long long>::Test(a,(signed long long)bigNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_LONG_LONG);
        UT_ASSERT(b == ck_string::format("%lld",(signed long long)bigNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<signed long long>::Test(a,(signed long long)smallNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_LONG_LONG);
        UT_ASSERT(b == ck_string::format("%lld",(signed long long)smallNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<unsigned long long>::Test(a,(unsigned long long)bigNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_LONG_LONG);
        UT_ASSERT(b == ck_string::format("%llu",(unsigned long long)bigNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<float>::Test(a,(float)smallNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_FLOAT);
        UT_ASSERT(b == ck_string::format("%f",(float)smallNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<float>::Test(a,(float)bigNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_FLOAT);
        UT_ASSERT(b == ck_string::format("%f",(float)bigNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        ck_variant a;
        Tester<double>::Test(a,(double)smallNum);

        ck_string b = a;
        UT_ASSERT(a.get_type() == CK_VARTYPE_DOUBLE);
        UT_ASSERT(b == ck_string::format("%f",(double)smallNum));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        long double testLongDouble = 13841.0;
        ck_variant a((long double)testLongDouble);
        UT_ASSERT(a.get_type() == CK_VARTYPE_LONG_DOUBLE);
        //Tester<long double>::Test(a,testLongDouble);

        ck_string b = a;
        UT_ASSERT(b == ck_string::format("%Lf",(long double)testLongDouble));
        UT_ASSERT(a.get<ck_string>() == b);
    }

    {
        const ck_uuid_bits testUuid = {{0xee,0xa2,0x52,0xe1,0xe2,0x4a,0x44,0x4e,0x95,0xa,0x11,0xf8,0xbe,0xce,0x60,0xd9}};

        ck_variant a(testUuid);

        UT_ASSERT(a.get_type() == CK_VARTYPE_UUID);
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

        ck_string b = a;
        b = b.to_upper();
        UT_ASSERT(b == "EEA252E1-E24A-444E-950A-11F8BECE60D9");

        ck_memory c = a;
        UT_ASSERT(c.size_data() == sizeof(ck_uuid_bits));
        UT_ASSERT(memcmp(c.map().get_ptr(),&testUuid,sizeof(ck_uuid_bits)) == 0);
    }

    {
        ck_variant a("10283");

        UT_ASSERT(a.get_type() == CK_VARTYPE_TEXT);

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
        ck_uuid_bits uuidTest = a;

        ck_string b = a;
        UT_ASSERT(b == "10283");

        ck_memory c = a;
        UT_ASSERT(c.size_data() == strlen("10283"));

        UT_ASSERT(memcmp(c.map().get_ptr(),b.data(),strlen("10283")) == 0);
    }

    {
        ck_variant a("this is a string");

        UT_ASSERT(a.get_type() == CK_VARTYPE_TEXT);

        UT_ASSERT_THROWS((bool)a,ck_exception);
        UT_ASSERT_THROWS((signed char)a,ck_exception);
        UT_ASSERT_THROWS((char)a,ck_exception);
        UT_ASSERT_THROWS((unsigned char)a,ck_exception);
        UT_ASSERT_THROWS((signed short)a,ck_exception);
        UT_ASSERT_THROWS((unsigned short)a,ck_exception);
        UT_ASSERT_THROWS((signed int)a,ck_exception);
        UT_ASSERT_THROWS((unsigned int)a,ck_exception);
        UT_ASSERT_THROWS((signed long)a,ck_exception);
        UT_ASSERT_THROWS((unsigned long)a,ck_exception);
        UT_ASSERT_THROWS((float)a,ck_exception);
        UT_ASSERT_THROWS((double)a,ck_exception);
        UT_ASSERT_THROWS((long double)a,ck_exception);
    }

    {
        ck_variant a("EEA252E1-E24A-444E-950A-11F8BECE60D9");
        const ck_uuid_bits testUuid = {{0xee,0xa2,0x52,0xe1,0xe2,0x4a,0x44,0x4e,0x95,0xa,0x11,0xf8,0xbe,0xce,0x60,0xd9}};

        UT_ASSERT(a.get_type() == CK_VARTYPE_TEXT);

        ck_uuid_bits b = a;

        UT_ASSERT(ck_uuid_is_equal(&b,&testUuid));
    }

    {
        char testMem [25];
        memset(testMem,0x4f,sizeof(testMem));

        ck_memory mem;
        memcpy(mem.extend_data(sizeof(testMem)).get_ptr(),testMem,sizeof(testMem));

        ck_variant a(mem);

        UT_ASSERT(a.get_type() == CK_VARTYPE_BYTES);

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
            ck_string b((const char *)testMem,sizeof(testMem));
            ck_string testString = a;
            UT_ASSERT(testString == b);
        }
    }
}

void ck_variant_test::test_assignment_operators()
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
    ck_memory testMem;
    memcpy(testMem.extend_data(sizeof(mem)).get_ptr(),mem,sizeof(mem));

    ck_string testString("blah blah");
    const ck_uuid_bits testUuid = {{0x1e,0xa2,0x52,0x91,0xe2,0x4a,0x34,0x4e,0x95,0x1a,0x11,0xed,0xbe,0xce,0x99,0x09}};

    {
        ck_variant a;

        UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

        UT_ASSERT_NO_THROW(a = testBool);
        UT_ASSERT(a.get_type() == CK_VARTYPE_BOOL);
        UT_ASSERT((bool)a == testBool);

        UT_ASSERT_NO_THROW(a = testSignedChar);
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_CHAR);
        UT_ASSERT((signed char)a == testSignedChar);

        UT_ASSERT_NO_THROW(a = testChar);
        UT_ASSERT(a.get_type() == CK_VARTYPE_CHAR);
        UT_ASSERT((char)a == testChar);

        UT_ASSERT_NO_THROW(a = testUnsignedChar);
        UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_CHAR);
        UT_ASSERT((unsigned char)a == testUnsignedChar);

        UT_ASSERT_NO_THROW(a = testSignedShort);
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_SHORT);
        UT_ASSERT((signed short)a == testSignedShort);

        UT_ASSERT_NO_THROW(a = testUnsignedShort);
        UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_SHORT);
        UT_ASSERT((unsigned short)a == testUnsignedShort);

        UT_ASSERT_NO_THROW(a = testSignedInt);
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_INT);
        UT_ASSERT((signed int)a == testSignedInt);

        UT_ASSERT_NO_THROW(a = testUnsignedInt);
        UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_INT);
        UT_ASSERT((unsigned int)a == testUnsignedInt);

        UT_ASSERT_NO_THROW(a = testSignedLong);
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_LONG);
        UT_ASSERT((signed long)a == testSignedLong);

        UT_ASSERT_NO_THROW(a = testUnsignedLong);
        UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_LONG);
        UT_ASSERT((unsigned long)a == testUnsignedLong);

        UT_ASSERT_NO_THROW(a = testSignedLongLong);
        UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_LONG_LONG);
        UT_ASSERT((signed long long)a == testSignedLongLong);

        UT_ASSERT_NO_THROW(a = testUnsignedLongLong);
        UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_LONG_LONG);
        UT_ASSERT((unsigned long long)a == testUnsignedLongLong);

        UT_ASSERT_NO_THROW(a = testFloat);
        UT_ASSERT(a.get_type() == CK_VARTYPE_FLOAT);
        UT_ASSERT((float)a == testFloat);

        UT_ASSERT_NO_THROW(a = testDouble);
        UT_ASSERT(a.get_type() == CK_VARTYPE_DOUBLE);
        UT_ASSERT((double)a == testDouble);

        UT_ASSERT_NO_THROW(a = testLongDouble);
        UT_ASSERT(a.get_type() == CK_VARTYPE_LONG_DOUBLE);
        UT_ASSERT((long double)a == testLongDouble);

        UT_ASSERT_NO_THROW(a = testMem);
        UT_ASSERT(a.get_type() == CK_VARTYPE_BYTES);
        ck_memory moreMem = a;
        UT_ASSERT(memcmp(moreMem.map().get_ptr(),testMem.map().get_ptr(),testMem.size_data()) == 0);

        UT_ASSERT_NO_THROW(a = testUuid);
        UT_ASSERT(a.get_type() == CK_VARTYPE_UUID);
        ck_uuid_bits moreUuid = a;
        UT_ASSERT(memcmp(&moreUuid,&testUuid,sizeof(testUuid)) == 0);

        UT_ASSERT_NO_THROW(a = testString);
        UT_ASSERT(a.get_type() == CK_VARTYPE_TEXT);
        ck_string moreString = a;
        UT_ASSERT(moreString == testString);
    }
}

void ck_variant_test::test_cast_operators()
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
    ck_memory testMem;
    memcpy(testMem.extend_data(sizeof(mem)).get_ptr(),mem,sizeof(mem));

    ck_string testString("blah blah blaskndadn");
    const ck_uuid_bits testUuid = {{0x10,0x12,0x82,0x99,0xe2,0x4a,0x34,0x4e,0x95,0x1a,0x11,0xed,0xbe,0xcc,0xc9,0x09}};

    {
        ck_variant a(testBool);
        bool test = false;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testBool);
    }

    {
        ck_variant a(testSignedChar);
        signed char test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testSignedChar);
    }

    {
        ck_variant a(testChar);
        char test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testChar);
    }

    {
        ck_variant a(testUnsignedChar);
        unsigned char test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testUnsignedChar);
    }

    {
        ck_variant a(testSignedShort);
        signed short test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testSignedShort);
    }

    {
        ck_variant a(testUnsignedShort);
        unsigned short test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testUnsignedShort);
    }

    {
        ck_variant a(testSignedInt);
        signed int test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testSignedInt);
    }

    {
        ck_variant a(testUnsignedInt);
        unsigned int test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testUnsignedInt);
    }

    {
        ck_variant a(testSignedLong);
        signed long test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testSignedLong);
    }

    {
        ck_variant a(testUnsignedLong);
        unsigned long test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testUnsignedLong);
    }

    {
        ck_variant a(testSignedLongLong);
        signed long long test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testSignedLongLong);
    }

    {
        ck_variant a(testUnsignedLongLong);
        unsigned long long test = 0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testUnsignedLongLong);
    }

    {
        ck_variant a(testFloat);
        float test = 0.0f;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testFloat);
    }

    {
        ck_variant a(testDouble);
        double test = 0.0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testDouble);
    }

    {
        ck_variant a(testLongDouble);
        long double test = 0.0;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(test == testLongDouble);
    }

    {
        ck_variant a(testString);
        ck_string test = a;
        UT_ASSERT(test == testString);
    }

    {
        ck_variant a(testMem);
        ck_memory test;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(memcmp(test.map().get_ptr(),testMem.map().get_ptr(),testMem.size_data()) == 0);
    }

    {
        ck_variant a(testUuid);
        ck_uuid_bits test;
        UT_ASSERT_NO_THROW(test = a);
        UT_ASSERT(memcmp(&test,&testUuid,sizeof(ck_uuid_bits)) == 0);
    }
}

void ck_variant_test::test_clear()
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
    ck_memory testMem;
    memcpy(testMem.extend_data(sizeof(mem)).get_ptr(),mem,sizeof(mem));

    ck_string testString("blah blah blaskndadn");
    const ck_uuid_bits testUuid = {{0x10,0x12,0x82,0x99,0xe2,0x4a,0x34,0x4e,0x95,0x1a,0x11,0xed,0xbe,0xcc,0xc9,0x09}};

    ck_variant a;

    UT_ASSERT(a.is_empty() == true);

    UT_ASSERT_NO_THROW(a = testBool);
    UT_ASSERT(a.get_type() == CK_VARTYPE_BOOL);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testSignedChar);
    UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_CHAR);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testChar);
    UT_ASSERT(a.get_type() == CK_VARTYPE_CHAR);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testUnsignedChar);
    UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_CHAR);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testSignedShort);
    UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_SHORT);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testUnsignedShort);
    UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_SHORT);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testUnsignedInt);
    UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_INT);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testSignedInt);
    UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_INT);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testSignedLong);
    UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_LONG);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testUnsignedLong);
    UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_LONG);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testSignedLongLong);
    UT_ASSERT(a.get_type() == CK_VARTYPE_SIGNED_LONG_LONG);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testUnsignedLongLong);
    UT_ASSERT(a.get_type() == CK_VARTYPE_UNSIGNED_LONG_LONG);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testMem);
    UT_ASSERT(a.get_type() == CK_VARTYPE_BYTES);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testString);
    UT_ASSERT(a.get_type() == CK_VARTYPE_TEXT);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testUuid);
    UT_ASSERT(a.get_type() == CK_VARTYPE_UUID);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testFloat);
    UT_ASSERT(a.get_type() == CK_VARTYPE_FLOAT);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testDouble);
    UT_ASSERT(a.get_type() == CK_VARTYPE_DOUBLE);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);

    UT_ASSERT_NO_THROW(a = testLongDouble);
    UT_ASSERT(a.get_type() == CK_VARTYPE_LONG_DOUBLE);
    UT_ASSERT_NO_THROW(a.clear());
    UT_ASSERT(a.is_empty() == true);
    UT_ASSERT(a.get_type() == CK_VARTYPE_EMPTY);
}
