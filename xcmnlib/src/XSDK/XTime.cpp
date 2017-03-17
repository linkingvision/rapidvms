
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XTime.h"

#include <algorithm>

#include "XSDK/XDomParser.h"
#include "XSDK/XDomParserNode.h"

using namespace std;
using namespace XSDK;


int secsInTimeTuple(const XTimeTuple& time)
{
    return (int)convert(HOURS, SECONDS, time.Hour()) +
           (int)convert(MINUTES, SECONDS, time.Minute()) +
           time.Second();
}


XTime::XTime(const XDateTuple& date,
             const XTimeTuple& time,
             XIRef<XTimeZone> tz)
    : _ticks(0),
      _tz(tz)
{
    const int days = date.DayOfGregorianCal() - 1;
    const int seconds = secsInTimeTuple(time);
    const int64_t adjusted = convert(DAYS, HNSECS, days) +
                             convert(SECONDS, HNSECS, seconds);

    _ticks = tz->TZToUTC(adjusted);
}


XTime::XTime(const XDateTuple& date,
             const XTimeTuple& time,
             const XFracSec& fracSec,
             XIRef<XTimeZone> tz)
    : _ticks(0),
      _tz(tz)
{
    if(fracSec.IsNegative())
        X_THROW(("XTime's XFracSec cannot be negative."));

    const int days = date.DayOfGregorianCal() - 1;
    const int seconds = secsInTimeTuple(time);
    const int64_t adjusted = convert(DAYS, HNSECS, days) +
                             convert(SECONDS, HNSECS, seconds) +
                             fracSec.HNSecs();

    _ticks = tz->TZToUTC(adjusted);
}


XTime::XTime(time_t unixTime, const XFracSec& fracSec, XIRef<XTimeZone> tz)
    : _ticks(0),
      _tz(tz)
{
    if(fracSec.IsNegative())
        X_THROW(("XTime's XFracSec cannot be negative."));

    _ticks = unixTimeToTicks(unixTime) + fracSec.HNSecs();
}


void XTime::SetYear(int value)
{
    if(value < 1)
        X_THROW(("Years less that 1 not supported"));

    int64_t hnsecs = AdjTime();
    int64_t days = splitUnitsFromHNSecs(DAYS, hnsecs) + 1;

    XDateTuple date = XDateTuple((int)days);

    if(date._day > XDateTuple::MaxDay(value, date._month))
        X_THROW(("There is no February 29th in %d.", value));

    date._year = value;

    const int64_t newDaysHNSecs = convert(DAYS, HNSECS, date.DayOfGregorianCal() - 1);

    AdjTime(newDaysHNSecs + hnsecs);
}


void XTime::SetMonth(int value)
{
    if(value < 1 || value > 12)
        X_THROW(("%d is not a valid month of the year.", value));

    int64_t hnsecs = AdjTime();
    int64_t days = splitUnitsFromHNSecs(DAYS, hnsecs) + 1;

    XDateTuple date = XDateTuple((int)days);

    if(date._day > XDateTuple::MaxDay(date._year, value))
        X_THROW(("There is no day %d in month %d in year %d.", date._day, value, date._year));

    date._month = value;

    const int64_t newDaysHNSecs = convert(DAYS, HNSECS, date.DayOfGregorianCal() - 1);

    AdjTime(newDaysHNSecs + hnsecs);
}


void XTime::SetDay(int value)
{
    int64_t hnsecs = AdjTime();
    int64_t days = splitUnitsFromHNSecs(DAYS, hnsecs) + 1;

    XDateTuple date = XDateTuple((int)days);

    if(value < 0 || value > XDateTuple::MaxDay(date._year, date._month))
        X_THROW(("%d is not a valid day of the month %d.", value, date._month));

    date._day = value;

    const int64_t newDaysHNSecs = convert(DAYS, HNSECS, date.DayOfGregorianCal() - 1);

    AdjTime(newDaysHNSecs + hnsecs);
}


