
#include "XStringSliceTest.h"

#include <algorithm>
#include <cstring>
#include <iostream>

#include "XSDK/XStringSlice.h"
#include "XSDK/XException.h"
#include "XSDK/XMemory.h"
#include "XSDK/OS.h"

#ifdef WIN32
#include "WindowsCompliantUnicodeConstants.h"
#else
#include "LinuxCompliantUnicodeConstants.h"
#endif

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XStringSliceTest);

void XStringSliceTest::TestConstructor()
{
    {
        XStringSlice slice;
        UT_ASSERT(slice.size() == 0);
        UT_ASSERT(slice.empty());
        UT_ASSERT(slice == "");
    }
    {
        const XString str = "Foo";
        const XStringSlice slice = &str;

        UT_ASSERT(slice.size() == 3);
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice == str);
    }
}


void XStringSliceTest::TestConstructorUnicode()
{
    const XString str = CHINESE_UNICODE_STRING_1;
    const XStringSlice slice = &str;
    UT_ASSERT(!slice.empty());
    UT_ASSERT(slice == CHINESE_UNICODE_STRING_1);
}


void XStringSliceTest::TestCopyConstructor()
{
    const XString str = "Foo";
    const XStringSlice a = &str;
    const XStringSlice b = a;

    UT_ASSERT(a.size() == 3);
    UT_ASSERT(!a.empty());
    UT_ASSERT(b.size() == 3);
    UT_ASSERT(!b.empty());
    UT_ASSERT(a == b);
}


void XStringSliceTest::TestCopyConstructorUnicode()
{
    const XString str = CHINESE_UNICODE_STRING_2;
    const XStringSlice c = &str;
    const XStringSlice d = c;

    UT_ASSERT(!c.empty());
    UT_ASSERT(!d.empty());
    UT_ASSERT(c.size() == CHINESE_UNICODE_STRING_2_SIZE);
    UT_ASSERT(d.size() == CHINESE_UNICODE_STRING_2_SIZE);
    UT_ASSERT(c == d);
}


void XStringSliceTest::TestAssignmentOperator()
{
    XStringSlice a;
    XStringSlice b;
    XStringSlice c;
    const XString str1 = "hello";
    const XString str2 = "I swear, by my life and my love of it...";

    a = XStringSlice(&str1);
    UT_ASSERT(a == str1);

    b = a;
    UT_ASSERT(a == b);
    UT_ASSERT(a == str1);

    c = XStringSlice(&str2);
    UT_ASSERT(a == str1);
    UT_ASSERT(b == str1);
    UT_ASSERT(c == str2);
}


void XStringSliceTest::TestAssignmentOperatorUnicode()
{
    XStringSlice a;
    XStringSlice b;
    XStringSlice c;
    const XString str1 = CHINESE_UNICODE_STRING_3;
    const XString str2 = "Surround Wei to rescue Zhao";

    a = XStringSlice(&str1);
    UT_ASSERT(a == CHINESE_UNICODE_STRING_3);

    b = a;
    UT_ASSERT(a == b);
    UT_ASSERT(a == str1);

    c = b = XStringSlice(&str2);
    UT_ASSERT(a == str1);
    UT_ASSERT(b == str2);
    UT_ASSERT(c == str2);
}


