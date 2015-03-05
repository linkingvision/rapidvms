
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XDateTuple_H_
#define _XDateTuple_H_

#include <map>

#include "XSDK/Errors.h"
#include "XSDK/Types.h"
#include "XSDK/XDuration.h"
#include "XSDK/XString.h"

class XDateTupleTest;
class XTimeTupleTest;

namespace XSDK
{
class XLocalTime;
class XTime;

/// Basic type for holding the three parts of a date.
///
/// An XException will be thrown if you ever try and set any of the fields to an invalid value.
class XDateTuple
{
    friend class XLocalTime;
    friend class XTime;
    friend class ::XDateTupleTest;
    friend class XTimeTuple;

public:
    ///
    X_API XDateTuple() : _year(1), _month(1), _day(1) {}

    /// Creates an XDateTuple with the given year, month, and day.
    ///
    /// Throws an XException if the date is invalid.
    X_API XDateTuple(int year, int month, int day);

    ///
    X_API bool operator==(const XDateTuple& rhs) const
    {
        return _year == rhs._year && _month == rhs._month && _day == rhs._day;
    }

    ///
    X_API bool operator!=(const XDateTuple& rhs) const { return !(*this == rhs); }

    ///
    X_API bool operator<(const XDateTuple& rhs) const { return Cmp(rhs) < 0; }

    ///
    X_API bool operator<=(const XDateTuple& rhs) const { return Cmp(rhs) <= 0; }

    ///
    X_API bool operator>=(const XDateTuple& rhs) const { return Cmp(rhs) >= 0; }

    ///
    X_API bool operator>(const XDateTuple& rhs) const { return Cmp(rhs) > 0; }

    ///
    X_API XDuration operator-(const XDateTuple& rhs) const
    {
        return XDuration(DAYS, DayOfGregorianCal() - rhs.DayOfGregorianCal());
    }

    ///
    X_API int Year() const { return _year; }

    ///
    X_API void SetYear(int value);

    ///
    X_API int Month() const { return _month; }

    ///
    X_API void SetMonth(int value);

    ///
    X_API int Day() const { return _day; }

    ///
    X_API void SetDay(int value);

    /// Returns the day of the week. Sunday is 0.
    X_API int DayOfWeek() const;

    /// \brief Returns an XDateTuple which has the same year and month as this one except
    ///        that its day is the last day of the month.
    X_API XDateTuple EndOfMonth() const;

    /// Returns the date in the ISO Extended format: "YYYY-MM-DD".
    X_API XString ToISOExtString() const;

    /// \brief Returns the date in the format specified by the given string.
    ///
    /// The first occurrence of "YYYY" will be replaced with the 4 digit year. If "YYYY" is
    /// not found in the string, then the first occurence of "YY" will be replaced with the
    /// two digit year.
    ///
    /// The first occurence of "MM" will be replaced with the two digit month.
    ///
    /// The first occurence of "DD" will be replaced with the two digit day.
    ///
    /// The rest of the string will be ignored and returned as-is.
    X_API XString ToCustomString(XString format) const;

    /// \brief Takes a string in the ISO Extended format for dates ("YYYY-MM-DD") and
    ///        returns an equivalent XDateTuple.
    ///
    /// The given string will be stripped, so extra whitespace at the beginning or end is fine.
    ///
    /// An XException is thrown if the string is not in the valid format.
    X_API static XDateTuple FromISOExtString(const XString& isoExtString);

private:
    X_API static int MaxDay(int year, int month);

    /// Returns the number of days from the current day of the week to the next given day of the week.
    ///
    /// For example, it's 2 days from Sunday to Tuesday and 3 days from Thursday to Sunday.
    ///
    /// Sunday is 0.
    X_API static int DaysToDayOfWeek(int currDoW, int dow);

    /// Returns the day of the Gregorian Calendar for the given 100ns ticks from midnight, January 1st, 1 A.D.
    X_API static int DayOfGregorianCal(int64_t ticks);

    X_API XDateTuple(int dayOfGregorianCal);

    X_API int Cmp(const XDateTuple& rhs) const;
    X_API int DayOfGregorianCal() const;
    X_API int DayOfYear() const;
    X_API void SetDayOfYear(int day);

