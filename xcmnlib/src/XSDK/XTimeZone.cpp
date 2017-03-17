
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XTimeZone.h"

#include <algorithm>

#include "XSDK/OS.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XDuration.h"
#include "XSDK/XDomParser.h"

using namespace std;
using namespace XSDK;

bool XUTC::_cInitialized = false;
XMutex XUTC::_cInstanceLock;

bool XLocalTime::_cInitialized = false;
XMutex XLocalTime::_cInstanceLock;


//For some reason, I can't get isdigit to work directly.
static bool verifyDigit(char c)
{
    //I should be able to just return the result, but Windows is stupid.
    return isdigit(c) != 0;
}


XDuration XTimeZone::UTCOffsetAt(int64_t ticks) const
{
    return XDuration(HNSECS, UTCToTZ(ticks) - ticks);
}


XIRef<XTimeZone> XLocalTime::Instance()
{
    static XIRef<XTimeZone> instance = 0;

    if(!_cInitialized)
    {
        XGuard lock(_cInstanceLock);

        if(instance.IsEmpty())
        {
            instance = new XLocalTime;
#ifndef WIN32
            tzset();
#endif
            _cInitialized = true;
        }
    }

    return instance;
}

bool XLocalTime::HasDST() const
{
#ifdef WIN32
    TIME_ZONE_INFORMATION tzInfo;
    GetTimeZoneInformation(&tzInfo);

    return tzInfo.DaylightDate.wMonth != 0;
#else
    return (bool)(daylight);
#endif
}


bool XLocalTime::DSTInEffect(int64_t ticks) const
{
#ifdef WIN32
    TIME_ZONE_INFORMATION tzInfo;
    GetTimeZoneInformation(&tzInfo);

    return winDSTInEffect(&tzInfo, ticks);
#else
    time_t unixTime = ticksToUnixTime(ticks);
    tm* timeInfo = localtime(&unixTime);

    return (bool)timeInfo->tm_isdst;
#endif
}


int XLocalTime::UTCOffset() const
{
    timeval tv;
    x_gettimeofday(&tv);
    const int64_t ticks = unixTimeToTicks(tv.tv_sec);
    const int64_t adjTicks = UTCToTZ(ticks);
    const int currDiff = (int)convert(HNSECS, MINUTES, adjTicks - ticks);

    if(DSTInEffect(ticks))
        return currDiff - DSTOffset();

    return currDiff;
}


int XLocalTime::DSTOffset() const
{
#ifdef WIN32
    const uint64_t hnsecsFrom1601 = 504911232000000000LL;

    SYSTEMTIME janST;
    GetLocalTime(&janST);
    janST.wMonth = 1;
    janST.wDay = 4;

    FILETIME janFT;
    SystemTimeToFileTime(&janST, &janFT);
    uint64_t janHNSecs = janFT.dwHighDateTime;
    janHNSecs <<= 32;
    janHNSecs |= janFT.dwLowDateTime;
    janHNSecs += hnsecsFrom1601;
    const int64_t janTicks = (int64_t)janHNSecs;
    const int64_t janAdjTicks = UTCToTZ(janTicks);
    const int janDiff = (int)convert(HNSECS, MINUTES, janAdjTicks - janTicks);

    SYSTEMTIME julST = janST;
    julST.wMonth = 7;

    FILETIME julFT;
    SystemTimeToFileTime(&julST, &julFT);
    uint64_t julHNSecs = julFT.dwHighDateTime;
    julHNSecs <<= 32;
    julHNSecs |= julFT.dwLowDateTime;
    julHNSecs += hnsecsFrom1601;
    const int64_t julTicks = (int64_t)julHNSecs;
    const int64_t julAdjTicks = UTCToTZ(julTicks);
    const int julDiff = (int)convert(HNSECS, MINUTES, julAdjTicks - julTicks);

    return abs(julDiff - janDiff);
#else
    time_t currTimeT = time(0);
    tm janTM = *gmtime(&currTimeT);
    janTM.tm_mon = 1;
    janTM.tm_mday = 4;
    const time_t janTimeT = mktime(&janTM);
    const int64_t janTicks = unixTimeToTicks(janTimeT);
    const int64_t janAdjTicks = UTCToTZ(janTicks);
    const int janDiff = (int)convert(HNSECS, MINUTES, janAdjTicks - janTicks);

    tm julTM = janTM;
    julTM.tm_mon = 7;
    const time_t julTimeT = mktime(&julTM);
    const int64_t julTicks = unixTimeToTicks(julTimeT);
    const int64_t julAdjTicks = UTCToTZ(julTicks);
    const int julDiff = (int)convert(HNSECS, MINUTES, julAdjTicks - julTicks);

    return abs(julDiff - janDiff);
#endif
}