void XStringSliceTest::TestEqualityOperator()
{
    const XString str1 = "I swear, by my life and my love of it...";
    const XString str2 = "foo";
    const XString str3 = "Hello World";
    const XString str4 = "I swear, by my life and my love of it...";
    const XStringSlice a = &str1;
    const XStringSlice b = &str2;
    const XStringSlice c = &str3;
    const XStringSlice d = &str4;

    const std::string str = "Hello World";
    const char* cstr = "foo";
    static const char cstr2[] = "foo";
    const XString str5 = JAPANESE_UNICODE_STRING_1;
    const XString str6 = JAPANESE_UNICODE_STRING_1;
    const XStringSlice e = &str5;
    const XStringSlice f = &str6;

    UT_ASSERT(a == a);
    UT_ASSERT(!(a == b));
    UT_ASSERT(!(a == c));
    UT_ASSERT(a == d);
    UT_ASSERT(!(a == str));
    UT_ASSERT(!(a == cstr));
    UT_ASSERT(!(a == cstr2));

    UT_ASSERT(!(b == a));
    UT_ASSERT(b == b);
    UT_ASSERT(!(b == c));
    UT_ASSERT(!(b == d));
    UT_ASSERT(!(b == str));
    UT_ASSERT(b == cstr);
    UT_ASSERT(b == cstr2);

    UT_ASSERT(!(c == a));
    UT_ASSERT(!(c == b));
    UT_ASSERT(c == c);
    UT_ASSERT(!(c == d));
    UT_ASSERT(c == str);
    UT_ASSERT(!(c == cstr));
    UT_ASSERT(!(c == cstr2));

    UT_ASSERT(d == a);
    UT_ASSERT(!(d == b));
    UT_ASSERT(!(d == c));
    UT_ASSERT(d == d);
    UT_ASSERT(!(d == str));
    UT_ASSERT(!(d == cstr));
    UT_ASSERT(!(d == cstr2));

    UT_ASSERT(!(str == a));
    UT_ASSERT(!(str == b));
    UT_ASSERT(!(str == d));
    UT_ASSERT(str == c);

    UT_ASSERT(!(e == cstr2));
    UT_ASSERT(e == JAPANESE_UNICODE_STRING_1);
    UT_ASSERT((e == f));
}


void XStringSliceTest::TestInequalityOperator()
{
    const XString str1 = "I swear, by my life and my love of it...";
    const XString str2 = "foo";
    const XString str3 = "Hello World";
    const XString str4 = "I swear, by my life and my love of it...";
    const XStringSlice a = &str1;
    const XStringSlice b = &str2;
    const XStringSlice c = &str3;
    const XStringSlice d = &str4;

    const std::string str = "Hello World";
    const char* cstr = "foo";
    const XString str5 = JAPANESE_UNICODE_STRING_1;
    const XString str6 = JAPANESE_UNICODE_STRING_1;
    const XStringSlice e = &str5;
    const XStringSlice f = &str6;

    UT_ASSERT(!(a != a));
    UT_ASSERT(a != b);
    UT_ASSERT(a != c);
    UT_ASSERT(!(a != d));
    UT_ASSERT(a != str);
    UT_ASSERT(a != cstr);

    UT_ASSERT(b != a);
    UT_ASSERT(!(b != b));
    UT_ASSERT(b != c);
    UT_ASSERT(b != d);
    UT_ASSERT(b != str);
    UT_ASSERT(!(b != cstr));

    UT_ASSERT(c != a);
    UT_ASSERT(c != b);
    UT_ASSERT(!(c != c));
    UT_ASSERT(c != d);
    UT_ASSERT(!(c != str));
    UT_ASSERT(c != cstr);

    UT_ASSERT(!(d != a));
    UT_ASSERT(d != b);
    UT_ASSERT(d != c);
    UT_ASSERT(!(d != d));
    UT_ASSERT(d != str);
    UT_ASSERT(d != cstr);

    UT_ASSERT(!(e != f));
    UT_ASSERT(e != b);
    UT_ASSERT(e != c);
    UT_ASSERT(!(e != e));
    UT_ASSERT(e != str);
    UT_ASSERT(e != cstr);
    UT_ASSERT(!(e != JAPANESE_UNICODE_STRING_1));

    UT_ASSERT(str != a);
    UT_ASSERT(str != b);
    UT_ASSERT(!(str != c));
    UT_ASSERT(str != d);
    UT_ASSERT(str != e);

    UT_ASSERT(cstr != a);
    UT_ASSERT(!(cstr != b));
    UT_ASSERT(cstr != c);
    UT_ASSERT(cstr != d);
    UT_ASSERT(cstr != e);
}

