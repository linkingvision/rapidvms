
#include "XFullVariantTest.h"
#include "XSDK/XFullVariant.h"

using namespace XSDK;

REGISTER_TEST_FIXTURE(VariantTest);

void VariantTest::setup()
{
}

void VariantTest::teardown()
{
}

void VariantTest::TestDefaultConstructor()
{
    XFullVariant v;
    // Should throw...
    UT_ASSERT_THROWS(v.GetSignedInteger(), XException);
}

void VariantTest::TestCopyConstructor()
{
    try
    {
        {
            XFullVariant v1;
            const int val1 = 42;
            v1.SetSignedInteger( val1 );

            const XFullVariant v2 = v1;
            const int val2 = v2.GetSignedInteger();

            UT_ASSERT( val1 == val2 );
        }

        {
            XFullVariant v1;
            const unsigned int val1 = 42;
            v1.SetUnsignedInteger( val1 );

            const XFullVariant v2 = v1;
            const unsigned int val2 = v2.GetUnsignedInteger();

            UT_ASSERT( val1 == val2 );
        }

        {
            XFullVariant v1;
            const double val1 = 42.42;
            v1.SetFloatingPoint( val1 );

            XFullVariant v2 = v1;
            const double val2 = v2.GetFloatingPoint();

            UT_ASSERT( val1 == val2 );
        }

        {
            XFullVariant v1;
            unsigned char bytes1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            v1.SetBytes( bytes1, 10 );

            XFullVariant v2 = v1;
            unsigned char* bytes2 = NULL;
            const size_t bytes2Size = v2.GetBytes( &bytes2);

            UT_ASSERT( bytes2Size == 10 );

            UT_ASSERT( bytes1 != bytes2 );

            UT_ASSERT( memcmp( bytes1, bytes2, 10 ) == 0 );
        }

        {
            XFullVariant v1;
            const XString val1 = "42";
            v1.SetString( val1 );

            XFullVariant v2 = v1;
            const XString val2 = v2.GetString();

            UT_ASSERT( val1 == val2 );
            UT_ASSERT( strcmp( val2.c_str(), "42" ) == 0 );
        }
    }
    catch( XSDK::XException& ex )
    {
        printf("%s %d %s\n",ex.GetSrcFile(),ex.GetLineNum(),ex.GetMsg());
        throw ex;
    }
}

void VariantTest::TestAssignmentOperator()
{
    {
        XFullVariant v1;
        const int val1 = 42;
        v1.SetSignedInteger( val1 );

        XFullVariant v2;
        v2 = v1; // assignment
        const int val2 = v2.GetSignedInteger();

        UT_ASSERT( val1 == val2 );
    }

    {
        XFullVariant v1;
        const unsigned int val1 = 42;
        v1.SetUnsignedInteger( val1 );

        XFullVariant v2;
        v2 = v1; // assignment
        unsigned int val2 = v2.GetUnsignedInteger();

        UT_ASSERT( val1 == val2 );
    }

    {
        XFullVariant v1;
        const double val1 = 42.42;
        v1.SetFloatingPoint( val1 );

        XFullVariant v2;
        v2 = v1; // assignment
        double val2 = v2.GetFloatingPoint();

        UT_ASSERT( val1 == val2 );
    }

    {
        XFullVariant v1;
        unsigned char bytes1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        v1.SetBytes( bytes1, 10 );

        XFullVariant v2;
        v2 = v1; // assignment
        unsigned char* bytes2 = NULL;
        const size_t bytes2Size = v2.GetBytes( &bytes2);

        UT_ASSERT( bytes2Size == 10 );

        UT_ASSERT( bytes1 != bytes2 );

        UT_ASSERT( memcmp( bytes1, bytes2, 10 ) == 0 );
    }

    {
        XFullVariant v1;
        const XString val1 = "42";
        v1.SetString( val1 );

        XFullVariant v2;
        v2 = v1; // assignment
        const XString val2 = v2.GetString();

        UT_ASSERT( val1 == val2 );
        UT_ASSERT( strcmp( val2.c_str(), "42" ) == 0 );
    }
}

void VariantTest::TestGetType()
{
        {
            XFullVariant v1;
            const int val1 = 42;
            v1.SetSignedInteger( val1 );

            UT_ASSERT( v1.GetType() == XFullVariant::SIGNED_INTEGER );
        }

        {
            XFullVariant v1;
            const unsigned int val1 = 42;
            v1.SetUnsignedInteger( val1 );

            UT_ASSERT( v1.GetType() == XFullVariant::UNSIGNED_INTEGER );
        }

        {
            XFullVariant v1;
            const double val1 = 42.42;
            v1.SetFloatingPoint( val1 );

            UT_ASSERT( v1.GetType() == XFullVariant::FLOATING_POINT );
        }

        {
            XFullVariant v1;
            unsigned char bytes1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            v1.SetBytes( bytes1, 10 );

            UT_ASSERT( v1.GetType() == XFullVariant::BYTES );
        }

        {
            XFullVariant v1;
            const XString val1 = "42";
            v1.SetString( val1 );

            UT_ASSERT( v1.GetType() == XFullVariant::TEXT );
        }
}

void VariantTest::TestClear()
{
    XFullVariant v;
    const int val = 42;

    v.SetSignedInteger( val );

    v.Clear();

    // Should throw...
    UT_ASSERT_THROWS(v.GetSignedInteger(), XException);
}

void VariantTest::TestVariant()
{
  {
    XFullVariant v;

    v.SetSignedInteger( -42 );

    const int val = v.GetSignedInteger();

    UT_ASSERT( val == -42 );
  }

  {
    XFullVariant v;

    v.SetSignedInteger( -42 );

    const int val = v.GetSignedInteger();

    UT_ASSERT( val == -42 );
  }

  {
    XFullVariant v;

    v.SetFloatingPoint( .42 );

    const double val = v.GetFloatingPoint();

    UT_ASSERT( val == .42 );
  }

  {
    unsigned char data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    XFullVariant v;

    v.SetBytes( data, 10 );

    unsigned char* bytes = NULL;

    const size_t size = v.GetBytes(&bytes);

    UT_ASSERT( bytes != NULL );

    UT_ASSERT( size == 10 );

    UT_ASSERT( memcmp( data, bytes, 10 ) == 0 );
  }

  {
    XFullVariant v;

    const XString input = "42";

    v.SetString( input );

    const XString output = v.GetString();

    UT_ASSERT( output == input );

    UT_ASSERT( output == "42" );
  }

  {
    unsigned char data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    XFullVariant v;

    v.SetBytes( data, 10 );

    v.Clear();
  }

  {
    unsigned char data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    XFullVariant v;

    v.SetBytes( data, 10 );

    XFullVariant v2 = v;

    unsigned char* bytes = NULL;

    const size_t size = v2.GetBytes(&bytes);

    UT_ASSERT( bytes != NULL );

    UT_ASSERT( size == 10 );

    UT_ASSERT( memcmp( bytes, data, 10 ) == 0 );
  }

}

void VariantTest::TestVariant64()
{
  {
    XFullVariant v;

    v.SetSigned64Integer( -10000000000ll );

    int64_t val = v.GetSigned64Integer();

    UT_ASSERT( val == -10000000000ll );
  }

  {
    XFullVariant v;

    v.SetUnsigned64Integer( 10000000000ull );

    uint64_t val = v.GetUnsigned64Integer();

    UT_ASSERT( val == 10000000000ull );
  }
}
