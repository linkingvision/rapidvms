
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_OS_STL_h
#define XSDK_OS_STL_h

#include "XSDK/XSharedLib.h"
#include <math.h>
#include <limits>

namespace XSDK
{
/// You probably think it's pretty strange to have our own version of find_if.
/// Well, I agree with you. Unfortunately, whatever STL version we have on the
/// XP build servers (XP only mind you, Vista, Windows 7 and Linux work fine)
/// has a subtly broken find_if implementation that causes a CommandQueue unit
/// test failure on VC9/XP. Oh, the joys of writing an OS portability layer.
template<typename _InputIterator, typename _Predicate>
inline _InputIterator x_find_if(_InputIterator __first, _InputIterator __last,
                                 _Predicate __pred )
{
    while (__first != __last && !bool(__pred(*__first)))
        ++__first;
    return __first;
}

template<class T, class U>
X_API int closest_find( const T& container,
                        double (*compare)( const U& objA, const U& objB ),
                        int lastIndex,
                        bool directionIsRight,
                        const U& targetVal,
                        int left,
                        int right,
                        int absLeft,
                        int absRight,
                        double& differential,
            bool preferGreaterValue)
{
    if( lastIndex != -1 )//Not the first run through
    {
        if( directionIsRight )//Heading towards end of set
        {
            const double cmp = ((*compare)( container[left], targetVal ));
            if( cmp > 0 )//left(first item in set) value is greater then target
            {
                differential = cmp;
                return left;
            }
        }
        else//!directionIsRight => Means heading left, heading twards the begining of set
        {
            const double cmp = ((*compare)( container[right], targetVal ));
            if( cmp < 0 )//right(Last item in set) value is less than target
            {
                differential = cmp;
                return right;
            }
        }
    }

    int midIndex = left + ((right-left)>>1);
    const U& midValue = container[midIndex];

    double cmp = ((*compare)( targetVal, midValue ));

    if( cmp < 0 )//Target Value is less than mid value
    {
        if( midIndex == absLeft )//Mid is first item in overall set
        {
            if (preferGreaterValue)
            {
                differential = cmp;
                return midIndex;
            }
            differential = 0;
            return -1;
        }
        double diff = 0;
        int result = closest_find( container, compare, midIndex, false, targetVal, left, (midIndex-1), absLeft, absRight, diff, preferGreaterValue );
        if ( fabs(diff) > fabs(cmp) )
        {
            differential = cmp;
            return midIndex;
        }
        if ( fabs(diff) < fabs(cmp) )
        {
            differential = diff;
            return result;
        }
        if (preferGreaterValue)
        {
            differential = cmp;
            return midIndex;
        }
        differential = diff;
        return result;
    }
    else if( cmp > 0 )//Target value is greater then mid value
    {
        if( midIndex == absRight )//mid is the last item in the overall set
        {
            if (!preferGreaterValue)
            {
                differential = cmp;
                return midIndex;
            }
            differential = 0;
            return -1;
        }
        double diff = 0;
        int result = closest_find( container, compare, midIndex, true, targetVal, (midIndex+1), right, absLeft, absRight, diff, preferGreaterValue );
        if ( fabs(diff) > fabs(cmp) )
        {
            differential = cmp;
            return midIndex;
        }
        if ( fabs(diff) < fabs(cmp) )
        {
            differential = diff;
            return result;
        }
        if (!preferGreaterValue)
        {
            differential = cmp;
            return midIndex;
        }
        differential = diff;
        return result;
    }//Target Value is exactly value
    else return midIndex;
}

/// Finds and returns the index of targetVal in the container T implementing
/// [], > and <. If targetVal is not contained in container, then closest<>()
/// returns the index of the first value in container that is greater than
/// the targetVal. The exception to this is that if the value is greater than
/// any in the set, it will return the right most index.
template<class T, class U>
X_API int x_closest( const T& container,
                     double (*compare)( const U& objA, const U& objB ),
                     const U& targetVal,
                     int left,
                     int right,
             bool preferGreaterValue )
{
  double diff = 1.e30;
  return closest_find( container, compare, -1, true, targetVal, left, right, left, right, diff, preferGreaterValue );
}

template<class T, class U>
X_API int find_in_set( const T& container,
                        double (*compare)( const U& objA, const U& objB ),
                        int lastIndex,
                        bool directionIsRight,
                        const U& targetVal,
                        int left,
                        int right,
                        int absLeft,
                        int absRight,
            bool preferGreaterValue)
{
    if( lastIndex != -1 )
    {
        if( directionIsRight )
        {
            const double cmp = ((*compare)( container[left], targetVal ));

            if( cmp > 0 )
          return (preferGreaterValue)?left:-1;
        }
        else//!directionIsRight => Means heading left
        {
            const double cmp = ((*compare)( container[right], targetVal ));

            if( cmp < 0 )
          return (!preferGreaterValue)?right:-1;
        }
    }

    int midIndex = left + ((right-left)>>1);
    const U& midValue = container[midIndex];

    double cmp = ((*compare)( targetVal, midValue ));

    if( cmp < 0 )
      {
    if( midIndex == absLeft )
      return (preferGreaterValue)?midIndex:-1;
    int result = find_in_set( container, compare, midIndex, false, targetVal, left, (midIndex-1), absLeft, absRight, preferGreaterValue );
    if (result < 0 && preferGreaterValue )
      result = midIndex;
    return result;
      }
    else if( cmp > 0 )
      {
    if( midIndex == absRight )
      return (!preferGreaterValue)?midIndex:-1;
    int result = find_in_set( container, compare, midIndex, true, targetVal, (midIndex+1), right, absLeft, absRight, preferGreaterValue );
    if ( result < 0 && !preferGreaterValue )
      result = midIndex;
    return result;
      }
    else return midIndex;
}

/// Finds and returns the index of targetVal in the container T implementing
/// [], > and <. If targetVal is not contained in container, then closest<>()
/// returns the index of the first value in container that is greater than
/// the targetVal. The exception to this is that if the value is greater than
/// any in the set, it will return the right most index.
template<class T, class U>
X_API int x_find_in_set( const T& container,
                     double (*compare)( const U& objA, const U& objB ),
                     const U& targetVal,
                     int left,
                     int right,
             bool preferGreaterValue )
{
  return find_in_set( container, compare, -1, true, targetVal, left, right, left, right, preferGreaterValue );
}


}
#endif