void XStringSliceTest::TestPopX()
{
    {
        XString str = "hello world";
        XStringSlice slice = &str;
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice.size() == str.size());
        UT_ASSERT(slice.front() == 'h');
        UT_ASSERT(slice.back() == 'd');

        slice.popFront();
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice.size() == str.size() - 1);
        UT_ASSERT(slice.front() == 'e');
        UT_ASSERT(slice.back() == 'd');

        slice.popFront();
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice.size() == str.size() - 2);
        UT_ASSERT(slice.front() == 'l');
        UT_ASSERT(slice.back() == 'd');

        slice.popBack();
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice.size() == str.size() - 3);
        UT_ASSERT(slice.front() == 'l');
        UT_ASSERT(slice.back() == 'l');

        slice.popBack();
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice.size() == str.size() - 4);
        UT_ASSERT(slice.front() == 'l');
        UT_ASSERT(slice.back() == 'r');

        slice.popFrontN(3);
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice.size() == str.size() - 7);
        UT_ASSERT(slice.front() == ' ');
        UT_ASSERT(slice.back() == 'r');

        slice.popBackN(3);
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice.size() == 1);
        UT_ASSERT(slice.front() == ' ');
        UT_ASSERT(slice.back() == ' ');

        slice.popFront();
        UT_ASSERT(slice.empty());
        UT_ASSERT(slice.size() == 0);
    }
}


void XStringSliceTest::TestIterators()
{
    {
        XStringSlice slice;
        UT_ASSERT(slice.begin() == slice.end());
        UT_ASSERT(slice.rbegin() == slice.rend());
    }
    {
        XString str = "helbo worfd";
        XStringSlice slice = &str;
        UT_ASSERT(slice.begin() + slice.size() == slice.end());
        UT_ASSERT(slice.rbegin() + slice.size() == slice.rend());
        UT_ASSERT(*slice.begin() == 'h');
        UT_ASSERT(*(slice.rend() - 1) == 'h');
        UT_ASSERT(*slice.rbegin() == 'd');
        UT_ASSERT(*(slice.end() - 1) == 'd');
        UT_ASSERT(equal(slice.begin(), slice.end(), str.begin()));

        slice.popFront();
        UT_ASSERT(*slice.begin() == 'e');
        UT_ASSERT(*(slice.rend() - 1) == 'e');
        UT_ASSERT(*slice.rbegin() == 'd');
        UT_ASSERT(*(slice.end() - 1) == 'd');
        UT_ASSERT(equal(slice.begin(), slice.end(), str.begin() + 1));
        UT_ASSERT(equal(slice.rbegin(), slice.rend(), str.rbegin()));

        slice.popFront();
        UT_ASSERT(*slice.begin() == 'l');
        UT_ASSERT(*(slice.rend() - 1) == 'l');
        UT_ASSERT(*slice.rbegin() == 'd');
        UT_ASSERT(*(slice.end() - 1) == 'd');
        UT_ASSERT(equal(slice.begin(), slice.end(), str.begin() + 2));
        UT_ASSERT(equal(slice.rbegin(), slice.rend(), str.rbegin()));

        slice.popBack();
        UT_ASSERT(*slice.begin() == 'l');
        UT_ASSERT(*(slice.rend() - 1) == 'l');
        UT_ASSERT(*slice.rbegin() == 'f');
        UT_ASSERT(*(slice.end() - 1) == 'f');
        UT_ASSERT(equal(slice.begin(), slice.end(), str.begin() + 2));
        UT_ASSERT(equal(slice.rbegin(), slice.rend(), str.rbegin() + 1));

        slice.popBack();
        UT_ASSERT(*slice.begin() == 'l');
        UT_ASSERT(*(slice.rend() - 1) == 'l');
        UT_ASSERT(*slice.rbegin() == 'r');
        UT_ASSERT(*(slice.end() - 1) == 'r');
        UT_ASSERT(equal(slice.begin(), slice.end(), str.begin() + 2));
        UT_ASSERT(equal(slice.rbegin(), slice.rend(), str.rbegin() + 2));

        slice.popFrontN(6);
        UT_ASSERT(*slice.begin() == 'r');
        UT_ASSERT(*(slice.rend() - 1) == 'r');
        UT_ASSERT(*slice.rbegin() == 'r');
        UT_ASSERT(*(slice.end() - 1) == 'r');
        UT_ASSERT(slice.begin() + 1 == slice.end());
        UT_ASSERT(slice.rbegin() + 1 == slice.rend());
        UT_ASSERT(equal(slice.begin(), slice.end(), str.begin() + 8));
        UT_ASSERT(equal(slice.rbegin(), slice.rend(), str.rbegin() + 2));

        slice.popBack();
        UT_ASSERT(slice.empty());
        UT_ASSERT(slice.begin() == slice.end());
        UT_ASSERT(slice.rbegin() == slice.rend());
    }
}