void XTime::SetHour(int value)
{
    if(value < 0 || value > 23)
        X_THROW(("%d is not a valid hour of the day.", value));

    int64_t hnsecs = AdjTime();
    int64_t days = splitUnitsFromHNSecs(DAYS, hnsecs);
    const int64_t daysHNSecs = convert(DAYS, HNSECS, days);

    hnsecs = removeUnitsFromHNSecs(HOURS, hnsecs);
    hnsecs += convert(HOURS, HNSECS, value);

    AdjTime(daysHNSecs + hnsecs);
}


void XTime::SetMinute(int value)
{
    if(value < 0 || value > 59)
        X_THROW(("%d is not a valid minute of an hour.", value));

    int64_t hnsecs = AdjTime();
    int64_t days = splitUnitsFromHNSecs(DAYS, hnsecs);
    const int64_t daysHNSecs = convert(DAYS, HNSECS, days);

    const int64_t hour = splitUnitsFromHNSecs(HOURS, hnsecs);
    hnsecs = removeUnitsFromHNSecs(MINUTES, hnsecs);

    hnsecs += convert(HOURS, HNSECS, hour);
    hnsecs += convert(MINUTES, HNSECS, value);

    AdjTime(daysHNSecs + hnsecs);
}


void XTime::SetSecond(int value)
{
    if(value < 0 || value > 59)
        X_THROW(("%d is not a valid second of a minute.", value));

    int64_t hnsecs = AdjTime();
    int64_t days = splitUnitsFromHNSecs(DAYS, hnsecs);
    const int64_t daysHNSecs = convert(DAYS, HNSECS, days);

    const int64_t hour = splitUnitsFromHNSecs(HOURS, hnsecs);
    const int64_t minute = splitUnitsFromHNSecs(MINUTES, hnsecs);
    hnsecs = removeUnitsFromHNSecs(SECONDS, hnsecs);

    hnsecs += convert(HOURS, HNSECS, hour);
    hnsecs += convert(MINUTES, HNSECS, minute);
    hnsecs += convert(SECONDS, HNSECS, value);

    AdjTime(daysHNSecs + hnsecs);
}


void XTime::SetFracSec(const XFracSec& value)
{
    if(value.IsNegative())
        X_THROW(("XTime's XFracSec cannot be negative."));

    int64_t hnsecs = AdjTime();
    int64_t days = splitUnitsFromHNSecs(DAYS, hnsecs);
    const int64_t daysHNSecs = convert(DAYS, HNSECS, days);

    const int64_t hour = splitUnitsFromHNSecs(HOURS, hnsecs);
    const int64_t minute = splitUnitsFromHNSecs(MINUTES, hnsecs);
    const int64_t second = splitUnitsFromHNSecs(SECONDS, hnsecs);

    hnsecs = value.HNSecs();
    hnsecs += convert(HOURS, HNSECS, hour);
    hnsecs += convert(MINUTES, HNSECS, minute);
    hnsecs += convert(SECONDS, HNSECS, second);

    AdjTime(daysHNSecs + hnsecs);
}


XDateTuple XTime::ToXDateTuple() const
{
    return XDateTuple(DayOfGregorianCal());
}


XTimeTuple XTime::ToXTimeTuple() const
{
    int64_t hnsecs = AdjTime();
    hnsecs = removeUnitsFromHNSecs(DAYS, hnsecs);

    const int64_t hour = splitUnitsFromHNSecs(HOURS, hnsecs);
    const int64_t minute = splitUnitsFromHNSecs(MINUTES, hnsecs);
    const int64_t second = getUnitsFromHNSecs(SECONDS, hnsecs);

    return XTimeTuple((int)hour, (int)minute, (int)second);
}


