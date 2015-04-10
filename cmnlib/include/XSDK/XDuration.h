
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XDuration_H_
#define _XDuration_H_

#include "XSDK/XException.h"
#include "XSDK/Types.h"
#include "XSDK/XString.h"
#include <climits>
#include <ctime>

// To make timeval work on Windows
#ifdef WIN32
    #include <WinSock2.h>
#endif


class XFracSecTest;
class XDurationTest;

namespace XSDK
{

class XDuration;
class XTime;

///Represents units of time.
enum TimeUnits { NSECS = 0, HNSECS, USECS, MSECS, SECONDS, MINUTES, HOURS, DAYS, WEEKS };

X_API inline int64_t convert(TimeUnits from, TimeUnits to, int64_t value);

//These functions are used internally by the time stuff but probably should never be
//used by stuff outside it.
X_API inline int64_t hnsecsPer(TimeUnits units);
X_API inline int64_t splitUnitsFromHNSecs(TimeUnits units, int64_t& hnsecs);
X_API inline int64_t getUnitsFromHNSecs(TimeUnits units, int64_t hnsecs);
X_API inline int64_t removeUnitsFromHNSecs(TimeUnits units, int64_t hnsecs);


/**
    \brief Represents fractional seconds.

    This is the portion of the time which is smaller than a second and cannot
    hold values which would equal or exceed a second.

    It holds hnsecs (100 ns) internally, but you can create it using either milliseconds,
    microseconds, hnsecs, or nsecs. What it does is allow for a simple way to set or
    adjust the fractional seconds portion of a XDuration or an XTime without
    having to worry about whether you're dealing with milliseconds, microseconds,
    hnsecs, or nsecs.

    XFracSec's functions which take time unit strings do accept NESCS, but the
    because the resolution for XDuration and XTIME is HNSECS, you
    don't actually get precision higher than HNSECS. NSECS is accepted merely
    for convenience. Any values given as NSECS will be converted to HNSECS using
    convert (which uses truncation when converting to smaller units).
  */
class XFracSec
{
    friend class ::XFracSecTest;
    friend class XDuration;
    friend class XTime;

public:

    /// \brief Easy way to get an XFracSec of 0.
    X_API static inline const XFracSec Zero() {return XFracSec(0); }

    /**
        \brief Create a XFracSec from the given units (MSECS, USECS, HNSECS, or NSECS).

        The internal precision is HNSECS (100 ns).

        \param units The units to create a XFracSec from.
        \param value number of the given units passed the second. Must be > 0 && < 1 second.
      */
    X_API XFracSec(TimeUnits units, int64_t value)
        : _hnsecs((int)convert(units, HNSECS, value))
    {
        if(!_Valid(_hnsecs))
            X_STHROW(XException, (XSDK::XString::Format("Attempted to create XFracSec with invalid number of hnsecs: %I64d", value)));
    }


    ///
    X_API virtual ~XFracSec() throw() {}


    ///
    X_API bool operator==(const XFracSec& rhs) const { return this->_hnsecs == rhs._hnsecs; }

    ///
    X_API bool operator!=(const XFracSec& rhs) const { return this->_hnsecs != rhs._hnsecs; }

    ///
    X_API bool operator<(const XFracSec& rhs) const { return this->_hnsecs < rhs._hnsecs; }

    ///
    X_API bool operator<=(const XFracSec& rhs) const { return this->_hnsecs <= rhs._hnsecs; }

    ///
    X_API bool operator>(const XFracSec& rhs) const { return this->_hnsecs > rhs._hnsecs; }

    ///
    X_API bool operator>=(const XFracSec& rhs) const { return this->_hnsecs >= rhs._hnsecs; }

    ///
    X_API XFracSec operator-() const { return XFracSec(-_hnsecs); }


    /**
        \brief The value of this XFracSec as milliseconds.
      */
    X_API int MSecs() const
    {
        return (int)convert(HNSECS, MSECS, _hnsecs);
    }


    /**
        \brief The value of this XFracSec as milliseconds.

        \param milliseconds The number of milliseconds passed the second.
      */
    X_API void MSecs(int milliseconds)
    {
        const int hnsecs = (int)convert(MSECS, HNSECS, milliseconds);

        if(!_Valid(hnsecs))
            X_STHROW(XException, (XSDK::XString::Format("Attempted to set XFracSec to invalid number of msecs: %d", milliseconds)));

        _hnsecs = hnsecs;
    }


    /**
        \brief The value of this XFracSec as microseconds.
      */
    X_API int USecs() const
    {
        return (int)convert(HNSECS, USECS, _hnsecs);
    }