    int _year;
    int _month;
    int _day;
};


/// Basic type for holding the three parts of a time of day.
///
/// An XException will be thrown if you ever try and set any of the fields to an invalid value.
class XTimeTuple
{
private:
    friend class XLocalTime;
    friend class XTime;
    friend class ::XTimeTupleTest;

public:
    ///
    X_API XTimeTuple() : _hour(0), _minute(0), _second(0) {}

    /// Creates an XTimeTuple with the given hour, minute, and second.
    ///
    /// Throws an XException if the date is invalid.
    X_API XTimeTuple(int hour, int minute, int second);

    ///
    X_API bool operator==(const XTimeTuple& rhs) const
    {
        return _hour == rhs._hour && _minute == rhs._minute && _second == rhs._second;
    }

    ///
    X_API bool operator!=(const XTimeTuple& rhs) const { return !(*this == rhs); }

    ///
    X_API bool operator<(const XTimeTuple& rhs) const { return Cmp(rhs) < 0; }

    ///
    X_API bool operator<=(const XTimeTuple& rhs) const { return Cmp(rhs) <= 0; }

    ///
    X_API bool operator>=(const XTimeTuple& rhs) const { return Cmp(rhs) >= 0; }

    ///
    X_API bool operator>(const XTimeTuple& rhs) const { return Cmp(rhs) > 0; }

    ///
    X_API XDuration operator-(const XTimeTuple& rhs) const;

    ///
    X_API int Hour() const { return _hour; }

    ///
    X_API void SetHour(int value);

    ///
    X_API int Minute() const { return _minute; }

    ///
    X_API void SetMinute(int value);

    ///
    X_API int Second() const { return _second; }

    ///
    X_API void SetSecond(int value);

    /// Returns the time in the ISO Extended format: "HH:MM:SS".
    X_API XString ToISOExtString() const
    {
        return XString::Format("%02d:%02d:%02d", _hour, _minute, _second);
    }

    /// \brief Returns the time in the format specified by the given string.
    ///
    /// The first occurrence of "HH" will be replaced with the 2 digit hour.
    ///
    /// The first occurence of "MM" will be replaced with the two digit minute.
    ///
    /// The first occurence of "SS" will be replaced with the two digit second.
    ///
    /// The rest of the string will be ignored and returned as-is.
    X_API XString ToCustomString(XString format) const;

    /// \brief Takes a string in the ISO Extended format for times ("HH:MM:SS") and
    ///        returns an equivalent XTimeTuple.
    ///
    /// The given string will be stripped, so extra whitespace at the beginning or end is fine.
    ///
    /// An XException is thrown if the string is not in the valid format.
    X_API static XTimeTuple FromISOExtString(const XString& isoExtString);


    //This is needed in order to allow the free function addDuration access to the constructors that it needs in
    //XDateTuple and XTimeTuple. It really shouldn't be used by anything by the free function addDuration.
    X_API static std::pair<XDateTuple, XTimeTuple> addDuration(const std::pair<XDateTuple, XTimeTuple>& dateTime,
                                                               const XDuration& duration);

private:

    ///Constructs an XTimeTuple from the time portion of the ticks.
    X_API XTimeTuple(int64_t ticks);

    X_API int Cmp(const XTimeTuple& rhs) const;

    int _hour;
    int _minute;
    int _second;
};


/// Returns whether the lhs date and time is less than, equal to, or greater than the rhs date and time
///
/// Returns < 0 if lhs < rhs; 0 if lhs == rhs; > 0 if lhs > rhs.
X_API int cmpDateTime(const std::pair<XDateTuple, XTimeTuple>& lhs,
                      const std::pair<XDateTuple, XTimeTuple>& rhs);


/// Adds the given duration to the given date and time and returns the resulting date and time.
///
/// Returns make_pair(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0)) in cases where the value would
/// become a B.C. date.
inline X_API std::pair<XDateTuple, XTimeTuple> addDuration(const std::pair<XDateTuple, XTimeTuple>& dateTime,
                                                           const XDuration& duration)
{
    return XTimeTuple::addDuration(dateTime, duration);
}


}

#endif
