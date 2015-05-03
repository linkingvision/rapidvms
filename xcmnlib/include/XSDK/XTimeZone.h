
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XTimeZone_H_
#define _XTimeZone_H_

#include "XSDK/OS.h"

#include <ctime>
#include <map>

#include "XSDK/Errors.h"
#include "XSDK/Types.h"
#include "XSDK/XIRef.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XDateTuple.h"
#include "XSDK/XDomParserNode.h"
#include "XSDK/XDuration.h"

class XTimeZoneTest;

namespace XSDK
{

class XTime;

/**
   \brief Converts a time_t to hecto-nanosecond ticks.

    Converts a time_t (which uses midnight January 1st, 1970 UTC as its epoch
    and seconds as its units) to the number of hecto-nanosecond ticks since
    midnight January 1st, 1 AD UTC.

    \param unixTime The time_t to convert.
  */
X_API inline int64_t unixTimeToTicks(time_t unixTime);


/**
   \brief Converts a hecto-nanosecond ticks from the unix time epoch to hecto-nanosecond ticks
          from midnight, January 1st 1 A.D.

    \param unixTicks Hecto-nanoseconds from unix time epoch to convert to ticks.
  */
X_API inline int64_t unixTimeEpochToTicks(int64_t unixTicks);


/**
    Converts ticks (which uses midnight January 1st, 1 AD UTC as their epoch
    and hnsecs as their units) to time_t (which uses midnight
    January 1st, 1970 UTC as its epoch and seconds as its units).

    \param ticks The ticks to convert.
  */
X_API inline time_t ticksToUnixTime(int64_t ticks);


/**
    Converts ticks (which uses midnight January 1st, 1 AD UTC as their epoch
    and hnsecs as their units) to the epoch of midnight January 1st, 1970 UTC.
    However, they're still in hnsecs.

    \param ticks The ticks to convert.
  */
X_API inline int64_t ticksToUnixTimeEpoch(int64_t ticks);


/**
    Represents a time zone. It is used with XTime to indicate the time zone of
    an XTime. It is abstract.
  */
class XTimeZone : public XBaseObject
{
    friend class XTime;
    friend class ::XTimeZoneTest;
public:

    /**
        \brief Whether this time zone has Daylight Savings Time.
      */
    X_API virtual bool HasDST() const = 0;


    /**
        Takes the number of hecto-nanosecond (100 ns) ticks since midnight,
        January 1st, 1 A.D. in UTC time and returns whether DST is effect in
        this time zone at the given point in time.

        Params:
            ticks = The UTC time that needs to be checked for DST in this time zone.
      */
    X_API virtual bool DSTInEffect(int64_t ticks) const = 0;


    /// \brief The total offset from UTC at the given time (includes adjustment for DST).
    X_API virtual XDuration UTCOffsetAt(int64_t ticks) const;


    /**
        \brief The offset from UTC in minutes.

        West of UTC is negative.
      */
    X_API virtual int UTCOffset() const = 0;


    /**
        \brief Minutes added to the UTC offset during DST.

        If HasDST() is false, then this is always zero.
      */
    X_API virtual int DSTOffset() const = 0;


    /**
        Takes the number of hecto-nanosecond (100 ns) ticks since midnight,
        January 1st, 1 A.D. in UTC time and converts them to this time zone's time.

        /param ticks The UTC time that needs to be adjusted to this time zone's
                     time.
      */
    X_API virtual int64_t UTCToTZ(int64_t ticks) const = 0;


    /**
        Takes the number of hecto-nanosecond (100 ns) ticks since midnight,
        January 1st, 1 A.D. in this time zone's time and converts them to UTC.

        /param adjustedTicks The time in this time zone that needs to be adjusted to
                             UTC time.
      */
    X_API virtual int64_t TZToUTC(int64_t adjustedTicks) const = 0;


private:
    /// Returns the time zone as an XML string.
    X_API virtual XIRef<XDomParserNode> ToXML() const = 0;
};


/**
    A XTimeZone which represents UTC.
  */
class XUTC : public XTimeZone
{
    friend class ::XTimeZoneTest;
public:

    /// XUTC is a singleton class.
    X_API static XIRef<XTimeZone> Instance();

    X_API virtual bool HasDST() const;
    X_API virtual bool DSTInEffect(int64_t ticks) const;
    X_API virtual XDuration UTCOffsetAt(int64_t ticks) const;
    X_API virtual int UTCOffset() const;
    X_API virtual int DSTOffset() const;
    X_API virtual int64_t UTCToTZ(int64_t ticks) const;
    X_API virtual int64_t TZToUTC(int64_t adjustedTicks) const;

private:

    X_API virtual XIRef<XDomParserNode> ToXML() const;

    static bool    _cInitialized;
    static XMutex _cInstanceLock;

    XUTC() {}
};


class XLocalTime : public XTimeZone
{
public:

    /// XLocalTime is a singleton class.
    X_API static XIRef<XTimeZone> Instance();

    X_API virtual bool HasDST() const;
    X_API virtual bool DSTInEffect(int64_t ticks) const;

    /// Gives UTC offset for the current date.
    ///
    /// This uses the current UTC offset regardless of whether the UTC offset
    /// has ever changed for the local time zone. Complete accuracy would
    /// require checking the UTC offset for a specific date and time.
    X_API virtual int UTCOffset() const;

    /// Gives DST offset for the current date.
    ///
    /// This uses the current DST rules regardless of whether the DST offset
    /// rules have ever changed for this time zone. Complete accuracy would
    /// require checking the DST Offset for a specific date and time.
    X_API virtual int DSTOffset() const;

    X_API virtual int64_t UTCToTZ(int64_t ticks) const;
    X_API virtual int64_t TZToUTC(int64_t adjustedTicks) const;

private:

    X_API virtual XIRef<XDomParserNode> ToXML() const;

#ifdef WIN32
    X_API static int64_t winUTCToTZ(const TIME_ZONE_INFORMATION* tzInfo, int64_t ticks, bool hasDST);
    X_API static int64_t winTZToUTC(const TIME_ZONE_INFORMATION* tzInfo, int64_t adjTicks, bool hasDST);
    X_API static bool winDSTInEffect(const TIME_ZONE_INFORMATION* tzInfo, int64_t ticks);
    X_API static bool dstInEffectForLocalDateTime(const TIME_ZONE_INFORMATION* tzInfo,
                                                  XDateTuple localDate,
                                                  XTimeTuple localTime);
#endif

    static bool   _cInitialized;
    static XMutex _cInstanceLock;

    XLocalTime() {}
};


/**
    Represents a time zone with an offset (in minutes, west is negative) from
    UTC. If a DST offset is non-zero, then DST is considered to be in effect,
    and the DST offset is added to the UTC offset. If the DST offset is 0, then
    DST is not considered to be in effect.
  */
class XSimpleTimeZone : public XTimeZone
{
    friend class XTime;
    friend class ::XTimeZoneTest;
public:

    X_API virtual bool HasDST() const;
    X_API virtual bool DSTInEffect(int64_t ticks) const;
    X_API virtual int UTCOffset() const;
    X_API virtual int DSTOffset() const;

    X_API virtual int64_t UTCToTZ(int64_t ticks) const;
    X_API virtual int64_t TZToUTC(int64_t adjustedTicks) const;

    /**
        \param utcOffset This time zone's offset from UTC in minutes with west
                         of UTC being negative (it is added to UTC to get the
                         adjusted time).
        \param dstOffset The number of minutes to add to the UTC offset during DST.
      */
    X_API XSimpleTimeZone(int utcOffset, int dstOffset = 0)
        : _utcOffset(utcOffset),
          _dstOffset(dstOffset)
    {

        if(_dstOffset < 0)
        {
            X_STHROW(XException,
                     (XSDK::XString::Format("DST offsets cannot be negative. utcOffset [%d] dstOffset [%d]",
                                            _utcOffset,  _dstOffset)));
        }

        if(abs(_utcOffset) > 780 || abs(_utcOffset + _dstOffset) > 780)
        {
            X_STHROW(XException,
                     (XSDK::XString::Format("Total offset must be within range [-13:00 - +13:00]. utcOffset [%d] dstOffset [%d]",
                                            _utcOffset,  _dstOffset)));
        }
    }


private:

    X_API virtual XIRef<XDomParserNode> ToXML() const;

    /// \brief Takes a string of XML in the format returned by ToXML and returns an equivalent XSimpleTimeZone.
    ///
    /// An empty XIRef is returned if the node is not valid XML for an XSimpleTimeZone.
    X_API static XIRef<XTimeZone> FromXML(XIRef<XDomParserNode> node);

    X_API static XString ToISOString(int utcOffset);
    X_API static XIRef<XTimeZone> FromISOString(const XString& isoString);

    const int _utcOffset;
    const int _dstOffset;
};



X_API inline int64_t unixTimeToTicks(time_t unixTime)
{
    return 621355968000000000LL + convert(SECONDS, HNSECS, unixTime);
}


X_API inline time_t ticksToUnixTime(int64_t ticks)
{
    const int64_t hnsecsAtUnixTimeEpoch = convert(HNSECS, SECONDS, ticks - 621355968000000000LL);

    return (time_t)hnsecsAtUnixTimeEpoch;
}


X_API inline int64_t unixTimeEpochToTicks(int64_t unixTicks)
{
    return 621355968000000000LL + unixTicks;
}


X_API inline int64_t ticksToUnixTimeEpoch(int64_t ticks)
{
    return ticks - 621355968000000000LL;
}

}

#endif