    /**
        \brief The value of this XFracSec as microseconds.

        /param microseconds The number of microseconds passed the second.
      */
    X_API void USecs(int microseconds)
    {
        const int hnsecs = (int)convert(USECS, HNSECS, microseconds);

        if(!_Valid(hnsecs))
            X_STHROW(XException, (XSDK::XString::Format("Attempted to set XFracSec to invalid number of usecs: %d", microseconds)));

        _hnsecs = hnsecs;
    }


    /**
        \brief The value of this XFracSec as hnsecs.
      */
    X_API int HNSecs() const
    {
        return _hnsecs;
    }


    /**
        \brief The value of this XFracSec as hnsecs.

        \param hnsecs The number of hnsecs passed the second.
      */
    X_API void HNSecs(int hnsecs)
    {
        if(!_Valid(hnsecs))
            X_STHROW(XException, (XSDK::XString::Format("Attempted to set XFracSec to invalid number of hnsecs: %I64d", hnsecs)));

        _hnsecs = hnsecs;
    }


    /**
        \brief The value of this XFracSec as nsecs.

        \note This does not give you any greater precision
              than getting the value of this XFracSec as hnsecs.
      */
    X_API int64_t NSecs() const
    {
        return convert(HNSECS, NSECS, _hnsecs);
    }


    /**
        \brief The value of this XFracSec as nsecs.

        \note This does not give you any greater precision
              than setting the value of this XFracSec as hnsecs.

        \param nsecs The number of nsecs passed the second.
      */
    X_API void NSecs(int64_t nsecs)
    {
        const int hnsecs = (int)convert(NSECS, HNSECS, nsecs);

        if(!_Valid(hnsecs))
            X_STHROW(XException, (XSDK::XString::Format("Attempted to set XFracSec to invalid number of nsecs: %I64d", nsecs)));

        _hnsecs = hnsecs;
    }


    /**
        \brief Returns whether this XFracSec is negative.
      */
    X_API bool IsNegative() const
    {
        return _hnsecs < 0;
    }


    /**
        \brief Returns whether this XFracSec is zero.
      */
    X_API bool IsZero() const
    {
        return _hnsecs == 0;
    }


private:

    ///Returns the fractional portion of the time in the ISO format.
    X_API static XString ToISOString(int64_t hnsecs);

    ///Returns the fractional portion of the time in the MPEG-7 format.
    X_API static XString ToMPEG7String(int64_t hnsecs);

    ///Returns the fractional portion of the time in the ISO format.
    X_API static XFracSec FromISOString(const XString& isoString);

    ///Returns the fractional portion of the time in the MPEG-7 format.
    X_API static XFracSec FromMPEG7String(const XString& isoString);

    /**
        Returns whether the given number of hnsecs fits within the range of XFracSec.

        Params:
            hnsecs = The number of hnsecs.
      */
    static bool _Valid(int hnsecs)
    {
        const int64_t absHNSecs = abs(hnsecs);
        return absHNSecs >= 0 && absHNSecs < convert(SECONDS, HNSECS, 1);
    }


    /**
        \param hnsecs The number of hnsecs passed the second.
      */
    XFracSec(int hnsecs)
        : _hnsecs(hnsecs)
    {
        X_ASSERT(_Valid(hnsecs));
    }


    int _hnsecs;
};



/**
    \brief Represents a duration of time of weeks or less (kept internally as hnsecs).
           (e.g. 22 days or 700 seconds).

    It is used when representing a duration of time. One example would be XTime
    using it to represent the difference between two XTimes.
    sleep with Thread.sleep().

    Use the dur function to create XDurations.

\code
//Examples
assert(XDuration(DAYS, 12) == XDuration(10368000000000LL));
assert(XDuration(HNSECS, 27) == XDuration(27));
\endcode
 */
class XDuration
{
    friend class ::XDurationTest;
    friend class XTime;

public:

    /// \brief Easy way to get an XDuration of 0.
    X_API static inline const XDuration Zero() { return XDuration(0); }

    ///
    X_API static inline const XDuration Min()
    {
#ifdef IS_64BIT
        return XDuration(LLONG_MIN);
#elif defined(IS_32BIT)
        return XDuration(LONG_MIN);
#else
    #error ">> Unknown Architecture!"
#endif
    }

    ///
    X_API static inline const XDuration Max()
    {
#ifdef IS_64BIT
        return XDuration(LLONG_MAX);
#elif defined(IS_32BIT)
        return XDuration(LONG_MAX);
#else
    #error ">> Unknown Architecture!"
#endif
    }


    /**
        \brief Constructs a default XDuration of 0.
     */
    X_API XDuration()
        : _hnsecs(0)
    {}