int64_t XLocalTime::UTCToTZ(int64_t ticks) const
{
#ifdef IS_LINUX
    time_t unixTime = ticksToUnixTime(ticks);
    tm* timeInfo = localtime(&unixTime);

    return ticks + convert(SECONDS, HNSECS, timeInfo->tm_gmtoff);
#elif defined(IS_WINDOWS)
    TIME_ZONE_INFORMATION tzInfo;
    GetTimeZoneInformation(&tzInfo);

    return winUTCToTZ(&tzInfo, ticks, HasDST());
#endif
}


int64_t XLocalTime::TZToUTC(int64_t adjustedTicks) const
{
#ifdef IS_LINUX
    time_t unixTime = ticksToUnixTime(adjustedTicks);

    time_t past = unixTime - (time_t)convert(DAYS, SECONDS, 1);
    tm* timeInfo = localtime(past < unixTime ? &past : &unixTime);
    const int pastOffset = timeInfo->tm_gmtoff;

    time_t future = unixTime + (time_t)convert(DAYS, SECONDS, 1);
    timeInfo = localtime(future > unixTime ? &future : &unixTime);
    const int futureOffset = timeInfo->tm_gmtoff;

    if(pastOffset == futureOffset)
        return adjustedTicks - convert(SECONDS, HNSECS, pastOffset);

    if(pastOffset < futureOffset)
        unixTime -= (time_t)convert(HOURS, SECONDS, 1);

    unixTime -= pastOffset;
    timeInfo = localtime(&unixTime);

    return adjustedTicks - convert(SECONDS, HNSECS, timeInfo->tm_gmtoff);
#elif defined(IS_WINDOWS)
    TIME_ZONE_INFORMATION tzInfo;

    GetTimeZoneInformation(&tzInfo);

    return winTZToUTC(&tzInfo, adjustedTicks, HasDST());
#endif
}


XIRef<XDomParserNode> XLocalTime::ToXML() const
{
    XIRef<XDomParserNode> node = new XDomParserNode;
    node->SetTagName("XLocalTime");

    return node;
}


XIRef<XTimeZone> XUTC::Instance()
{
    static XIRef<XTimeZone> instance = 0;

    if(!_cInitialized)
    {
        XGuard lock(_cInstanceLock);

        if(instance.IsEmpty())
        {
            instance = new XUTC;
            _cInitialized = true;
        }
    }

    return instance;
}

bool XUTC::HasDST() const
{
    return false;
}


bool XUTC::DSTInEffect(int64_t ticks) const
{
    return false;
}


XDuration XUTC::UTCOffsetAt(int64_t ticks) const
{
    return XDuration::Zero();
}


int XUTC::UTCOffset() const
{
    return 0;
}


int XUTC::DSTOffset() const
{
    return 0;
}


int64_t XUTC::UTCToTZ(int64_t ticks) const
{
    return ticks;
}


int64_t XUTC::TZToUTC(int64_t adjustedTicks) const
{
    return adjustedTicks;
}


XIRef<XDomParserNode> XUTC::ToXML() const
{
    XIRef<XDomParserNode> node = new XDomParserNode;
    node->SetTagName("XUTC");

    return node;
}


