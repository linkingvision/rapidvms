//=============================================================================
//Copyright (c) 2013 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XStringSliceTest.cpp $
// $Revision:$
// $Date:$
// $Author: Jonathan M Davis $
//=============================================================================

#include "ck_string_slice_test.h"

#include <algorithm>
#include <cstring>
#include <iostream>

#include "cppkit/ck_string_slice.h"
#include "cppkit/ck_exception.h"
#include "cppkit/ck_memory.h"
#include "cppkit/os/ck_platform.h"

#ifdef WIN32
#include "windows_compliant_unicode_constants.h"
#else
#include "linux_compliant_unicode_constants.h"
#endif

using namespace std;
using namespace cppkit;

REGISTER_TEST_FIXTURE(ck_string_slice_test);

void ck_string_slice_test::test_constructor()
{
    {
        ck_string_slice slice;
        UT_ASSERT(slice.size() == 0);
        UT_ASSERT(slice.empty());
        UT_ASSERT(slice == "");
    }
    {
        const ck_string str = "Foo";
        const ck_string_slice slice = &str;

        UT_ASSERT(slice.size() == 3);
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice == str);
    }
}


void ck_string_slice_test::test_constructor_unicode()
{
    const ck_string str = CHINESE_UNICODE_STRING_1;
    const ck_string_slice slice = &str;
    UT_ASSERT(!slice.empty());
    UT_ASSERT(slice == CHINESE_UNICODE_STRING_1);
}


void ck_string_slice_test::test_copy_constructor()
{
    const ck_string str = "Foo";
    const ck_string_slice a = &str;
    const ck_string_slice b = a;

    UT_ASSERT(a.size() == 3);
    UT_ASSERT(!a.empty());
    UT_ASSERT(b.size() == 3);
    UT_ASSERT(!b.empty());
    UT_ASSERT(a == b);
}


void ck_string_slice_test::test_copy_constructor_unicode()
{
    const ck_string str = CHINESE_UNICODE_STRING_2;
    const ck_string_slice c = &str;
    const ck_string_slice d = c;

    UT_ASSERT(!c.empty());
    UT_ASSERT(!d.empty());
    UT_ASSERT(c.size() == CHINESE_UNICODE_STRING_2_SIZE);
    UT_ASSERT(d.size() == CHINESE_UNICODE_STRING_2_SIZE);
    UT_ASSERT(c == d);
}


void ck_string_slice_test::test_assignment_operator()
{
    ck_string_slice a;
    ck_string_slice b;
    ck_string_slice c;
    const ck_string str1 = "hello";
    const ck_string str2 = "I swear, by my life and my love of it...";

    a = ck_string_slice(&str1);
    UT_ASSERT(a == str1);

    b = a;
    UT_ASSERT(a == b);
    UT_ASSERT(a == str1);

    c = ck_string_slice(&str2);
    UT_ASSERT(a == str1);
    UT_ASSERT(b == str1);
    UT_ASSERT(c == str2);
}


void ck_string_slice_test::test_assignment_operator_unicode()
{
    ck_string_slice a;
    ck_string_slice b;
    ck_string_slice c;
    const ck_string str1 = CHINESE_UNICODE_STRING_3;
    const ck_string str2 = "Surround Wei to rescue Zhao";

    a = ck_string_slice(&str1);
    UT_ASSERT(a == CHINESE_UNICODE_STRING_3);

    b = a;
    UT_ASSERT(a == b);
    UT_ASSERT(a == str1);

    c = b = ck_string_slice(&str2);
    UT_ASSERT(a == str1);
    UT_ASSERT(b == str2);
    UT_ASSERT(c == str2);
}


