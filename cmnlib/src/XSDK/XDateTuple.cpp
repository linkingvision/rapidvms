
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XDateTuple.h"

#include <algorithm>

using namespace std;
using namespace XSDK;

const int daysInYear     = 365;  /// The number of days in a non-leap year.
const int daysInLeapYear = 366;  /// The numbef or days in a leap year.
const int daysIn4Years   = daysInYear * 3 + daysInLeapYear;  /// Number of days in 4 years.
const int daysIn100Years = daysIn4Years * 25 - 1;  /// The number of days in 100 years.
const int daysIn400Years = daysIn100Years * 4 + 1; /// The number of days in 400 years.


bool yearIsLeapYear(int year)
{
    if(year % 400 == 0)
        return true;

    if(year % 100 == 0)
        return false;

    return year % 4 == 0;
}


int XDateTuple::MaxDay(int year, int month)
{
    switch(month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 2:
            return yearIsLeapYear(year) ? 29 : 28;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            X_ASSERT(0 && "Invalid month.");
            return 0;
    }
}


int XDateTuple::DaysToDayOfWeek(int currDoW, int dow)
{
    if(currDoW == dow)
        return 0;

    if(currDoW < dow)
        return dow - currDoW;

    return (6 - currDoW) + dow + 1;
}


int XDateTuple::DayOfGregorianCal(int64_t ticks)
{
    X_ASSERT(ticks >= 0);
    return (int)getUnitsFromHNSecs(DAYS, ticks) + 1;
}


XDateTuple::XDateTuple(int year, int month, int day)
    : _year(year),
      _month(month),
      _day(day)
{
    if(year < 1)
        X_THROW(("Years less that 1 not supported"));

    if(month < 1 || month > 12)
        X_THROW(("%d is not a valid month of the year.", month));

    if(day < 0 || day > MaxDay(year, month))
        X_THROW(("%d is not a valid day of the month %d.", day, month));
}


XDateTuple::XDateTuple(int dayOfGregorianCal)
    : _year(0),
      _month(0),
      _day(0)
{
    int day = dayOfGregorianCal;
    int years = (day / daysIn400Years) * 400 + 1;
    day %= daysIn400Years;

    {
        const int tempYears = day / daysIn100Years;

        if(tempYears == 4)
        {
            years += 300;
            day -= daysIn100Years * 3;
        }
        else
        {
            years += tempYears * 100;
            day %= daysIn100Years;
        }
    }

    years += (day / daysIn4Years) * 4;
    day %= daysIn4Years;

    {
        const int tempYears = day / daysInYear;

        if(tempYears == 4)
        {
            years += 3;
            day -= daysInYear * 3;
        }
        else
        {
            years += tempYears;
            day %= daysInYear;
        }
    }

    if(day == 0)
    {
        _year = years - 1;
        _month = 12;
        _day = 31;
    }
    else
    {
        _year = years;
        SetDayOfYear(day);
    }
}


int XDateTuple::Cmp(const XDateTuple& rhs) const
{
    if(_year < rhs._year)
        return true;
    if(_year > rhs._year)
        return 1;

    if(_month < rhs._month)
        return -1;
    if(_month > rhs._month)
        return 1;

    if(_day < rhs._day)
        return -1;
    if(_day > rhs._day)
        return 1;

    return 0;
}


int XDateTuple::DayOfGregorianCal() const
{
    if(_year == 1)
        return DayOfYear();

    int years = _year - 1;
    int days = (years / 400) * daysIn400Years;
    years %= 400;

    days += (years / 100) * daysIn100Years;
    years %= 100;

    days += (years / 4) * daysIn4Years;
    years %= 4;

    days += years * daysInYear;

    days += DayOfYear();

    return days;
}


int XDateTuple::DayOfYear() const
{
    switch(_month)
    {
        case 1:
            return _day;
        case 2:
            return 31 + _day;
        case 3:
            return (yearIsLeapYear(_year) ? 60 : 59) + _day;
        case 4:
            return (yearIsLeapYear(_year) ? 91 : 90) + _day;
        case 5:
            return (yearIsLeapYear(_year) ? 121 : 120) + _day;
        case 6:
            return (yearIsLeapYear(_year) ? 152 : 151) + _day;
        case 7:
            return (yearIsLeapYear(_year) ? 182 : 181) + _day;
        case 8:
            return (yearIsLeapYear(_year) ? 213 : 212) + _day;
        case 9:
            return (yearIsLeapYear(_year) ? 244 : 243) + _day;
        case 10:
            return (yearIsLeapYear(_year) ? 274 : 273) + _day;
        case 11:
            return (yearIsLeapYear(_year) ? 305 : 304) + _day;
        case 12:
            return (yearIsLeapYear(_year) ? 335 : 334) + _day;
        default:
            X_ASSERT(0 && "Invalid month of the year.");
            return 0;
    }
}