void XStringSliceTest::TestStartsWith()
{
    {
        const XString str = "I swear, by my life and my love of it...";
        const XStringSlice a = &str;
        const XString start = "I swear, by";
        UT_ASSERT(a.StartsWith(start));
    }
    {
        const XString str = "I swear, by my life and my love of it...";
        const XStringSlice a = &str;
        const std::string start = "I swear, by";
        UT_ASSERT(a.StartsWith(start));
    }
    {
        const XString str = "I swear, by my life and my love of it...";
        const XStringSlice a = &str;
        UT_ASSERT(a.StartsWith("I swear, by"));
    }

    {
        const XString str = "I swear, by my life and my love of it...";
        const XStringSlice a = &str;
        const std::string start = " swear, by";
        UT_ASSERT(!a.StartsWith(start));
    }
    {
        const XString str = "I swear, by my life and my love of it...";
        const XStringSlice a = &str;
        UT_ASSERT(!a.StartsWith(" swear, by"));
    }
}


void XStringSliceTest::TestStartsWithUnicodeVersion()
{
    const XString str = CHINESE_UNICODE_STRING_1;
    const XStringSlice a = &str;
    UT_ASSERT(a.StartsWith(CHINESE_UNICODE_STRING_1_BEGIN));
    UT_ASSERT(!a.StartsWith(CHINESE_UNICODE_STRING_1_MIDDLE));
    UT_ASSERT(!a.StartsWith(CHINESE_UNICODE_STRING_1_END));
}


void XStringSliceTest::TestEndsWith()
{
    {
        const XString str = "I swear, by my life and my love of it...";
        const XStringSlice a = &str;
        const XString end = "love of it...";
        UT_ASSERT(a.EndsWith(end));
    }
    {
        const XString str = "I swear, by my life and my love of it...";
        const XStringSlice a = &str;
        const std::string end = "love of it...";
        UT_ASSERT(a.EndsWith(end));
    }
    {
        const XString str = "I swear, by my life and my love of it...";
        const XStringSlice a = &str;
        UT_ASSERT(a.EndsWith("love of it..."));
    }

    {
        const XString str = "I swear, by my life and my love of it...";
        const XStringSlice a = &str;
        const XString end = "love of it..";
        UT_ASSERT(!a.EndsWith(end));
    }
    {
        const XString str = "I swear, by my life and my love of it...";
        const XStringSlice a = &str;
        const std::string end = "love of it..";
        UT_ASSERT(!a.EndsWith(end));
    }
    {
        const XString str = "I swear, by my life and my love of it...";
        const XStringSlice a = &str;
        UT_ASSERT(!a.EndsWith("love of it.."));
    }
}


void XStringSliceTest::TestEndsWithUnicodeVersion()
{
    const XString str = CHINESE_UNICODE_STRING_1;
    const XStringSlice a = &str;
    UT_ASSERT(!a.EndsWith(CHINESE_UNICODE_STRING_1_BEGIN));
    UT_ASSERT(!a.EndsWith(CHINESE_UNICODE_STRING_1_MIDDLE));
    UT_ASSERT(a.EndsWith(CHINESE_UNICODE_STRING_1_END));
}