    /**
        \brief Constructs an XDuration from the given time units with the
               given length.

        \param units The time units of the duration (e.g. SECONDS).
        \param length The number of units in the duration.
      */
    X_API XDuration(TimeUnits units, int64_t length)
        : _hnsecs(convert(units, HNSECS, length))
    {}


    ///
    X_API virtual ~XDuration() throw() {}

    ///
    X_API bool operator==(const XDuration& rhs) const { return this->_hnsecs == rhs._hnsecs; }

    ///
    X_API bool operator!=(const XDuration& rhs) const { return this->_hnsecs != rhs._hnsecs; }

    ///
    X_API bool operator<(const XDuration& rhs) const { return this->_hnsecs < rhs._hnsecs; }

    ///
    X_API bool operator<=(const XDuration& rhs) const { return this->_hnsecs <= rhs._hnsecs; }

    ///
    X_API bool operator>(const XDuration& rhs) const { return this->_hnsecs > rhs._hnsecs; }

    ///
    X_API bool operator>=(const XDuration& rhs) const { return this->_hnsecs >= rhs._hnsecs; }

    ///
    X_API XDuration operator+(const XDuration& rhs) const { return XDuration(_hnsecs + rhs._hnsecs); }

    ///
    X_API XDuration operator-(const XDuration& rhs) const { return XDuration(_hnsecs - rhs._hnsecs); }

    ///
    X_API XDuration operator+=(const XDuration& rhs)
    {
        _hnsecs += rhs._hnsecs;

        return *this;
    }

    ///
    X_API XDuration operator-=(const XDuration& rhs)
    {
        _hnsecs -= rhs._hnsecs;

        return *this;
    }

    ///
    X_API XDuration operator*(int64_t value) const { return XDuration(_hnsecs * value); }

    ///
    X_API XDuration operator/(int64_t value) const
    {
        X_ASSERT(value != 0);

        return XDuration(_hnsecs / value);
    }

    ///
    X_API int64_t operator/(XDuration rhs) const
    {
        X_ASSERT(rhs._hnsecs != 0);

        return _hnsecs / rhs._hnsecs;
    }

    ///
    X_API XDuration operator*=(int64_t value)
    {
        _hnsecs *= value;

        return *this;
    }

    ///
    X_API XDuration operator/=(int64_t value)
    {
        X_ASSERT(value != 0);
        _hnsecs /= value;

        return *this;
    }

    ///
    X_API XDuration operator-() const { return XDuration(-_hnsecs); }


    /**
        \brief Returns the number of the given units in the duration
               (minus the larger units).

        \param The units to get (must be SECONDS or greater).

        \note Use XFracSec to get the fractional seconds.

\code
//Examples
assert(XDuration(WEEKS, 12).GetOnly(WEEKS) == 12);
assert(XDuration(WEEKS, 12).GetOnly(DAYS) == 0);

assert(XDuration(DAYS, 13).GetOnly(WEEKS) == 1);
assert(XDuration(DAYS, 13).GetOnly(DAYS) == 6);

assert(XDuration(HOURS, 49).GetOnly(DAYS) == 2);
assert(XDuration(HOURS, 49).GetOnly(HOURS) == 1);

assert(XDuration(HOURS, 25).GetOnly(HOURS) == 1);
assert(XDuration(HOURS, 25).GetOnly(SECONDS) == 0);

assert(XDuration(MSECS, 1002).GetOnly(SECONDS) == 1);
\endcode
      */
    X_API int64_t GetOnly(TimeUnits units) const
    {
        X_ASSERT(units >= SECONDS && units <= WEEKS);

        if(units == WEEKS)
            return getUnitsFromHNSecs(WEEKS, _hnsecs);
        else
        {
            const int64_t hnsecs = removeUnitsFromHNSecs((TimeUnits)(units + 1), _hnsecs);

            return getUnitsFromHNSecs(units, hnsecs);
        }
    }


    /**
        \brief Returns the fractional seconds passed the second.

\code
//Examples
assert(XDuration(MSECS, 1000).XFracSec() == XFracSec(MSECS, 0));
assert(XDuration(MSECS, 1217).XFracSec() == XFracSec(MSECS, 217));
assert(XDuration(USECS, 43).XFracSec() == XFracSec(USECS, 43));
assert(XDuration(HNSECS, 50_007).XFracSec() == XFracSec(HNSECS, 50_007));
\endcode
     */
    X_API XFracSec FracSec() const
    {
        int64_t hnsecs = removeUnitsFromHNSecs(SECONDS, _hnsecs);

        return XFracSec((int)hnsecs);
    }