bool XSimpleTimeZone::HasDST() const
{
    return _dstOffset != 0;

}


bool XSimpleTimeZone::DSTInEffect(int64_t ticks) const
{
    return _dstOffset != 0;
}


int XSimpleTimeZone::UTCOffset() const
{
    return _utcOffset;
}


int XSimpleTimeZone::DSTOffset() const
{
    return _dstOffset;
}


int64_t XSimpleTimeZone::UTCToTZ(int64_t ticks) const
{
    return ticks + convert(MINUTES, HNSECS, _utcOffset + _dstOffset);
}


int64_t XSimpleTimeZone::TZToUTC(int64_t adjustedTicks) const
{
    return adjustedTicks - convert(MINUTES, HNSECS, _utcOffset + _dstOffset);
}


XIRef<XDomParserNode> XSimpleTimeZone::ToXML() const
{
    XIRef<XDomParserNode> node = new XDomParserNode;
    node->SetTagName("XSimpleTimeZone");
    node->AddMetaData("utcOffset", XString::FromInt(_utcOffset));
    node->AddMetaData("dstOffset", XString::FromInt(_dstOffset));

    return node;
}


XIRef<XTimeZone> XSimpleTimeZone::FromXML(XIRef<XDomParserNode> node)
{
    const XString utcOffsetStr = node->GetMetaData("utcOffset");
    const XString dstOffsetStr = node->GetMetaData("dstOffset");

    if(node->GetTagName() != "XSimpleTimeZone" ||
       utcOffsetStr.empty() ||
       (!verifyDigit(utcOffsetStr[0]) && utcOffsetStr[0] != '-') ||
       count_if(utcOffsetStr.begin() + 1, utcOffsetStr.end(), verifyDigit) != (int)utcOffsetStr.size() - 1 ||
       dstOffsetStr.empty() ||
       (!verifyDigit(dstOffsetStr[0]) && dstOffsetStr[0] != '-') ||
       count_if(dstOffsetStr.begin() + 1, dstOffsetStr.end(), verifyDigit) != (int)dstOffsetStr.size() - 1)
    {
        return 0;
    }

    const int utcOffset = utcOffsetStr.ToInt();
    const int dstOffset = dstOffsetStr.ToInt();

    return new XSimpleTimeZone(utcOffset, dstOffset);
}


XString XSimpleTimeZone::ToISOString(int utcOffset)
{
    const int absOffset = abs(utcOffset);
    X_ASSERT(absOffset <= 780 && "UTC Offset must be within range (-13:00 - +13:00)");

    const int hours = (int)convert(MINUTES, HOURS, absOffset);
    const int minutes = (int)(absOffset - convert(HOURS, MINUTES, hours));

    if(utcOffset < 0)
        return XString::Format("-%02d:%02d", hours, minutes);

    return XString::Format("+%02d:%02d", hours, minutes);
}

XIRef<XTimeZone> XSimpleTimeZone::FromISOString(const XString& isoString)
{
    if(isoString[0] != '-' && isoString[0] != '+')
        X_THROW(("Invalid ISO Extended String: %s", isoString.c_str()));

    const int sign = isoString[0] == '-' ? -1 : 1;
    const XString str = isoString.substr(1);

    if(str.empty())
        X_THROW(("Invalid ISO Extended String: %s", isoString.c_str()));

    const size_t colonDex = str.find(':');
    XString hoursStr;
    XString minutesStr;

    if(colonDex != string::npos)
    {
        hoursStr = str.substr(0, colonDex);
        minutesStr = str.substr(colonDex + 1);

        if(minutesStr.size() != 2)
            X_THROW(("Invalid ISO Extended String: %s", isoString.c_str()));
    }
    else
        hoursStr = str;

    if(count_if(hoursStr.begin(), hoursStr.end(), verifyDigit) != (int)hoursStr.size() ||
       count_if(minutesStr.begin(), minutesStr.end(), verifyDigit) != (int)minutesStr.size())
    {
        X_THROW(("Invalid ISO Extended String: %s", isoString.c_str()));
    }

    const int hours = hoursStr.ToInt();
    const int minutes = minutesStr.empty() ? 0 : minutesStr.ToInt();

    return new XSimpleTimeZone(sign * (int)(convert(HOURS, MINUTES, hours) + minutes));
}