void ck_string_slice_test::test_equality_operator()
{
    const ck_string str1 = "I swear, by my life and my love of it...";
    const ck_string str2 = "foo";
    const ck_string str3 = "Hello World";
    const ck_string str4 = "I swear, by my life and my love of it...";
    const ck_string_slice a = &str1;
    const ck_string_slice b = &str2;
    const ck_string_slice c = &str3;
    const ck_string_slice d = &str4;

    const std::string str = "Hello World";
    const char* cstr = "foo";
    static const char cstr2[] = "foo";
    const ck_string str5 = JAPANESE_UNICODE_STRING_1;
    const ck_string str6 = JAPANESE_UNICODE_STRING_1;
    const ck_string_slice e = &str5;
    const ck_string_slice f = &str6;

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


void ck_string_slice_test::test_inequality_operator()
{
    const ck_string str1 = "I swear, by my life and my love of it...";
    const ck_string str2 = "foo";
    const ck_string str3 = "Hello World";
    const ck_string str4 = "I swear, by my life and my love of it...";
    const ck_string_slice a = &str1;
    const ck_string_slice b = &str2;
    const ck_string_slice c = &str3;
    const ck_string_slice d = &str4;

    const std::string str = "Hello World";
    const char* cstr = "foo";
    const ck_string str5 = JAPANESE_UNICODE_STRING_1;
    const ck_string str6 = JAPANESE_UNICODE_STRING_1;
    const ck_string_slice e = &str5;
    const ck_string_slice f = &str6;

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

void ck_string_slice_test::test_pop_x()
{
    {
        ck_string str = "hello world";
        ck_string_slice slice = &str;
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice.size() == str.size());
        UT_ASSERT(slice.front() == 'h');
        UT_ASSERT(slice.back() == 'd');

        slice.pop_front();
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice.size() == str.size() - 1);
        UT_ASSERT(slice.front() == 'e');
        UT_ASSERT(slice.back() == 'd');

        slice.pop_front();
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice.size() == str.size() - 2);
        UT_ASSERT(slice.front() == 'l');
        UT_ASSERT(slice.back() == 'd');

        slice.pop_back();
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice.size() == str.size() - 3);
        UT_ASSERT(slice.front() == 'l');
        UT_ASSERT(slice.back() == 'l');

        slice.pop_back();
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice.size() == str.size() - 4);
        UT_ASSERT(slice.front() == 'l');
        UT_ASSERT(slice.back() == 'r');

        slice.pop_front_n(3);
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice.size() == str.size() - 7);
        UT_ASSERT(slice.front() == ' ');
        UT_ASSERT(slice.back() == 'r');

        slice.pop_back_n(3);
        UT_ASSERT(!slice.empty());
        UT_ASSERT(slice.size() == 1);
        UT_ASSERT(slice.front() == ' ');
        UT_ASSERT(slice.back() == ' ');

        slice.pop_front();
        UT_ASSERT(slice.empty());
        UT_ASSERT(slice.size() == 0);
    }
}