XString XTime::ToISOExtString() const
{
    const int64_t adjustedTime = AdjTime();
    int64_t hnsecs = adjustedTime;
    const int64_t days = splitUnitsFromHNSecs(DAYS, hnsecs) + 1;

    const XDateTuple date = XDateTuple((int)days);
    const XTimeTuple time = XTimeTuple(hnsecs);
    hnsecs = removeUnitsFromHNSecs(SECONDS, hnsecs);
    const XString fracSecStr = XFracSec::ToISOString((int)hnsecs);

    const XString retval = date.ToISOExtString() + "T" + time.ToISOExtString() + fracSecStr;

    if(_tz.Get() == XLocalTime::Instance().Get())
        return retval;

    if(_tz.Get() == XUTC::Instance().Get())
        return retval + "Z";

    const int utcOffset = (int)convert(HNSECS, MINUTES, adjustedTime - _ticks);

    return retval + XSimpleTimeZone::ToISOString(utcOffset);
}


XString XTime::ToMPEG7String() const
{
    const int64_t adjustedTime = AdjTime();
    int64_t hnsecs = adjustedTime;
    const int64_t days = splitUnitsFromHNSecs(DAYS, hnsecs) + 1;

    const XDateTuple date = XDateTuple((int)days);
    const XTimeTuple time = XTimeTuple(hnsecs);
    hnsecs = removeUnitsFromHNSecs(SECONDS, hnsecs);
    const XString fracSecStr = XFracSec::ToMPEG7String((int)hnsecs);

    return date.ToISOExtString() + "T" + time.ToISOExtString() + fracSecStr;
}


XString XTime::ToCustomString(const XString& dateFormat,
                              const XString& timeFormat,
                              const XString& sep,
                              bool dateFirst,
                              size_t numFracDigits) const
{
    const int64_t adjustedTime = AdjTime();
    int64_t hnsecs = adjustedTime;
    const int64_t days = splitUnitsFromHNSecs(DAYS, hnsecs) + 1;

    const XDateTuple date = XDateTuple((int)days);
    const XString dateStr = date.ToCustomString(dateFormat);
    const XTimeTuple time = XTimeTuple(hnsecs);
    const XString timeStr = time.ToCustomString(timeFormat);

    hnsecs = removeUnitsFromHNSecs(SECONDS, hnsecs);
    XString fracSecStr = numFracDigits == 0 ? "" : XFracSec::ToISOString((int)hnsecs);

    if(!fracSecStr.empty())
    {
        if(fracSecStr.size() - 1 > numFracDigits)
        {
            fracSecStr = fracSecStr.substr(0, fracSecStr.size() - (7 - numFracDigits));

            size_t last = fracSecStr.find_last_not_of('0');

            if(last == 0)
                fracSecStr = "";
            else if(last + 1 < fracSecStr.size())
                fracSecStr = fracSecStr.substr(0, last + 1);
        }
    }

    if(dateFirst)
        return dateStr + sep + timeStr + fracSecStr;

    return timeStr + fracSecStr + sep + dateStr;
}


XString XTime::ToXML() const
{
    XDomParser parser;
    XIRef<XDomParserNode> timeNode = new XDomParserNode;
    timeNode->SetTagName("XTime");
    timeNode->AddMetaData("ticks", XString::FromUInt64(_ticks));
    timeNode->AppendChild(_tz->ToXML());

    return parser.GetXmlString(timeNode);
}