#ifdef IS_WINDOWS
int64_t XLocalTime::winUTCToTZ(const TIME_ZONE_INFORMATION* tzInfo, int64_t ticks, bool hasDST)
{
    if(hasDST && winDSTInEffect(tzInfo, ticks))
        return ticks - convert(MINUTES, HNSECS, tzInfo->Bias + tzInfo->DaylightBias);

    return ticks - convert(MINUTES, HNSECS, tzInfo->Bias + tzInfo->StandardBias);
}

bool XLocalTime::dstInEffectForLocalDateTime(const TIME_ZONE_INFORMATION* tzInfo,
                                             XDateTuple localDate,
                                             XTimeTuple localTime)
{
    //The limits of what SystemTimeToTZSpecificLocalTime will accept.
    if(localDate.Year() < 1601)
    {
        if(localDate.Month() == 2 && localDate.Day() == 29)
            localDate.SetDay(28);

        localDate.SetYear(1601);
    }
    else if(localDate.Year() > 9999)
    {
        if(localDate.Month() == 2 && localDate.Day() == 29)
            localDate.SetDay(28);

        localDate.SetYear(9999);
    }

    //SystemTimeToTZSpecificLocalTime doesn't act correctly at the
    //beginning or end of the year (bleh). Unless some bizarre time
    //zone changes DST on January 1st or December 31st, this should
    //fix the problem.
    if(localDate.Month() == 1)
    {
        if(localDate.Day() == 1)
            localDate.SetDay(2);
    }
    else if(localDate.Month() == 12 && localDate.Day() == 31)
        localDate.SetDay(30);

    SYSTEMTIME utcST;
    SYSTEMTIME localST;

    localST.wYear = localDate.Year();
    localST.wMonth = localDate.Month();
    localST.wDay = localDate.Day();
    localST.wHour = localTime.Hour();
    localST.wMinute = localTime.Minute();
    localST.wSecond = localTime.Second();
    localST.wMilliseconds = 0;

    const int result = TzSpecificLocalTimeToSystemTime(tzInfo,
                                                       &localST,
                                                       &utcST);
    X_ASSERT(result != 0);

    const XDateTuple utcDate = XDateTuple(utcST.wYear, utcST.wMonth, utcST.wDay);
    const XTimeTuple utcTime = XTimeTuple(utcST.wHour, utcST.wMinute, utcST.wSecond);
    const XDuration diff = (localDate - utcDate) + (localTime - utcTime);
    const int64_t minutes = -tzInfo->Bias - diff.Total(MINUTES);

    if(minutes == tzInfo->DaylightBias)
        return true;

    X_ASSERT(minutes == tzInfo->StandardBias);

    return false;
}


