
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XTime_H_
#define _XTime_H_

#include <climits>
#include <ctime>

#ifndef WIN32
#include <sys/time.h>
#else
#include <winsock2.h>
#endif

#include "XSDK/Errors.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/Types.h"
#include "XSDK/XIRef.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XDateTuple.h"
#include "XSDK/XDuration.h"
#include "XSDK/XTimeZone.h"


class XTimeTest;

namespace XSDK
{

/**
    @brief Class for representing the time. It holds the time internally in
           hecto-nanosecond (100 ns) ticks from midnight, Jaunary 1st 1 A.D.

    Conversions are done internally using the time zone of an XTime when a function
    needs the time in the XTime's time zone rather than UTC. That way, the time is
    always kept internally in UTC and there are no conversion errors.

    @todo Implement functions for getting the year, month, day, hour, etc.
  */
class XTime
{
    friend class ::XTimeTest;
public:

    /// Default constructor, used when you have an XTime member of an object, or for testing.
    X_API XTime() : _ticks(0), _tz(XLocalTime::Instance()) {}

    /**
        \param ticks The number of hecto-nanosecond (100 ns) for midnight, January 1st, 1 A.D. in UTC.
        \param tz The time zone that will be used for this XTime.
      */
    explicit X_API XTime(int64_t ticks, XIRef<XTimeZone> tz = XLocalTime::Instance())
        : _ticks(ticks),
          _tz(tz)
    {}


    /**
        \param The date.
        \param The time of day.
        \param tz The time zone that will be used for this XTime.

        \note The given date and time is assumed to be in the given time zone.
      */
    explicit X_API XTime(const XDateTuple& date,
                         const XTimeTuple& time,
                         XIRef<XTimeZone> tz = XLocalTime::Instance());


    /**
        \param The date.
        \param The time of day.
        \param fracSec The fractional seconds portion of the time.
        \param tz The time zone that will be used for this XTime.

        \note The given date and time is assumed to be in the given time zone.
      */
    explicit X_API XTime(const XDateTuple& date,
                         const XTimeTuple& time,
                         const XFracSec& fracSec,
                         XIRef<XTimeZone> tz = XLocalTime::Instance());


    /**
        \param unixTime The number of seconds from midnight, January 1st, 1970.
        \param fracSec The fractional seconds portion of the time.
        \param tz The time zone that will be used for this XTime.
      */
    explicit X_API XTime(time_t unixTime,
                         const XFracSec& fracSec,
                         XIRef<XTimeZone> tz = XLocalTime::Instance());


    /**
        \param tv Holds the time in UTC, including fractional seconds.
        \param tz The time zone that will be used for this XTime.
      */
    explicit X_API XTime(const timeval& tv, XIRef<XTimeZone> tz = XLocalTime::Instance())
        : _ticks(unixTimeToTicks(tv.tv_sec) + convert(USECS, HNSECS, tv.tv_usec)),
          _tz(tz)
    {}


    /**
        \brief Factory method for constructing on XTime when you have unix time in milliseconds.

        \param unixTimeAsMSecs Same as time_t except that it's in milliseconds.
        \param tz The time zone that will be used for this XTime.
      */
    static X_API XTime CreateFromUnixTimeAsMSecs(int64_t unixTimeAsMSecs, XIRef<XTimeZone> tz = XLocalTime::Instance())
    {
        return XTime(unixTimeEpochToTicks(convert(MSECS, HNSECS, unixTimeAsMSecs)), tz);
    }


    ///Destructor
    X_API virtual ~XTime() {}