XTime XTime::FromISOExtString(const XString& isoExtString)
{
    const XString str = isoExtString.Strip();
    const size_t tDex = str.find('T');

    if(tDex == string::npos)
        X_THROW(("Invalid ISO Extended String: %s", str.c_str()));

    const size_t dotDex = str.find('.');
    const size_t zDex = str.find('Z');
    const size_t plusDex = str.find('+', 1);
    const size_t subDex = str.find('-', str.find('-', str.find('-') + 1) + 1);

    if(plusDex != string::npos && subDex != string::npos)
        X_THROW(("Invalid ISO Extended String: %s", str.c_str()));

    size_t dtEnd = dotDex;

    if(dtEnd == string::npos || dtEnd > zDex)
        dtEnd = zDex;

    if(dtEnd == string::npos || dtEnd > plusDex)
        dtEnd = plusDex;

    if(dtEnd == string::npos || dtEnd > subDex)
        dtEnd = subDex;

    const XString dateStr = str.substr(0, tDex);
    const XString timeStr = str.substr(tDex + 1, (dtEnd - tDex) - 1);
    XString fracSecStr;

    if(dotDex != string::npos)
    {
        size_t fsEnd = zDex;

        if(fsEnd == string::npos || fsEnd > plusDex)
            fsEnd = plusDex;

        if(fsEnd == string::npos || fsEnd > subDex)
            fsEnd = subDex;

        const size_t fsStart = dateStr.size() + 1 + timeStr.size();
        const size_t fsLen = fsEnd == string::npos ? string::npos : fsEnd - fsStart;

        fracSecStr = str.substr(fsStart, fsLen);
    }

    const size_t zoneDex = dateStr.size() + 1 + timeStr.size() + fracSecStr.size();
    const XString zoneStr = zDex == str.size() ? "" : str.substr(zoneDex);

    const XDateTuple date = XDateTuple::FromISOExtString(dateStr);
    const XTimeTuple time = XTimeTuple::FromISOExtString(timeStr);
    const XFracSec fracSec = XFracSec::FromISOString(fracSecStr);
    XIRef<XTimeZone> tz;

    if(zoneStr.empty())
        tz = XLocalTime::Instance();
    else if(zoneStr == "Z")
        tz = XUTC::Instance();
    else
        tz = XSimpleTimeZone::FromISOString(zoneStr);

    return XTime(date, time, fracSec, tz);
}


XTime XTime::FromMPEG7String(const XString& mpeg7String)
{
    const XString str = mpeg7String.Strip();
    const size_t tDex = str.find('T');

    if(tDex == string::npos)
        X_THROW(("Invalid MPEG-7: %s", str.c_str()));

    const size_t lastColonDex = str.find(':', str.find(':', str.find(':') + 1) + 1);

    const XString dateStr = str.substr(0, tDex);
    const XString timeStr = str.substr(tDex + 1, (lastColonDex - tDex) - 1);
    XString fracSecStr;

    if(lastColonDex != string::npos)
    {
        const size_t fsStart = dateStr.size() + 1 + timeStr.size();

        fracSecStr = str.substr(fsStart);
    }

    const XDateTuple date = XDateTuple::FromISOExtString(dateStr);
    const XTimeTuple time = XTimeTuple::FromISOExtString(timeStr);
    const XFracSec fracSec = XFracSec::FromMPEG7String(fracSecStr);

    return XTime(date, time, fracSec, XLocalTime::Instance());
}


//For some reason, I can't get isdigit to work directly.
static bool verifyDigit(char c)
{
    //I should be able to just return the result, but Windows is stupid.
    return isdigit(c) != 0;
}


XTime XTime::FromXML(const XString& xml)
{
    XDomParser parser;
    parser.SetDocument(xml);
    XIRef<XDomParserNode> timeNode = parser.Parse();
    const XString ticksStr = timeNode->GetMetaData("ticks");

    if(timeNode->GetTagName() != "XTime" ||
       ticksStr.empty() ||
       count_if(ticksStr.begin(), ticksStr.end(), verifyDigit) != (int)ticksStr.size() ||
       timeNode->GetChildren().size() != 1)
    {
        X_THROW(("Invalid XML for an XTime: %s", xml.c_str()));
    }

    int64_t ticks = (int64_t)ticksStr.ToUInt64();
    XIRef<XDomParserNode> tzNode = timeNode->GetChildren().front();

    if(tzNode->GetTagName() == "XLocalTime")
        return XTime(ticks, XLocalTime::Instance());
    else if(tzNode->GetTagName() == "XUTC")
        return XTime(ticks, XUTC::Instance());
    else if(tzNode->GetTagName() == "XSimpleTimeZone")
    {
        XIRef<XTimeZone> tz = XSimpleTimeZone::FromXML(tzNode);

        if(!tz.IsValid())
            X_THROW(("Invalid XML for an XTime: %s", xml.c_str()));

        return XTime(ticks, tz);
    }
    else
        X_THROW(("Invalid XML for an XTime: %s", xml.c_str()));
}