int64_t XLocalTime::winTZToUTC(const TIME_ZONE_INFORMATION* tzInfo, int64_t adjTicks, bool hasDST)
{

    if(hasDST)
    {
        const XDateTuple localDate = XDateTuple(XDateTuple::DayOfGregorianCal(adjTicks));
        const XTimeTuple localTime = XTimeTuple(adjTicks);

        int64_t adjTicksBefore = adjTicks - convert(HOURS, HNSECS, 1);

        if(adjTicksBefore < 0)
            adjTicksBefore = 0;

        const XDateTuple localDateBefore = XDateTuple(XDateTuple::DayOfGregorianCal(adjTicksBefore));
        const XTimeTuple localTimeBefore = XTimeTuple(adjTicksBefore);

        const int64_t adjTicksAfter = adjTicks + convert(HOURS, HNSECS, 1);
        const XDateTuple localDateAfter = XDateTuple(XDateTuple::DayOfGregorianCal(adjTicksAfter));
        const XTimeTuple localTimeAfter = XTimeTuple(adjTicksAfter);

        const bool dstInEffectNow = dstInEffectForLocalDateTime(tzInfo, localDate, localTime);
        const bool dstInEffectBefore = dstInEffectForLocalDateTime(tzInfo, localDateBefore, localTimeBefore);
        const bool dstInEffectAfter = dstInEffectForLocalDateTime(tzInfo, localDateAfter, localTimeAfter);

        bool isDST;

        if(dstInEffectBefore && dstInEffectNow && dstInEffectAfter)
            isDST = true;
        else if(!dstInEffectBefore && !dstInEffectNow && !dstInEffectAfter)
            isDST = false;
        else if(!dstInEffectBefore && dstInEffectAfter)
            isDST = false;
        else if(dstInEffectBefore && !dstInEffectAfter)
            isDST = dstInEffectNow;
        else
            X_ASSERT(0 && "Bad Logic.");

        if(isDST)
            return adjTicks + convert(MINUTES, HNSECS, tzInfo->Bias + tzInfo->DaylightBias);
    }

    return adjTicks + convert(MINUTES, HNSECS, tzInfo->Bias + tzInfo->StandardBias);
}


bool XLocalTime::winDSTInEffect(const TIME_ZONE_INFORMATION* tzInfo, int64_t ticks)
{
    if(tzInfo->DaylightDate.wMonth == 0)
        return false;

    XDateTuple utcDate = XDateTuple(XDateTuple::DayOfGregorianCal(ticks));
    XTimeTuple utcTime = XTimeTuple(ticks);

    //The limits of what SystemTimeToTZSpecificLocalTime will accept.
    //This is pointless for what we do at Pelco (since we never care about
    //dates outside of the range that SystemTimeToTZSpecificLocalTime accepts),
    //but some of the more thorough unit tests might care.
    if(utcDate.Year() < 1601)
    {
        if(utcDate.Month() == 2 && utcDate.Day() == 29)
            utcDate.SetDay(28);

        utcDate.SetYear(1601);
    }
    else if(utcDate.Year() > 9999)
    {
        if(utcDate.Month() == 2 && utcDate.Day() == 29)
            utcDate.SetDay(28);

        utcDate.SetYear(9999);
    }

    //SystemTimeToTZSpecificLocalTime doesn't act correctly at the
    //beginning or end of the year (bleh). Unless some bizarre time
    //zone changes DST on January 1st or December 31st, this should
    //fix the problem.
    if(utcDate.Month() == 1)
    {
        if(utcDate.Day() == 1)
            utcDate.SetDay(2);
    }
    else if(utcDate.Month() == 12 && utcDate.Day() == 31)
        utcDate.SetDay(30);

    SYSTEMTIME utcST;
    SYSTEMTIME otherST;

    utcST.wYear = utcDate.Year();
    utcST.wMonth = utcDate.Month();
    utcST.wDay = utcDate.Day();
    utcST.wHour = utcTime.Hour();
    utcST.wMinute = utcTime.Minute();
    utcST.wSecond = utcTime.Second();
    utcST.wMilliseconds = 0;

    int result = SystemTimeToTzSpecificLocalTime((TIME_ZONE_INFORMATION*)tzInfo, &utcST, &otherST);
    X_ASSERT(result != 0);

    const XDateTuple otherDate = XDateTuple(otherST.wYear, otherST.wMonth, otherST.wDay);
    const XTimeTuple otherTime = XTimeTuple(otherST.wHour, otherST.wMinute, otherST.wSecond);
    const XDuration diff = (utcDate - otherDate) + (utcTime - otherTime);
    const int64_t minutes = diff.Total(MINUTES) - tzInfo->Bias;

    if(minutes == tzInfo->DaylightBias)
        return true;

    X_ASSERT(minutes == tzInfo->StandardBias);

    return false;
}
#endif
