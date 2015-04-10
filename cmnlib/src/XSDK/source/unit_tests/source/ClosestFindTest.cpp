
#include "ClosestFindTest.h"
#include "XSDK/OS/STL.h"
#include "XSDK/XString.h"

using namespace XSDK;
using namespace std;

REGISTER_TEST_FIXTURE(ClosestFindTest);

static const int COUNT = 100;

void ClosestFindTest::setup()
{
  _values.resize(COUNT);
  for ( int i =0; i < COUNT; ++i )
    _values[i] = (double)i;
}

void ClosestFindTest::teardown()
{
  _values.resize(0);
}

double ClosestFindTest::compare(const double& a, const double& b)
{
  return a-b;
}

void ClosestFindTest::TestFindExact()
{
    for ( int i = 0; i < COUNT; ++i )
      {
        int x = x_closest(_values,compare,(double)i,0,_values.size()-1,true);
          if (i != x)
            UT_ASSERT( false );
      }
    for ( int i = 0; i < COUNT; ++i )
      {
        int x = x_closest(_values,compare,(double)i,0,_values.size()-1,false);
        if ( i != x )
          UT_ASSERT( false );
      }
}

void ClosestFindTest::TestFindIntermediateValueInMiddle()
{
    for ( int i = 0; i < COUNT; ++i )
    {
        int x = x_closest(_values,compare,(double)(i+.5),0,_values.size()-1,true);
        int x2 = x_closest(_values,compare,(double)(i+.5),0,_values.size()-1,false);
        int x3 = x_closest(_values,compare,(double)(i-.5),0,_values.size()-1,true);
        int x4 = x_closest(_values,compare,(double)(i-.5),0,_values.size()-1,false);
        if ( i == COUNT-1)
        {
            if ( -1 != x )
                UT_ASSERT( false );
        }
        else
        {
            if( i+1 != x )
                UT_ASSERT( false  );
        }
        UT_ASSERT(i == x2);
        UT_ASSERT(i == x3);
        UT_ASSERT(i-1 == x4);
    }
}

void ClosestFindTest::TestFindIntermediateSmallDifference()
{
    for ( int i = 0; i < COUNT; ++i )
    {
        int x = x_closest(_values,compare,(double)(i+.3),0,_values.size()-1,true);
        int x2 = x_closest(_values,compare,(double)(i+.3),0,_values.size()-1,false);
        int x3 = x_closest(_values,compare,(double)(i-.3),0,_values.size()-1,true);
        int x4 = x_closest(_values,compare,(double)(i-.3),0,_values.size()-1,false);
        if ( i == COUNT-1)
            UT_ASSERT( -1 == x );
        else
            UT_ASSERT( i == x );
        UT_ASSERT(i == x2);
        UT_ASSERT(i == x3);
        if ( i == 0 )
            UT_ASSERT(-1 == x4);
        else
            UT_ASSERT(i == x4);

    }

}

void ClosestFindTest::TestFindIntermediateLargeDifference()
{
    for ( int i = 0; i < COUNT; ++i )
    {
        int x = x_closest(_values,compare,(double)(i+.7),0,_values.size()-1,true);
        int x2 = x_closest(_values,compare,(double)(i+.7),0,_values.size()-1,false);
        int x3 = x_closest(_values,compare,(double)(i-.7),0,_values.size()-1,true);
        int x4 = x_closest(_values,compare,(double)(i-.7),0,_values.size()-1,false);
        if ( i == COUNT-1)
            UT_ASSERT( -1 == x );
        else
            UT_ASSERT( i+1 == x );
        if ( i == COUNT-1)
            UT_ASSERT( i == x2 );
        else
            UT_ASSERT(i+1 == x2);
        if ( i == 0 )
            UT_ASSERT(0 == x3);
        else
            UT_ASSERT(i-1 == x3);
        UT_ASSERT(i-1 == x4);

    }
}

void ClosestFindTest::TestFindInSetExact()
{
    for ( int i = 0; i < COUNT; ++i )
      {
          int x = x_find_in_set(_values,compare,(double)i,0,_values.size()-1,true);
          if (i != x)
            UT_ASSERT( false );
      }
    for ( int i = 0; i < COUNT; ++i )
      {
        int x = x_find_in_set(_values,compare,(double)i,0,_values.size()-1,false);
        if ( i != x )
          UT_ASSERT( false );
      }
}

void ClosestFindTest::TestFindInSetIntermediate()
{
    for ( int i = 0; i < COUNT; ++i )
      {
        int x = x_find_in_set(_values,compare,(double)(i+.5),0,_values.size()-1,true);
        if ( i == COUNT-1)
          {
            if ( -1 != x )
              UT_ASSERT( false );
          }
        else
          {
            if( i+1 != x )
              UT_ASSERT( false  );
          }
      }
    int y = x_find_in_set(_values,compare,(double)(-.5),0,_values.size()-1,true);
    if ( y != 0 )
      UT_ASSERT( false  );
    for ( int i = 0; i < COUNT; ++i )
      {
        int x = x_find_in_set(_values,compare,(double)(i+.5),0,_values.size()-1,false);
        if( i != x )
          UT_ASSERT( false  );
      }
    y = x_find_in_set(_values,compare,(double)(-.5),0,_values.size()-1,false);
    if ( y != -1 )
      UT_ASSERT( false  );
}