void XDateTuple::SetDayOfYear(int day)
{
    if(yearIsLeapYear(_year))
    {
        X_ASSERT(day > 0 && day <= daysInLeapYear && "Invalid day of the year.");

        if(day >= 1 && day <= 31)
        {
            _month = 1;
            _day = day;
        }
        else if(day >= 32 && day <= 60)
        {
            _month = 2;
            _day = day - 31;
        }
        else if(day >= 61 && day <= 91)
        {
            _month = 3;
            _day = day - 60;
        }
        else if(day >= 92 && day <= 121)
        {
            _month = 4;
            _day = day - 91;
        }
        else if(day >= 122 && day <= 152)
        {
            _month = 5;
            _day = day - 121;
        }
        else if(day >= 153 && day <= 182)
        {
            _month = 6;
            _day = day - 152;
        }
        else if(day >= 183 && day <= 213)
        {
            _month = 7;
            _day = day - 182;
        }
        else if(day >= 214 && day <= 244)
        {
            _month = 8;
            _day = day - 213;
        }
        else if(day >= 245 && day <= 274)
        {
            _month = 9;
            _day = day - 244;
        }
        else if(day >= 275 && day <= 305)
        {
            _month = 10;
            _day = day - 274;
        }
        else if(day >= 306 && day <= 335)
        {
            _month = 11;
            _day = day - 305;
        }
        else
        {
            _month = 12;
            _day = day - 335;
        }
    }
    else
    {
        X_ASSERT(day > 0 && day <= daysInYear && "Invalid day of the year.");

        if(day >= 1 && day <= 31)
        {
            _month = 1;
            _day = day;
        }
        else if(day >= 32 && day <= 59)
        {
            _month = 2;
            _day = day - 31;
        }
        else if(day >= 60 && day <= 90)
        {
            _month = 3;
            _day = day - 59;
        }
        else if(day >= 91 && day <= 120)
        {
            _month = 4;
            _day = day - 90;
        }
        else if(day >= 121 && day <= 151)
        {
            _month = 5;
            _day = day - 120;
        }
        else if(day >= 152 && day <= 181)
        {
            _month = 6;
            _day = day - 151;
        }
        else if(day >= 182 && day <= 212)
        {
            _month = 7;
            _day = day - 181;
        }
        else if(day >= 213 && day <= 243)
        {
            _month = 8;
            _day = day - 212;
        }
        else if(day >= 244 && day <= 273)
        {
            _month = 9;
            _day = day - 243;
        }
        else if(day >= 274 && day <= 304)
        {
            _month = 10;
            _day = day - 273;
        }
        else if(day >= 305 && day <= 334)
        {
            _month = 11;
            _day = day - 304;
        }
        else if(day >= 335 && day <= 365)
        {
            _month = 12;
            _day = day - 334;
        }
    }
}


void XDateTuple::SetYear(int value)
{
    if(value < 1)
        X_THROW(("Years less that 1 not supported"));

    if(_day > MaxDay(value, _month))
        X_THROW(("There is no February 29th in %d.", value));

    _year = value;
}


void XDateTuple::SetMonth(int value)
{
    if(value < 1 || value > 12)
        X_THROW(("%d is not a valid month of the year.", value));

    if(_day > MaxDay(_year, value))
        X_THROW(("There is no day %d in month %d in year %d.", _day, value, _year));

    _month = value;
}


void XDateTuple::SetDay(int value)
{
    if(value < 0 || value > MaxDay(_year, _month))
        X_THROW(("%d is not a valid day of the month %d.", value, _month));

    _day = value;
}