void ck_string_slice_test::test_iterators()
{
    {
        ck_string_slice slice;
        UT_ASSERT(slice.begin() == slice.end());
        UT_ASSERT(slice.rbegin() == slice.rend());
    }
    {
        ck_string str = "helbo worfd";
        ck_string_slice slice = &str;
        UT_ASSERT(slice.begin() + slice.size() == slice.end());
        UT_ASSERT(slice.rbegin() + slice.size() == slice.rend());
        UT_ASSERT(*slice.begin() == 'h');
        UT_ASSERT(*(slice.rend() - 1) == 'h');
        UT_ASSERT(*slice.rbegin() == 'd');
        UT_ASSERT(*(slice.end() - 1) == 'd');
        UT_ASSERT(equal(slice.begin(), slice.end(), str.begin()));

        slice.pop_front();
        UT_ASSERT(*slice.begin() == 'e');
        UT_ASSERT(*(slice.rend() - 1) == 'e');
        UT_ASSERT(*slice.rbegin() == 'd');
        UT_ASSERT(*(slice.end() - 1) == 'd');
        UT_ASSERT(equal(slice.begin(), slice.end(), str.begin() + 1));
        UT_ASSERT(equal(slice.rbegin(), slice.rend(), str.rbegin()));

        slice.pop_front();
        UT_ASSERT(*slice.begin() == 'l');
        UT_ASSERT(*(slice.rend() - 1) == 'l');
        UT_ASSERT(*slice.rbegin() == 'd');
        UT_ASSERT(*(slice.end() - 1) == 'd');
        UT_ASSERT(equal(slice.begin(), slice.end(), str.begin() + 2));
        UT_ASSERT(equal(slice.rbegin(), slice.rend(), str.rbegin()));

        slice.pop_back();
        UT_ASSERT(*slice.begin() == 'l');
        UT_ASSERT(*(slice.rend() - 1) == 'l');
        UT_ASSERT(*slice.rbegin() == 'f');
        UT_ASSERT(*(slice.end() - 1) == 'f');
        UT_ASSERT(equal(slice.begin(), slice.end(), str.begin() + 2));
        UT_ASSERT(equal(slice.rbegin(), slice.rend(), str.rbegin() + 1));

        slice.pop_back();
        UT_ASSERT(*slice.begin() == 'l');
        UT_ASSERT(*(slice.rend() - 1) == 'l');
        UT_ASSERT(*slice.rbegin() == 'r');
        UT_ASSERT(*(slice.end() - 1) == 'r');
        UT_ASSERT(equal(slice.begin(), slice.end(), str.begin() + 2));
        UT_ASSERT(equal(slice.rbegin(), slice.rend(), str.rbegin() + 2));

        slice.pop_front_n(6);
        UT_ASSERT(*slice.begin() == 'r');
        UT_ASSERT(*(slice.rend() - 1) == 'r');
        UT_ASSERT(*slice.rbegin() == 'r');
        UT_ASSERT(*(slice.end() - 1) == 'r');
        UT_ASSERT(slice.begin() + 1 == slice.end());
        UT_ASSERT(slice.rbegin() + 1 == slice.rend());
        UT_ASSERT(equal(slice.begin(), slice.end(), str.begin() + 8));
        UT_ASSERT(equal(slice.rbegin(), slice.rend(), str.rbegin() + 2));

        slice.pop_back();
        UT_ASSERT(slice.empty());
        UT_ASSERT(slice.begin() == slice.end());
        UT_ASSERT(slice.rbegin() == slice.rend());
    }
}



void ck_string_slice_test::test_starts_with()
{
    {
        const ck_string str = "I swear, by my life and my love of it...";
        const ck_string_slice a = &str;
        const ck_string start = "I swear, by";
        UT_ASSERT(a.starts_with(start));
    }
    {
        const ck_string str = "I swear, by my life and my love of it...";
        const ck_string_slice a = &str;
        const std::string start = "I swear, by";
        UT_ASSERT(a.starts_with(start));
    }
    {
        const ck_string str = "I swear, by my life and my love of it...";
        const ck_string_slice a = &str;
        UT_ASSERT(a.starts_with("I swear, by"));
    }

    {
        const ck_string str = "I swear, by my life and my love of it...";
        const ck_string_slice a = &str;
        const std::string start = " swear, by";
        UT_ASSERT(!a.starts_with(start));
    }
    {
        const ck_string str = "I swear, by my life and my love of it...";
        const ck_string_slice a = &str;
        UT_ASSERT(!a.starts_with(" swear, by"));
    }
}


void ck_string_slice_test::test_starts_with_unicode_version()
{
    const ck_string str = CHINESE_UNICODE_STRING_1;
    const ck_string_slice a = &str;
    UT_ASSERT(a.starts_with(CHINESE_UNICODE_STRING_1_BEGIN));
    UT_ASSERT(!a.starts_with(CHINESE_UNICODE_STRING_1_MIDDLE));
    UT_ASSERT(!a.starts_with(CHINESE_UNICODE_STRING_1_END));
}


void ck_string_slice_test::test_ends_with()
{
    {
        const ck_string str = "I swear, by my life and my love of it...";
        const ck_string_slice a = &str;
        const ck_string end = "love of it...";
        UT_ASSERT(a.ends_with(end));
    }
    {
        const ck_string str = "I swear, by my life and my love of it...";
        const ck_string_slice a = &str;
        const std::string end = "love of it...";
        UT_ASSERT(a.ends_with(end));
    }
    {
        const ck_string str = "I swear, by my life and my love of it...";
        const ck_string_slice a = &str;
        UT_ASSERT(a.ends_with("love of it..."));
    }

    {
        const ck_string str = "I swear, by my life and my love of it...";
        const ck_string_slice a = &str;
        const ck_string end = "love of it..";
        UT_ASSERT(!a.ends_with(end));
    }
    {
        const ck_string str = "I swear, by my life and my love of it...";
        const ck_string_slice a = &str;
        const std::string end = "love of it..";
        UT_ASSERT(!a.ends_with(end));
    }
    {
        const ck_string str = "I swear, by my life and my love of it...";
        const ck_string_slice a = &str;
        UT_ASSERT(!a.ends_with("love of it.."));
    }
}