    /**
        Returns the total number of the given units in the duration.
        So, unlike get, it does not strip out the larger units.

\code
//Examples
assert(XDuration(WEEKS, 12).Total(WEEKS) == 12);
assert(XDuration(WEEKS, 12).Total(DAYS) == 84);

assert(XDuration(DAYS, 13).Total(WEEKS) == 1);
assert(XDuration(DAYS, 13).Total(DAYS) == 13);

assert(XDuration(HOURS, 49).Total(DAYS) == 2);
assert(XDuration(HOURS, 49).Total(HOURS) == 49);

assert(XDuration(HOURS, 25).Total(HOURS) == 25);
assert(XDuration(HOURS, 25).Total(SECONDS) == 90000);

assert(XDuration(MSECS, 1002).Total(SECONDS) == 1);
assert(XDuration(MSECS, 1002).Total(MSECS) == 1002);

assert(XDuration(NSECS, 2007).Total(HNSECS) == 20);
assert(XDuration(NSECS, 2007).Total(NSECS) == 2000);
\endcode
      */
    X_API int64_t Total(TimeUnits units) const { return convert(HNSECS, units, _hnsecs); }

    ///
    X_API timeval ToTimeVal() const { timeval retval = { (long)Total(SECONDS), (long)FracSec().USecs() }; return retval; }


    /// Returns whether the length of this XDuration is negative.
    X_API bool IsNegative() const { return _hnsecs < 0; }


    /// Returns whether the length of this XDuration is zero.
    X_API bool IsZero() const { return _hnsecs == 0; }

    X_API XString ToString() const;

private:


    /// \param hnsecs The total number of hecto-nanoseconds in this duration.
    XDuration(int64_t hnsecs)
        : _hnsecs(hnsecs)
    {}


    int64_t _hnsecs;
};


/**
\brief Generic way of converting between two time units. Conversions to
       smaller units use truncating division.

Params:
    \param tuFrom The units of time to covert from.
    \param tuFrom The units of time to covert type.
    \param value  The value to convert.

\code
//Examples
assert(convert(WEEKS, DAYS, 1) == 7);
assert(convert(HOURS, SECONDS, 1) == 3600);
assert(convert(SECONDS, DAYS, 1) == 0);
assert(convert(SECONDS, DAYS, 86_400) == 1);

assert(convert(NSECS, NSECS, 1) == 1);
assert(convert(NSECS, HNSECS, 1) == 0);
assert(convert(HNSECS, NSECS, 1) == 100);
assert(convert(NSECS, SECONDS, 1) == 0);
assert(convert(SECONDS, NSECS, 1) == 1_000_000_000);
\endcode
*/
X_API int64_t convert(TimeUnits from, TimeUnits to, int64_t value)
{
    X_ASSERT(from >= NSECS && from <= WEEKS);
    X_ASSERT(to >= NSECS && to <= WEEKS);

    if(from > NSECS && to > NSECS)
        return (hnsecsPer(from) * value) / hnsecsPer(to);
    else
    {
        if(from == NSECS && to == NSECS)
            return value;
        else if(from == NSECS)
            return convert(HNSECS, to, value / 100);
        else
            return convert(from, HNSECS, value) * 100;
    }
}


///Returns the number of hnsecs (100 ns) in the given TimeUnits.
X_API inline int64_t hnsecsPer(TimeUnits units)
{
    X_ASSERT(units > NSECS && units <= WEEKS);

    switch(units)
    {
        case HNSECS:
            return 1LL;
        case USECS:
            return 10LL;
        case MSECS:
            return 10000LL;
        case SECONDS:
            return 10000000LL;
        case MINUTES:
            return 60 * 10000000LL;
        case HOURS:
            return 60 * 60 * 10000000LL;
        case DAYS:
            return 24 * 60 * 60 * 10000000LL;
        case WEEKS:
            return 7 * 24 * 60 * 60 * 10000000LL;
        default:
            X_ASSERT(0 && "Invalid case.");
            return 0;
    }
}

X_API inline int64_t splitUnitsFromHNSecs(TimeUnits units, int64_t& hnsecs)
{
    X_ASSERT(units >= HNSECS && units <= WEEKS);

    const int64_t value = convert(HNSECS, units, hnsecs);
    hnsecs -= convert(units, HNSECS, value);

    return value;
}


X_API inline int64_t getUnitsFromHNSecs(TimeUnits units, int64_t hnsecs)
{
    X_ASSERT(units >= HNSECS && units <= WEEKS);

    return convert(HNSECS, units, hnsecs);
}


X_API inline int64_t removeUnitsFromHNSecs(TimeUnits units, int64_t hnsecs)
{
    X_ASSERT(units >= HNSECS && units <= WEEKS);

    const int64_t value = convert(HNSECS, units, hnsecs);

    return hnsecs - convert(units, HNSECS, value);
}

}

#endif