int XDateTuple::DayOfWeek() const
{
    const int dayOfG = DayOfGregorianCal();

    //January 1st, 1 A.D. was a Monday
    if(dayOfG >= 0)
        return dayOfG % 7;
    else
    {
        const int dow = (dayOfG % 7) + 7;

        if(dow == 7)
            return 0;
        else
            return dow;
    }
}


XDateTuple XDateTuple::EndOfMonth() const
{
    return XDateTuple(_year, _month, MaxDay(_year, _month));
}


XString XDateTuple::ToISOExtString() const
{
    if(_year < 10000)
        return XString::Format("%04d-%02d-%02d", _year, _month, _day);
    else  //Likely pointless for us, but it _is_ the ISO standard.
        return XString::Format("+%05d-%02d-%02d", _year, _month, _day);
}


XString XDateTuple::ToCustomString(XString format) const
{
    if(_year > 9999)
        X_THROW(("ToCustomString cannot handle years with more than 4 digits: %d.", _year));

    XString yearStr = XString::Format("%04d", _year);
    const XString monthStr = XString::Format("%02d", _month);
    const XString dayStr = XString::Format("%02d", _day);

    size_t yearDex = format.find("YYYY");

    if(yearDex == string::npos)
    {
        yearStr = yearStr.substr(2);
        yearDex = format.find("YY");
    }

    const size_t monthDex = format.find("MM");
    const size_t dayDex = format.find("DD");

    if(yearDex != string::npos)
    {
        for(size_t i = 0, e = yearStr.size(); i < e; ++i)
            format[yearDex + i] = yearStr[i];
    }

    if(monthDex != string::npos)
    {
        for(size_t i = 0, e = monthStr.size(); i < e; ++i)
            format[monthDex + i] = monthStr[i];
    }

    if(dayDex != string::npos)
    {
        for(size_t i = 0, e = dayStr.size(); i < e; ++i)
            format[dayDex + i] = dayStr[i];
    }

    return format;
}


//For some reason, I can't get isdigit to work directly.
static bool verifyDigit(char c)
{
    //I should be able to just return the result, but Windows is stupid.
    return isdigit(c) != 0;
}

XDateTuple XDateTuple::FromISOExtString(const XString& isoExtString)
{
    const XString str = isoExtString.Strip();
    const size_t len = str.size();

    if(len < 10 ||
       str[len - 3] != '-' ||
       str[len - 6] != '-')
    {
        X_THROW(("Invalid ISO Extended String: %s", str.c_str()));
    }

    const XString day = str.substr(len - 2, 2);
    const XString month = str.substr(len - 5, 2);
    XString year = str.substr(0, len - 6);

    if(year[0] == '+' && year.size() < 5)
        X_THROW(("Invalid ISO Extended String: %s", str.c_str()));

    if(year[0] == '-' || year == "0000")
        X_THROW(("XTime does not support B.C.: %s", str.c_str()));

    if(year.size() > 4 && year[0] != '+')
        X_THROW(("Invalid ISO Extended String: %s", str.c_str()));

    if(year[0] == '+')
        year = year.substr(1);

    if(count_if(year.begin(), year.end(), verifyDigit) != (int)year.size() ||
       count_if(month.begin(), month.end(), verifyDigit) != (int)month.size() ||
       count_if(day.begin(), day.end(), verifyDigit) != (int)day.size())
    {
        X_THROW(("Invalid ISO Extended String: %s", str.c_str()));
    }

    return XDateTuple(year.ToInt(), month.ToInt(), day.ToInt());
}


XTimeTuple::XTimeTuple(int hour, int minute, int second)
    : _hour(hour),
      _minute(minute),
      _second(second)
{
    if(hour < 0 || hour > 23)
        X_THROW(("%d is not a valid hour of the day.", _hour));

    if(minute < 0 || minute > 59)
        X_THROW(("%d is not a valid minute of an hour.", _minute));

    if(second < 0 || second > 59)
        X_THROW(("%d is not a valid second of a minute.", _second));
}


XTimeTuple::XTimeTuple(int64_t ticks)
    : _hour(0), _minute(0), _second(0)
{
    ticks = removeUnitsFromHNSecs(DAYS, ticks);
    _hour = (int)splitUnitsFromHNSecs(HOURS, ticks);
    _minute = (int)splitUnitsFromHNSecs(MINUTES, ticks);
    _second = (int)getUnitsFromHNSecs(SECONDS, ticks);
}