void ck_string_slice_test::test_ends_with_unicode_version()
{
    const ck_string str = CHINESE_UNICODE_STRING_1;
    const ck_string_slice a = &str;
    UT_ASSERT(!a.ends_with(CHINESE_UNICODE_STRING_1_BEGIN));
    UT_ASSERT(!a.ends_with(CHINESE_UNICODE_STRING_1_MIDDLE));
    UT_ASSERT(a.ends_with(CHINESE_UNICODE_STRING_1_END));
}


void ck_string_slice_test::test_to_string()
{
    const ck_string str = "foogly googly foo";

    {
        const ck_string_slice slice;
        UT_ASSERT(slice.to_string() == "");
    }
    {
        ck_string_slice slice = &str;
        UT_ASSERT(slice.to_string() == "foogly googly foo");
        slice.pop_front();
        UT_ASSERT(slice.to_string() == "oogly googly foo");
        slice.pop_back();
        UT_ASSERT(slice.to_string() == "oogly googly fo");
        slice.pop_front_n(3);
        UT_ASSERT(slice.to_string() == "ly googly fo");
        slice.pop_back_n(5);
        UT_ASSERT(slice.to_string() == "ly goog");
        slice.pop_front();
        UT_ASSERT(slice.to_string() == "y goog");
        slice.pop_front_n(4);
        UT_ASSERT(slice.to_string() == "og");
        slice.pop_front();
        slice.pop_front();
        UT_ASSERT(slice.to_string() == "");
        UT_ASSERT(slice.empty());
    }
    {
        const ck_string_slice slice = &str;
        UT_ASSERT(slice.to_string(0, slice.size()) == "foogly googly foo");
        UT_ASSERT(slice.to_string(1, slice.size()) == "oogly googly foo");
        UT_ASSERT(slice.to_string(0, slice.size() - 1) == "foogly googly fo");
        UT_ASSERT(slice.to_string(4, 10) == "ly goo");
        UT_ASSERT(slice.to_string(0, 0) == "");
        UT_ASSERT(slice.to_string(12, 12) == "");
        UT_ASSERT(slice.to_string(slice.size(), slice.size()) == "");
    }
    {
        ck_string_slice slice = &str;
        slice.pop_front_n(3);
        slice.pop_back_n(4);
        UT_ASSERT(slice == "gly googly");
        UT_ASSERT(slice.to_string() == "gly googly");
        UT_ASSERT(slice.to_string(0, slice.size()) == "gly googly");
        UT_ASSERT(slice.to_string(1, slice.size()) == "ly googly");
        UT_ASSERT(slice.to_string(0, slice.size() - 1) == "gly googl");
        UT_ASSERT(slice.to_string(4, 7) == "goo");
        UT_ASSERT(slice.to_string(0, 0) == "");
        UT_ASSERT(slice.to_string(2, 2) == "");
        UT_ASSERT(slice.to_string(slice.size(), slice.size()) == "");
    }
    {
        const ck_string russianStr = RUSSIAN_UNICODE_TO_BE_SEARCHED;
        const ck_string sought = RUSSIAN_UNICODE_TO_BE_FOUND;
        const ck_string_slice russian = &russianStr;
        const size_t found = russianStr.find(sought);
        UT_ASSERT(russian.to_string(found, found + sought.size()) == sought);
    }
}