    /**
     *@name Logical and Arithmatic Operators
     * Performs a comparison on the raw ticks only
     *@{
     */
    ///Eqality operator
    X_API bool operator==(const XTime& rhs) const { return this->_ticks == rhs._ticks; }
    ///Not Equals operator
    X_API bool operator!=(const XTime& rhs) const { return this->_ticks != rhs._ticks; }
    ///Less than operator
    X_API bool operator<(const XTime& rhs) const { return this->_ticks < rhs._ticks; }
    ///less than or equal to operator
    X_API bool operator<=(const XTime& rhs) const { return this->_ticks <= rhs._ticks; }
    ///Greater than operator
    X_API bool operator>(const XTime& rhs) const { return this->_ticks > rhs._ticks; }
    ///greater than or equal to operator
    X_API bool operator>=(const XTime& rhs) const { return this->_ticks >= rhs._ticks; }
    ///Minus operator
    X_API XDuration operator-(const XTime& rhs) const { return XDuration(this->_ticks - rhs._ticks); }
    ///Minus Equals operator
    X_API XTime& operator-=(const XTime& rhs)
    {
        this->_ticks -= rhs._ticks;
        return *this;
    }
    ///@}


    /**
     *@name Duration logical & Arithmatic operators
     *Perofrms work on the time using the duration
     *@{
     */
    ///Plus Operator
    X_API XTime operator+(const XDuration& rhs) const { return XTime(this->_ticks + rhs._hnsecs, _tz); }
    ///Minus operator
    X_API XTime operator-(const XDuration& rhs) const { return XTime(this->_ticks - rhs._hnsecs, _tz); }
    ///Plus Equals operator
    X_API XTime& operator+=(const XDuration& rhs)
    {
        this->_ticks += rhs._hnsecs;
        return *this;
    }
    ///Minus Equals operator
    X_API XTime& operator-=(const XDuration& rhs)
    {
        this->_ticks -= rhs._hnsecs;
        return *this;
    }
    ///@}


    /**
    *@name Setters
    * These methods will overwrite a XTime with the value passed
    *@{
    */
    X_API void Set(int64_t ticks, XIRef<XTimeZone> tz = XLocalTime::Instance())
    {
        _ticks = ticks;
        _tz = tz;
    }
    X_API void Set(time_t unixTime, XFracSec fracSec, XIRef<XTimeZone> tz = XLocalTime::Instance())
    {
        if(fracSec.IsNegative())
            X_STHROW(XException, ("Attempted to set an XTime's XFracSec component to a negative value."));
        _ticks = unixTimeToTicks(unixTime) + fracSec.HNSecs();
        _tz = tz;
    }
    X_API void Set(const timeval& tv, XIRef<XTimeZone> tz = XLocalTime::Instance())
    {
        _ticks = unixTimeToTicks(tv.tv_sec) + convert(USECS, HNSECS, tv.tv_usec);
        _tz = tz;
    }
    ///@}

    ///
    X_API int Year() const { return ToXDateTuple().Year(); }

    ///
    X_API void SetYear(int value);

    ///
    X_API int Month() const { return ToXDateTuple().Month(); }

    ///
    X_API void SetMonth(int value);

    ///
    X_API int Day() const { return ToXDateTuple().Day(); }

    ///
    X_API void SetDay(int value);

    ///
    X_API int Hour() const { return ToXTimeTuple().Hour(); }

    ///
    X_API void SetHour(int value);

    ///
    X_API int Minute() const { return ToXTimeTuple().Minute(); }

    ///
    X_API void SetMinute(int value);

    ///
    X_API int Second() const { return ToXTimeTuple().Second(); }

    ///
    X_API void SetSecond(int value);

    /// \brief Returns the fractional seconds portion of this XTime.
    X_API XFracSec FracSec() const
    {
        const int64_t hnsecs = removeUnitsFromHNSecs(SECONDS, AdjTime());

        return XFracSec((int)hnsecs);
    }

    ///
    X_API void SetFracSec(const XFracSec& value);

    /// \brief Returns the time zone for this XTime.
    X_API XIRef<XTimeZone> TimeZone() const { return _tz; }

    /// \brief Sets the time zone for this XTime.
    X_API void SetTimeZone(XIRef<XTimeZone> tz) { _tz = tz; }

    /// \brief Whether DST is in effect for this XTime.
    X_API bool DSTInEffect() const { return _tz->DSTInEffect(_ticks); }