int XTimeTuple::Cmp(const XTimeTuple& rhs) const
{
    if(_hour < rhs._hour)
        return true;
    if(_hour > rhs._hour)
        return 1;

    if(_minute < rhs._minute)
        return -1;
    if(_minute > rhs._minute)
        return 1;

    if(_second < rhs._second)
        return -1;
    if(_second > rhs._second)
        return 1;

    return 0;
}


XDuration XTimeTuple::operator-(const XTimeTuple& rhs) const
{
    int lhsSec = _hour * 3600 + _minute * 60 + _second;
    int rhsSec = rhs._hour * 3600 + rhs._minute * 60 + rhs._second;

    return XDuration(SECONDS, lhsSec - rhsSec);
}


void XTimeTuple::SetHour(int value)
{
    if(value < 0 || value > 23)
        X_THROW(("%d is not a valid hour of the day.", value));

    _hour = value;
}


void XTimeTuple::SetMinute(int value)
{
    if(value < 0 || value > 59)
        X_THROW(("%d is not a valid minute of an hour.", value));

    _minute = value;
}


void XTimeTuple::SetSecond(int value)
{
    if(value < 0 || value > 59)
        X_THROW(("%d is not a valid second of a minute.", value));

    _second = value;
}


XString XTimeTuple::ToCustomString(XString format) const
{
    const XString hourStr = XString::Format("%02d", _hour);
    const XString minuteStr = XString::Format("%02d", _minute);
    const XString secondStr = XString::Format("%02d", _second);
    const size_t hourDex = format.find("HH");
    const size_t minuteDex = format.find("MM");
    const size_t secondDex = format.find("SS");

    if(hourDex != string::npos)
    {
        for(size_t i = 0, e = hourStr.size(); i < e; ++i)
            format[hourDex + i] = hourStr[i];
    }

    if(minuteDex != string::npos)
    {
        for(size_t i = 0, e = minuteStr.size(); i < e; ++i)
            format[minuteDex + i] = minuteStr[i];
    }

    if(secondDex != string::npos)
    {
        for(size_t i = 0, e = secondStr.size(); i < e; ++i)
            format[secondDex + i] = secondStr[i];
    }

    return format;
}


XTimeTuple XTimeTuple::FromISOExtString(const XString& isoExtString)
{
    const XString str = isoExtString.Strip();

    if(str.size() != 8 || str[2] != ':' || str[5] != ':')
        X_THROW(("Invalid ISO Extended String: %s", str.c_str()));

    const XString hour = str.substr(0, 2);
    const XString minute = str.substr(3, 2);
    const XString second = str.substr(6, 2);

    if(count_if(hour.begin(), hour.end(), verifyDigit) != (int)hour.size() ||
       count_if(minute.begin(), minute.end(), verifyDigit) != (int)minute.size() ||
       count_if(second.begin(), second.end(), verifyDigit) != (int)second.size())
    {
        X_THROW(("Invalid ISO Extended String: %s", str.c_str()));
    }

    return XTimeTuple(hour.ToInt(), minute.ToInt(), second.ToInt());
}


X_API pair<XDateTuple, XTimeTuple> XTimeTuple::addDuration(const pair<XDateTuple, XTimeTuple>& dateTime,
                                                           const XDuration& duration)
{
    int64_t hnsecs = duration.Total(HNSECS);
    hnsecs += convert(HOURS, HNSECS, dateTime.second.Hour());
    hnsecs += convert(MINUTES, HNSECS, dateTime.second.Minute());
    hnsecs += convert(SECONDS, HNSECS, dateTime.second.Second());
    hnsecs += convert(DAYS, HNSECS, dateTime.first.DayOfGregorianCal());

    const int64_t days = convert(HNSECS, DAYS, hnsecs);

    if(days < 1)
        return make_pair(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0));
    else
        return make_pair(XDateTuple((int)days), XTimeTuple(hnsecs - convert(DAYS, HNSECS, days)));
}


int XSDK::cmpDateTime(const pair<XDateTuple, XTimeTuple>& lhs,
                      const pair<XDateTuple, XTimeTuple>& rhs)
{
    if(lhs.first < rhs.first)
        return -1;
    if(lhs.first > rhs.first)
        return 1;

    if(lhs.second < rhs.second)
        return -1;
    if(lhs.second > rhs.second)
        return 1;

    return 0;
}