void ck_string_slice_test::test_slice()
{
    {
        const ck_string_slice slice;
        UT_ASSERT(slice.slice() == "");
        UT_ASSERT(slice.slice(0, 0) == "");
    }

    const ck_string str = "foogly googly foo";

    {
        const ck_string_slice slice = &str;
        UT_ASSERT(slice.slice() == "foogly googly foo");
        UT_ASSERT(slice.slice(0, slice.size()) == "foogly googly foo");
        UT_ASSERT(slice.slice(1, slice.size()) == "oogly googly foo");
        UT_ASSERT(slice.slice(0, slice.size() - 1) == "foogly googly fo");
        UT_ASSERT(slice.slice(4, 10) == "ly goo");
        UT_ASSERT(slice.slice(0, 0) == "");
        UT_ASSERT(slice.slice(12, 12) == "");
        UT_ASSERT(slice.slice(slice.size(), slice.size()) == "");
    }
    {
        ck_string_slice slice = &str;
        slice.pop_front_n(3);
        slice.pop_back_n(4);
        UT_ASSERT(slice == "gly googly");
        UT_ASSERT(slice.slice() == "gly googly");
        UT_ASSERT(slice.slice(0, slice.size()) == "gly googly");
        UT_ASSERT(slice.slice(1, slice.size()) == "ly googly");
        UT_ASSERT(slice.slice(0, slice.size() - 1) == "gly googl");
        UT_ASSERT(slice.slice(4, 7) == "goo");
        UT_ASSERT(slice.slice(0, 0) == "");
        UT_ASSERT(slice.slice(2, 2) == "");
        UT_ASSERT(slice.slice(slice.size(), slice.size()) == "");

        UT_ASSERT(slice.slice(slice.begin(), slice.end()) == "gly googly");
        UT_ASSERT(slice.slice(slice.begin() + 1, slice.end()) == "ly googly");
        UT_ASSERT(slice.slice(slice.begin(), slice.end() - 1) == "gly googl");
        UT_ASSERT(slice.slice(slice.begin() + 4, slice.begin() + 7) == "goo");
        UT_ASSERT(slice.slice(slice.begin(), slice.begin()) == "");
        UT_ASSERT(slice.slice(slice.begin() + 2, slice.begin() + 2) == "");
        UT_ASSERT(slice.slice(slice.end(), slice.end()) == "");

        const ck_string_slice other = slice.slice(3, 8);
        UT_ASSERT(other == " goog");
        UT_ASSERT(other.size() == 5);
        UT_ASSERT(other.front() == ' ');
        UT_ASSERT(other.back() == 'g');
        UT_ASSERT(other == slice.slice(slice.begin() + 3, slice.begin() + 8));
    }
    {
        const ck_string russianStr = RUSSIAN_UNICODE_TO_BE_SEARCHED;
        const ck_string sought = RUSSIAN_UNICODE_TO_BE_FOUND;
        const ck_string_slice russian = &russianStr;
        const size_t found = russianStr.find(sought);
        UT_ASSERT(russian.slice(found, found + sought.size()) == sought);
    }
}


void ck_string_slice_test::test_find()
{
    const ck_string str = "I swear, by my life and my love of it...";
    const ck_string_slice haystack = &str;
    const ck_string_slice slice = haystack.slice(11, 19);

    {
        const ck_string needle = "my";
        const ck_string missing = "myl";
        UT_ASSERT(haystack.find(needle) == "my life and my love of it...");
        UT_ASSERT(slice.find(needle) == "my life");
        UT_ASSERT(haystack.find(missing).empty());
        UT_ASSERT(slice.find(missing).empty());
    }
    {
        const std::string needle = "my";
        const std::string missing = "myl";
        UT_ASSERT(haystack.find(needle) == "my life and my love of it...");
        UT_ASSERT(slice.find(needle) == "my life");
        UT_ASSERT(haystack.find(missing).empty());
        UT_ASSERT(slice.find(missing).empty());
    }
    {
        const char* needle = "my";
        const char* missing = "myl";
        UT_ASSERT(haystack.find(needle) == "my life and my love of it...");
        UT_ASSERT(slice.find(needle) == "my life");
        UT_ASSERT(haystack.find(missing).empty());
        UT_ASSERT(slice.find(missing).empty());
    }
    {
        const char needle = 'm';
        const char missing = 'q';
        UT_ASSERT(haystack.find(needle) == "my life and my love of it...");
        UT_ASSERT(slice.find(needle) == "my life");
        UT_ASSERT(haystack.find(missing).empty());
        UT_ASSERT(slice.find(missing).empty());
    }
}