    /// \brief The total UTC offset for this XTime (includes adjustment for DST).
    X_API XDuration UTCOffset() const { return _tz->UTCOffsetAt(_ticks); }

    /// \brief Returns a XTime with the same ticks as this one, but with XLocalTime as its time zone.
    X_API XTime ToLocalTime() const { return XTime(_ticks, XLocalTime::Instance()); }

    /// \brief Returns a XTime with the same ticks as this one, but with XUTC as its time zone.
    X_API XTime ToUTC() const { return XTime(_ticks, XUTC::Instance()); }

    /// \brief Returns a XTime with the same std time as this one, but with given time zone as its time zone.
    X_API XTime ToOtherTZ(XIRef<XTimeZone> tz) const { return XTime(_ticks, tz); }

    /// \brief Returns a time_t which represents this XTime.
    X_API time_t ToUnixTime() const { return ticksToUnixTime(_ticks); }

    /// \brief Same as ToUnixTime except that it's in milliseconds instead of seconds.
    X_API int64_t ToUnixTimeAsMSecs() const
    {
        return convert(HNSECS, MSECS, ticksToUnixTimeEpoch(_ticks));
    }

    /// \brief Returns a timeval which represents this XTime.
    X_API timeval ToTimeVal() const
    {
        const int64_t hnsecs = removeUnitsFromHNSecs(SECONDS, _ticks);

#ifdef IS_LINUX
        timeval tv = { (time_t)ticksToUnixTime(_ticks), (int)convert(HNSECS, USECS, hnsecs) };
#elif defined(IS_WINDOWS)
        //Why can't Windows have just been sane and defined tv_sec to be a time_t instead of long...
        timeval tv = { (long)ticksToUnixTime(_ticks), (int)convert(HNSECS, USECS, hnsecs) };
#else
    #error ">> Unknown OS!"
#endif

        return tv;
    }

    /// Returns the year, month, and day of this XTime as an XDateTuple.
    X_API XDateTuple ToXDateTuple() const;

    /// Returns the hour, minute, and second of this XTime as an XTimeTuple.
    X_API XTimeTuple ToXTimeTuple() const;

    /// Returns the number of hecto-nanoseconds since midnight, January 1st, 1 A.D. in UTC.
    X_API int64_t Ticks() const { return _ticks; }

    /// \brief Returns the date and time in the ISO Extended Format: "YYYY-MM-DDTHH:MMSS.FFFFFFFTZ"
    ///        (where F is fracional seconds and TZ is the time zone).
    ///
    /// Note that the number of digits in the fractional seconds varies with the
    /// number of fractional seconds. It's a maximum of 7 (which would be
    /// hnsecs), but only has as many as are necessary to hold the correct value
    /// (so no trailing zeroes), and if there are no fractional seconds, then
    /// there is no decimal point.

    /// If this XTime's time zone is XLocalTime, then TZ is empty. If its time zone
    /// is XUTC, then it is "Z". Otherwise, it is the offset from UTC (e.g. +1:00 or -7:00).
    /// Note that the offset from UTC is <i>not</i> enough to uniquely identify the time zone.
    ///
    /// Time zone offsets will be in the form +HH:MM or -HH:MM.
    X_API XString ToISOExtString() const;

    /// \brief Returns the date and time in the format used in the MPEG-7 standard.
    ///
    /// The format is identical to ISO Extended except that no timezone is given and the
    ///  fractional seconds are ":nnnFNNNN" which represents the fraction nnn/NNNN.
    /// Similar to toISOExtString, no fractional seconds are given if there aren't any,
    /// and the minimal fraction is used (e.g. ":12F1000" instead ":1200000F100000000").
    X_API XString ToMPEG7String() const;