void XStringSliceTest::TestToString()
{
    const XString str = "foogly googly foo";

    {
        const XStringSlice slice;
        UT_ASSERT(slice.ToString() == "");
    }
    {
        XStringSlice slice = &str;
        UT_ASSERT(slice.ToString() == "foogly googly foo");
        slice.popFront();
        UT_ASSERT(slice.ToString() == "oogly googly foo");
        slice.popBack();
        UT_ASSERT(slice.ToString() == "oogly googly fo");
        slice.popFrontN(3);
        UT_ASSERT(slice.ToString() == "ly googly fo");
        slice.popBackN(5);
        UT_ASSERT(slice.ToString() == "ly goog");
        slice.popFront();
        UT_ASSERT(slice.ToString() == "y goog");
        slice.popFrontN(4);
        UT_ASSERT(slice.ToString() == "og");
        slice.popFront();
        slice.popFront();
        UT_ASSERT(slice.ToString() == "");
        UT_ASSERT(slice.empty());
    }
    {
        const XStringSlice slice = &str;
        UT_ASSERT(slice.ToString(0, slice.size()) == "foogly googly foo");
        UT_ASSERT(slice.ToString(1, slice.size()) == "oogly googly foo");
        UT_ASSERT(slice.ToString(0, slice.size() - 1) == "foogly googly fo");
        UT_ASSERT(slice.ToString(4, 10) == "ly goo");
        UT_ASSERT(slice.ToString(0, 0) == "");
        UT_ASSERT(slice.ToString(12, 12) == "");
        UT_ASSERT(slice.ToString(slice.size(), slice.size()) == "");
    }
    {
        XStringSlice slice = &str;
        slice.popFrontN(3);
        slice.popBackN(4);
        UT_ASSERT(slice == "gly googly");
        UT_ASSERT(slice.ToString() == "gly googly");
        UT_ASSERT(slice.ToString(0, slice.size()) == "gly googly");
        UT_ASSERT(slice.ToString(1, slice.size()) == "ly googly");
        UT_ASSERT(slice.ToString(0, slice.size() - 1) == "gly googl");
        UT_ASSERT(slice.ToString(4, 7) == "goo");
        UT_ASSERT(slice.ToString(0, 0) == "");
        UT_ASSERT(slice.ToString(2, 2) == "");
        UT_ASSERT(slice.ToString(slice.size(), slice.size()) == "");
    }
    {
        const XString russianStr = RUSSIAN_UNICODE_TO_BE_SEARCHED;
        const XString sought = RUSSIAN_UNICODE_TO_BE_FOUND;
        const XStringSlice russian = &russianStr;
        const size_t found = russianStr.find(sought);
        UT_ASSERT(russian.ToString(found, found + sought.size()) == sought);
    }
}


