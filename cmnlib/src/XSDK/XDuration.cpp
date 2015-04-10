
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XDuration.h"

#include <algorithm>

using namespace std;
using namespace XSDK;


XString XFracSec::ToISOString(int64_t hnsecs)
{
    X_ASSERT(hnsecs >= 0);
    X_ASSERT(hnsecs < 10000000);
    XString isoString = XString::Format(".%07d", hnsecs);
    size_t len = isoString.size();

    for(; len > 1 && isoString[len - 1] == '0'; --len)
    {}

    if(len == 1)
        return "";

    return isoString.substr(0, len);
}


XString XFracSec::ToMPEG7String(int64_t hnsecs)
{
    if(hnsecs == 0)
        return "";

    X_ASSERT(hnsecs > 0);
    X_ASSERT(hnsecs < 10000000);
    XString denomString = XString::Format("%07d", hnsecs);
    XString numerString = "F10000000";
    const size_t lastDS = denomString.size() - 1;
    const size_t lastNS = numerString.size() - 1;
    size_t extra = 0;

    for(; denomString[lastDS - extra] == '0' && numerString[lastNS - extra] == '0'; ++extra)
    {}

    const size_t zeroes = denomString.find_first_not_of('0');

    return XString(":") +
           denomString.substr(zeroes, (denomString.size() - zeroes) - extra) +
           numerString.substr(0, numerString.size() - extra);
}


//For some reason, I can't get isdigit to work directly.
static bool verifyDigit(char c)
{
    //I should be able to just return the result, but Windows is stupid.
    return isdigit(c) != 0;
}


XFracSec XFracSec::FromISOString(const XString& isoString)
{
    if(isoString.empty())
        return XFracSec::Zero();

    if(!isoString.StartsWith('.'))
        X_THROW(("Invalid ISO Extended String: %s", isoString.c_str()));

    const XString str = isoString.substr(1);

    if(str.empty())
        X_THROW(("Invalid ISO Extended String: %s", isoString.c_str()));

    if(str.size() > 7)
        X_THROW(("Invalid ISO Extended String: %s", isoString.c_str()));

    if(count_if(str.begin(), str.end(), verifyDigit) != (int)str.size())
        X_THROW(("Invalid ISO Extended String: %s", isoString.c_str()));

    XString fullDigitStr = "0000000";

    for(size_t i = 0, e = str.size(); i < e; ++i)
        fullDigitStr[i] = str[i];

    return XFracSec(HNSECS, fullDigitStr.ToInt());
}

XFracSec XFracSec::FromMPEG7String(const XString& mpeg7String)
{
    if(mpeg7String.empty())
        return XFracSec::Zero();

    if(!mpeg7String.StartsWith(':'))
        X_THROW(("Invalid MPEG-7 String: %s", mpeg7String.c_str()));

    const size_t fDex = mpeg7String.find('F');

    if(fDex == string::npos)
        X_THROW(("Invalid MPEG-7 String: %s", mpeg7String.c_str()));

    const XString denomStr = mpeg7String.substr(1, fDex - 1);
    const XString numerStr = mpeg7String.substr(fDex + 1);

    if(denomStr.empty() || numerStr.empty())
        X_THROW(("Invalid MPEG-7 String: %s", mpeg7String.c_str()));

    if(count_if(denomStr.begin(), denomStr.end(), verifyDigit) != (int)denomStr.size() ||
       count_if(numerStr.begin(), numerStr.end(), verifyDigit) != (int)numerStr.size())
    {
        X_THROW(("Invalid MPEG-7 String: %s", mpeg7String.c_str()));
    }

    uint64_t denom = denomStr.ToUInt64();
    const uint64_t numer = numerStr.ToUInt64();

    uint64_t val = numer;
    int zeroes = 0;

    for(; val > 1 && val % 10 == 0; val /= 10, ++zeroes)
    {}

    //We don't want to divide by zero.
    if(val == 0)
        X_THROW(("Invalid MPEG-7 String: %s", mpeg7String.c_str()));

    //Numerator is a factor of 10. I _think_ that this is all we'll ever see,
    //but as far as I can tell, the standard doesn't actually guarantee that
    //the numerator is a factor of 10.
    if(val == 1)
    {
        //divisible by 10
        for(int i = 0, e = 7 - zeroes; i < e; ++i)
            denom *= 10;

        if(denom >= (uint64_t)convert(SECONDS, HNSECS, 1))
            X_THROW(("Invalid MPEG-7 String: %s", mpeg7String.c_str()));

        return XFracSec(HNSECS, (int64_t)denom);
    }

    //Numerator is not a factor of 10.
    const double seconds = (double)denom / (double)numer;

    if(seconds >= 1.0)
        X_THROW(("Invalid MPEG-7 String: %s", mpeg7String.c_str()));

    return XFracSec(HNSECS, (int64_t)(seconds * convert(SECONDS, HNSECS, 1)));
}

XString unitsToPrint(XString units, bool plural)
{
    if(units == "seconds")
        return plural ? "secs" : "sec";
    else if(units == "msecs")
        return "ms";
    else if(units == "usecs")
        return "\u03BCs";
    else
        return plural ? units : units.erase(units.size() - 1);
}

void addUnitStr(XString& retval, int& unitsUsed, int totalUnits, const XString& units, int64_t value)
{
    if(value != 0)
    {
        const XString utp = unitsToPrint(units, value != 1);
        const XString valueStr = XString::FromInt64(value);

        if(unitsUsed != 0)
        {
            if(unitsUsed == totalUnits - 1)
                retval.append(totalUnits == 2 ? " and " : ", and ");
            else
                retval.append(", ");
        }

        retval.append(valueStr);
        retval.append(" ");
        retval.append(utp);

        ++unitsUsed;
    }
}

XString XDuration::ToString() const
{
    int64_t hnsecs = _hnsecs;

    if(hnsecs == 0)
        return "0 hnsecs";

    const int64_t weeks = splitUnitsFromHNSecs(WEEKS, hnsecs);
    const int64_t days = splitUnitsFromHNSecs(DAYS, hnsecs);
    const int64_t hours = splitUnitsFromHNSecs(HOURS, hnsecs);
    const int64_t minutes = splitUnitsFromHNSecs(MINUTES, hnsecs);
    const int64_t seconds = splitUnitsFromHNSecs(SECONDS, hnsecs);
    const int64_t msecs = splitUnitsFromHNSecs(MSECS, hnsecs);
    const int64_t usecs = splitUnitsFromHNSecs(USECS, hnsecs);

    int totalUnits = 0;

    totalUnits += weeks != 0;
    totalUnits += days != 0;
    totalUnits += hours != 0;
    totalUnits += minutes != 0;
    totalUnits += seconds != 0;
    totalUnits += msecs != 0;
    totalUnits += usecs != 0;
    totalUnits += hnsecs != 0;

    XString retval;
    int unitsUsed = 0;

    addUnitStr(retval, unitsUsed, totalUnits, "weeks", weeks);
    addUnitStr(retval, unitsUsed, totalUnits, "days", days);
    addUnitStr(retval, unitsUsed, totalUnits, "hours", hours);
    addUnitStr(retval, unitsUsed, totalUnits, "minutes", minutes);
    addUnitStr(retval, unitsUsed, totalUnits, "seconds", seconds);
    addUnitStr(retval, unitsUsed, totalUnits, "msecs", msecs);
    addUnitStr(retval, unitsUsed, totalUnits, "usecs", usecs);
    addUnitStr(retval, unitsUsed, totalUnits, "hnsecs", hnsecs);

    return retval;
}