    /// \brief Returns the date and time in the format specified by the given strings. The time zone is not included.
    ///
    /// \param dateFormat    The format for the date per XDateTuple::ToCustomString.
    /// \param timeFormat    The format for the time per XTimeTuple::ToCustomString.
    /// \param sep           The string separating the date and time.
    /// \param dateFirst     Whether the date should be listed first.
    /// \param numFracDigits How many fractional digits to put in the string. They are
    ///                      in the ISO Extended format.
    X_API XString ToCustomString(const XString& dateFormat,
                                 const XString& timeFormat,
                                 const XString& sep = " ",
                                 bool dateFirst = true,
                                 size_t numFracDigits = 0) const;

    /// \brief Returns the date and time as XML. The time zone is included.
    X_API XString ToXML() const;

    /// \brief Takes a string in the ISO Extended format for date/times ("YYYY-MM-DDTHH:MM:SS.FFFFFFFTZ")
    //         and returns an equivalent XTime.
    ///
    /// The given string will be stripped, so extra whitespace at the beginning or end is fine.
    ///
    /// An XException is thrown if the string is not in the valid format.
    X_API static XTime FromISOExtString(const XString& isoExtString);

    /// \brief Takes a string in the MPEG-7 format for date/times ("YYYY-MM-DDTHH:MM:SS:nnnFNNN")
    //         and returns an equivalent XTime.
    ///
    /// The given string will be stripped, so extra whitespace at the beginning or end is fine.
    ///
    /// The MPEG-7 string is assumed to represent a time in local time, and the resulting
    /// XTime will have XLocalTime::Instance() as its time zone.
    ///
    /// An XException is thrown if the string is not in the valid format.
    X_API static XTime FromMPEG7String(const XString& mpeg7String);

    /// \brief Takes a string of XML in the format returned by ToXML and returns an equivalent XTime.
    ///
    /// An XException is thrown if the string is not in the valid format.
    X_API static XTime FromXML(const XString& xml);

    /**
        \brief Minimum value representable by XTime.

        Its TimeZone is XUTC.
     */
    static X_API XTime Min() { return XTime(0, XUTC::Instance()); }

    /**
        \brief Maximum value representable by XTime.

        Its TimeZone is XUTC.
     */
    static X_API XTime Max() { return XTime(LLONG_MAX, XUTC::Instance()); }


private:


    /// Returns the Xth day of the Gregorian Calendar that this XTime is on.
    X_API int DayOfGregorianCal() const
    {
        return XDateTuple::DayOfGregorianCal(AdjTime());
    }

    /// The ticks adjusted to the timezone for this XTime.
    X_API int64_t AdjTime() const
    {
        const int64_t adjustedTime = _tz->UTCToTZ(_ticks < 0 ? 0 : _ticks);
        return adjustedTime < 0 ? 0 : adjustedTime;
    }

    /// The ticks adjusted to the timezone for this XTime.
    X_API void AdjTime(int64_t adjustedTicks)
    {
        _ticks = _tz->TZToUTC(adjustedTicks < 0 ? 0 : adjustedTicks);

        if(_ticks < 0)
            _ticks = 0;
    }

    ///HectoNanoSeconds units
    int64_t _ticks;

    XIRef<XTimeZone> _tz;
};


namespace Clock
{
    /**
        \brief Returns the number of hnsecs since midnight, January 1st, 1 AD for the
               current time.

        \note Currently gives usec precision, but we could make it use hnsec precision
              if need be. Regardless, its hnsecs which are returned.
      */
    X_API inline int64_t currTick()
    {
        const int64_t hnsecsToUnixEpoch = 621355968000000000LL;
        timeval tv;

        x_gettimeofday(&tv);

        return convert(SECONDS, HNSECS, tv.tv_sec) +
               convert(USECS, HNSECS, tv.tv_usec) +
               hnsecsToUnixEpoch;
    }


    /**
        \brief Returns the current time.

        \param tz The time zone that you want the XTime which is returned to have.
      */
    X_API inline XTime currTime(XIRef<XTimeZone> tz = XLocalTime::Instance()/*= XLocalTime::Instance()*/)
    {
        return XTime(currTick(), tz);
    }
}


}

#endif