void XStringSliceTest::TestSlice()
{
    {
        const XStringSlice slice;
        UT_ASSERT(slice.Slice() == "");
        UT_ASSERT(slice.Slice(0, 0) == "");
    }

    const XString str = "foogly googly foo";

    {
        const XStringSlice slice = &str;
        UT_ASSERT(slice.Slice() == "foogly googly foo");
        UT_ASSERT(slice.Slice(0, slice.size()) == "foogly googly foo");
        UT_ASSERT(slice.Slice(1, slice.size()) == "oogly googly foo");
        UT_ASSERT(slice.Slice(0, slice.size() - 1) == "foogly googly fo");
        UT_ASSERT(slice.Slice(4, 10) == "ly goo");
        UT_ASSERT(slice.Slice(0, 0) == "");
        UT_ASSERT(slice.Slice(12, 12) == "");
        UT_ASSERT(slice.Slice(slice.size(), slice.size()) == "");
    }
    {
        XStringSlice slice = &str;
        slice.popFrontN(3);
        slice.popBackN(4);
        UT_ASSERT(slice == "gly googly");
        UT_ASSERT(slice.Slice() == "gly googly");
        UT_ASSERT(slice.Slice(0, slice.size()) == "gly googly");
        UT_ASSERT(slice.Slice(1, slice.size()) == "ly googly");
        UT_ASSERT(slice.Slice(0, slice.size() - 1) == "gly googl");
        UT_ASSERT(slice.Slice(4, 7) == "goo");
        UT_ASSERT(slice.Slice(0, 0) == "");
        UT_ASSERT(slice.Slice(2, 2) == "");
        UT_ASSERT(slice.Slice(slice.size(), slice.size()) == "");

        UT_ASSERT(slice.Slice(slice.begin(), slice.end()) == "gly googly");
        UT_ASSERT(slice.Slice(slice.begin() + 1, slice.end()) == "ly googly");
        UT_ASSERT(slice.Slice(slice.begin(), slice.end() - 1) == "gly googl");
        UT_ASSERT(slice.Slice(slice.begin() + 4, slice.begin() + 7) == "goo");
        UT_ASSERT(slice.Slice(slice.begin(), slice.begin()) == "");
        UT_ASSERT(slice.Slice(slice.begin() + 2, slice.begin() + 2) == "");
        UT_ASSERT(slice.Slice(slice.end(), slice.end()) == "");

        const XStringSlice other = slice.Slice(3, 8);
        UT_ASSERT(other == " goog");
        UT_ASSERT(other.size() == 5);
        UT_ASSERT(other.front() == ' ');
        UT_ASSERT(other.back() == 'g');
        UT_ASSERT(other == slice.Slice(slice.begin() + 3, slice.begin() + 8));
    }
    {
        const XString russianStr = RUSSIAN_UNICODE_TO_BE_SEARCHED;
        const XString sought = RUSSIAN_UNICODE_TO_BE_FOUND;
        const XStringSlice russian = &russianStr;
        const size_t found = russianStr.find(sought);
        UT_ASSERT(russian.Slice(found, found + sought.size()) == sought);
    }
}


void XStringSliceTest::TestFind()
{
    const XString str = "I swear, by my life and my love of it...";
    const XStringSlice haystack = &str;
    const XStringSlice slice = haystack.Slice(11, 19);

    {
        const XString needle = "my";
        const XString missing = "myl";
        UT_ASSERT(haystack.Find(needle) == "my life and my love of it...");
        UT_ASSERT(slice.Find(needle) == "my life");
        UT_ASSERT(haystack.Find(missing).empty());
        UT_ASSERT(slice.Find(missing).empty());
    }
    {
        const std::string needle = "my";
        const std::string missing = "myl";
        UT_ASSERT(haystack.Find(needle) == "my life and my love of it...");
        UT_ASSERT(slice.Find(needle) == "my life");
        UT_ASSERT(haystack.Find(missing).empty());
        UT_ASSERT(slice.Find(missing).empty());
    }
    {
        const char* needle = "my";
        const char* missing = "myl";
        UT_ASSERT(haystack.Find(needle) == "my life and my love of it...");
        UT_ASSERT(slice.Find(needle) == "my life");
        UT_ASSERT(haystack.Find(missing).empty());
        UT_ASSERT(slice.Find(missing).empty());
    }
    {
        const char needle = 'm';
        const char missing = 'q';
        UT_ASSERT(haystack.Find(needle) == "my life and my love of it...");
        UT_ASSERT(slice.Find(needle) == "my life");
        UT_ASSERT(haystack.Find(missing).empty());
        UT_ASSERT(slice.Find(missing).empty());
    }
}