void ck_string_slice_test::test_find_until()
{
    const ck_string str = "I swear, by my life and my love of it...";
    const ck_string_slice haystack = &str;
    const ck_string_slice slice = haystack.slice(5, 19);

    {
        const ck_string needle = "my";
        const ck_string missing = "myl";
        UT_ASSERT(haystack.find_until(needle) == "I swear, by ");
        UT_ASSERT(haystack.find_until(needle, true) == "I swear, by my");
        UT_ASSERT(slice.find_until(needle) == "ar, by ");
        UT_ASSERT(slice.find_until(needle, true) == "ar, by my");
        UT_ASSERT(haystack.find_until(missing) == haystack);
        UT_ASSERT(haystack.find_until(missing, false) ==haystack);
        UT_ASSERT(slice.find_until(missing) == slice);
        UT_ASSERT(slice.find_until(missing)  == slice);
    }
    {
        const std::string needle = "my";
        const std::string missing = "myl";
        UT_ASSERT(haystack.find_until(needle) == "I swear, by ");
        UT_ASSERT(haystack.find_until(needle, true) == "I swear, by my");
        UT_ASSERT(slice.find_until(needle) == "ar, by ");
        UT_ASSERT(slice.find_until(needle, true) == "ar, by my");
        UT_ASSERT(haystack.find_until(missing) == haystack);
        UT_ASSERT(haystack.find_until(missing, false) ==haystack);
        UT_ASSERT(slice.find_until(missing) == slice);
        UT_ASSERT(slice.find_until(missing)  == slice);
    }
    {
        const char* needle = "my";
        const char* missing = "myl";
        UT_ASSERT(haystack.find_until(needle) == "I swear, by ");
        UT_ASSERT(haystack.find_until(needle, true) == "I swear, by my");
        UT_ASSERT(slice.find_until(needle) == "ar, by ");
        UT_ASSERT(slice.find_until(needle, true) == "ar, by my");
        UT_ASSERT(haystack.find_until(missing) == haystack);
        UT_ASSERT(haystack.find_until(missing, false) ==haystack);
        UT_ASSERT(slice.find_until(missing) == slice);
        UT_ASSERT(slice.find_until(missing)  == slice);
    }
    {
        const char needle = 'm';
        const char missing = 'q';
        UT_ASSERT(haystack.find_until(needle) == "I swear, by ");
        UT_ASSERT(haystack.find_until(needle, true) == "I swear, by m");
        UT_ASSERT(slice.find_until(needle) == "ar, by ");
        UT_ASSERT(slice.find_until(needle, true) == "ar, by m");
        UT_ASSERT(haystack.find_until(missing) == haystack);
        UT_ASSERT(haystack.find_until(missing, false) ==haystack);
        UT_ASSERT(slice.find_until(missing) == slice);
        UT_ASSERT(slice.find_until(missing)  == slice);
    }

    {
        const ck_string needle1 = "I";
        const ck_string needle2 = "a";
        UT_ASSERT(haystack.find_until(needle1) == "");
        UT_ASSERT(haystack.find_until(needle1, true) == "I");
        UT_ASSERT(slice.find_until(needle2) == "");
        UT_ASSERT(slice.find_until(needle2, true) == "a");
    }
    {
        const std::string needle1 = "I";
        const std::string needle2 = "a";
        UT_ASSERT(haystack.find_until(needle1) == "");
        UT_ASSERT(haystack.find_until(needle1, true) == "I");
        UT_ASSERT(slice.find_until(needle2) == "");
        UT_ASSERT(slice.find_until(needle2, true) == "a");
    }
    {
        const char* needle1 = "I";
        const char* needle2 = "a";
        UT_ASSERT(haystack.find_until(needle1) == "");
        UT_ASSERT(haystack.find_until(needle1, true) == "I");
        UT_ASSERT(slice.find_until(needle2) == "");
        UT_ASSERT(slice.find_until(needle2, true) == "a");
    }
    {
        const char needle1 = 'I';
        const char needle2 = 'a';
        UT_ASSERT(haystack.find_until(needle1) == "");
        UT_ASSERT(haystack.find_until(needle1, true) == "I");
        UT_ASSERT(slice.find_until(needle2) == "");
        UT_ASSERT(slice.find_until(needle2, true) == "a");
    }
}