void XStringSliceTest::TestFindUntil()
{
    const XString str = "I swear, by my life and my love of it...";
    const XStringSlice haystack = &str;
    const XStringSlice slice = haystack.Slice(5, 19);

    {
        const XString needle = "my";
        const XString missing = "myl";
        UT_ASSERT(haystack.FindUntil(needle) == "I swear, by ");
        UT_ASSERT(haystack.FindUntil(needle, true) == "I swear, by my");
        UT_ASSERT(slice.FindUntil(needle) == "ar, by ");
        UT_ASSERT(slice.FindUntil(needle, true) == "ar, by my");
        UT_ASSERT(haystack.FindUntil(missing) == haystack);
        UT_ASSERT(haystack.FindUntil(missing, false) ==haystack);
        UT_ASSERT(slice.FindUntil(missing) == slice);
        UT_ASSERT(slice.FindUntil(missing)  == slice);
    }
    {
        const std::string needle = "my";
        const std::string missing = "myl";
        UT_ASSERT(haystack.FindUntil(needle) == "I swear, by ");
        UT_ASSERT(haystack.FindUntil(needle, true) == "I swear, by my");
        UT_ASSERT(slice.FindUntil(needle) == "ar, by ");
        UT_ASSERT(slice.FindUntil(needle, true) == "ar, by my");
        UT_ASSERT(haystack.FindUntil(missing) == haystack);
        UT_ASSERT(haystack.FindUntil(missing, false) ==haystack);
        UT_ASSERT(slice.FindUntil(missing) == slice);
        UT_ASSERT(slice.FindUntil(missing)  == slice);
    }
    {
        const char* needle = "my";
        const char* missing = "myl";
        UT_ASSERT(haystack.FindUntil(needle) == "I swear, by ");
        UT_ASSERT(haystack.FindUntil(needle, true) == "I swear, by my");
        UT_ASSERT(slice.FindUntil(needle) == "ar, by ");
        UT_ASSERT(slice.FindUntil(needle, true) == "ar, by my");
        UT_ASSERT(haystack.FindUntil(missing) == haystack);
        UT_ASSERT(haystack.FindUntil(missing, false) ==haystack);
        UT_ASSERT(slice.FindUntil(missing) == slice);
        UT_ASSERT(slice.FindUntil(missing)  == slice);
    }
    {
        const char needle = 'm';
        const char missing = 'q';
        UT_ASSERT(haystack.FindUntil(needle) == "I swear, by ");
        UT_ASSERT(haystack.FindUntil(needle, true) == "I swear, by m");
        UT_ASSERT(slice.FindUntil(needle) == "ar, by ");
        UT_ASSERT(slice.FindUntil(needle, true) == "ar, by m");
        UT_ASSERT(haystack.FindUntil(missing) == haystack);
        UT_ASSERT(haystack.FindUntil(missing, false) ==haystack);
        UT_ASSERT(slice.FindUntil(missing) == slice);
        UT_ASSERT(slice.FindUntil(missing)  == slice);
    }

    {
        const XString needle1 = "I";
        const XString needle2 = "a";
        UT_ASSERT(haystack.FindUntil(needle1) == "");
        UT_ASSERT(haystack.FindUntil(needle1, true) == "I");
        UT_ASSERT(slice.FindUntil(needle2) == "");
        UT_ASSERT(slice.FindUntil(needle2, true) == "a");
    }
    {
        const std::string needle1 = "I";
        const std::string needle2 = "a";
        UT_ASSERT(haystack.FindUntil(needle1) == "");
        UT_ASSERT(haystack.FindUntil(needle1, true) == "I");
        UT_ASSERT(slice.FindUntil(needle2) == "");
        UT_ASSERT(slice.FindUntil(needle2, true) == "a");
    }
    {
        const char* needle1 = "I";
        const char* needle2 = "a";
        UT_ASSERT(haystack.FindUntil(needle1) == "");
        UT_ASSERT(haystack.FindUntil(needle1, true) == "I");
        UT_ASSERT(slice.FindUntil(needle2) == "");
        UT_ASSERT(slice.FindUntil(needle2, true) == "a");
    }
    {
        const char needle1 = 'I';
        const char needle2 = 'a';
        UT_ASSERT(haystack.FindUntil(needle1) == "");
        UT_ASSERT(haystack.FindUntil(needle1, true) == "I");
        UT_ASSERT(slice.FindUntil(needle2) == "");
        UT_ASSERT(slice.FindUntil(needle2, true) == "a");
    }
}
