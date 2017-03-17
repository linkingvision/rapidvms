
#include "framework.h"
#include "ck_string_test.h"
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"

#ifdef IS_WINDOWS
#include "windows_compliant_unicode_constants.h"
#else
#include "linux_compliant_unicode_constants.h"
#endif

#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cppkit;

REGISTER_TEST_FIXTURE(ck_string_test);

void ck_string_test::setup()
{
}

void ck_string_test::teardown()
{
}

void ck_string_test::test_is_space()
{
}

void ck_string_test::test_constructor()
{
    {
        const ck_string a;
        UT_ASSERT(a.size() == 0);
        UT_ASSERT(a.empty());
    }

    {
        const std::string str = "Foo";
        const ck_string a(str);

        UT_ASSERT(a.size() == 3);
        UT_ASSERT(!a.empty());
        UT_ASSERT(a == str);
    }

    {
        const char* cstr = "Foo";
        const ck_string a(cstr);

        UT_ASSERT(a.size() == 3);
        UT_ASSERT(!a.empty());
        UT_ASSERT(a == cstr);
    }
}

void ck_string_test::test_copy_constructor()
{
    const ck_string a = "Foo";
    const ck_string b = a;

    UT_ASSERT(a.size() == 3);
    UT_ASSERT(!a.empty());
    UT_ASSERT(b.size() == 3);
    UT_ASSERT(!b.empty());
    UT_ASSERT(a == b);
}

void ck_string_test::test_conversion_operator()
{
    const ck_string a = "Foo";
    const std::string b = a;

    UT_ASSERT(b == "Foo");
}

void ck_string_test::test_cstring()
{
    const ck_string a = "I swear, by my life and my love of it...";

    UT_ASSERT(strlen(a.c_str()) == 40);
}

void ck_string_test::test_wcstring()
{
    ck_string a = LONG_CHINESE_UNICODE_STRING;
    size_t expectedLength = wcslen(LONG_CHINESE_UNICODE_STRING);
    size_t convertedSize = wcslen(a.get_wide_string().c_str());
    UT_ASSERT(convertedSize == expectedLength );
}

void ck_string_test::test_assignment_operator()
{
    ck_string a;
    ck_string b;
    ck_string c;
    const std::string str = "hello";
    const char* cstr = "I swear, by my life and my love of it...";

    a = "I swear, by my life and my love of it...";
    UT_ASSERT(a == cstr);

    b = a;
    UT_ASSERT(a == b);

    c = str;
    UT_ASSERT(c == str);
}

void ck_string_test::test_equality_operator()
{
    const ck_string a = "I swear, by my life and my love of it...";
    const ck_string b = "foo";
    const ck_string c = "Hello World";
    const ck_string d = "I swear, by my life and my love of it...";
    const std::string str = "Hello World";
    const char* cstr = "foo";
    static const char cstr2[] = "foo";
    const ck_string e = JAPANESE_UNICODE_STRING_1;
    const ck_string f(JAPANESE_UNICODE_STRING_1);

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

void ck_string_test::test_inequality_operator()
{
    const ck_string a = "I swear, by my life and my love of it...";
    const ck_string b = "foo";
    const ck_string c = "Hello World";
    const ck_string d = "I swear, by my life and my love of it...";
    const std::string str = "Hello World";
    const char* cstr = "foo";
    const ck_string e = JAPANESE_UNICODE_STRING_1;
    const ck_string f = JAPANESE_UNICODE_STRING_1;


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

void ck_string_test::test_ends_with()
{
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const ck_string end = "love of it...";
        UT_ASSERT(a.ends_with(end));
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const std::string end = "love of it...";
        UT_ASSERT(a.ends_with(end));
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        UT_ASSERT(a.ends_with("love of it..."));
    }

    {
        const ck_string a = "I swear, by my life and my love of it...";
        const ck_string end = "love of it..";
        UT_ASSERT(!a.ends_with(end));
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const std::string end = "love of it..";
        UT_ASSERT(!a.ends_with(end));
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        UT_ASSERT(!a.ends_with("love of it.."));
    }
}

void ck_string_test::test_starts_with()
{
    {
        const ck_string a = "mystring";
        const ck_string b = "mystring";
        UT_ASSERT(a.starts_with(b));
        UT_ASSERT(b.starts_with(a));
    }

    {
        const ck_string a = "I swear, by my life and my love of it...";
        const ck_string start = "I swear, by";
        UT_ASSERT(a.starts_with(start));
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const std::string start = "I swear, by";
        UT_ASSERT(a.starts_with(start));
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        UT_ASSERT(a.starts_with("I swear, by"));
    }

    {
        const ck_string a = "I swear, by my life and my love of it...";
        const ck_string start = " swear, by";
        UT_ASSERT(!a.starts_with(start));
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const std::string start = " swear, by";
        UT_ASSERT(!a.starts_with(start));
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        UT_ASSERT(!a.starts_with(" swear, by"));
    }
}

void ck_string_test::test_contains()
{
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const ck_string b = " my life and my ";
        UT_ASSERT(a.contains(b));
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const std::string b = " my life and my ";
        UT_ASSERT(a.contains(b));
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        UT_ASSERT(a.contains(" my life and my "));
    }

    {
        const ck_string a = "I swear, by my life and my love of it...";
        const ck_string b = " my life and mylove";
        UT_ASSERT(!a.contains(b));
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const std::string b = " my life and mylove";
        UT_ASSERT(!a.contains(b));
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        UT_ASSERT(!a.contains(" my life and mylove"));
    }
}

void ck_string_test::test_is_integer()
{
    UT_ASSERT(!ck_string("").is_integer());
    UT_ASSERT(!ck_string(" ").is_integer());
    UT_ASSERT(!ck_string(".").is_integer());
    UT_ASSERT(!ck_string(".0").is_integer());
    UT_ASSERT(!ck_string("0.").is_integer());
    UT_ASSERT(!ck_string("0.0").is_integer());
    UT_ASSERT(!ck_string("12a").is_integer());
    UT_ASSERT(!ck_string("a12").is_integer());
    UT_ASSERT(!ck_string("a").is_integer());
    UT_ASSERT(!ck_string("a.0").is_integer());
    UT_ASSERT(!ck_string("0.a").is_integer());

    UT_ASSERT(!ck_string("-").is_integer());
    UT_ASSERT(!ck_string(" -").is_integer());
    UT_ASSERT(!ck_string("- ").is_integer());
    UT_ASSERT(!ck_string("-.").is_integer());
    UT_ASSERT(!ck_string("-.0").is_integer());
    UT_ASSERT(!ck_string("-0.").is_integer());
    UT_ASSERT(!ck_string("-0.0").is_integer());
    UT_ASSERT(!ck_string("-12a").is_integer());
    UT_ASSERT(!ck_string("-a12").is_integer());
    UT_ASSERT(!ck_string("-a").is_integer());
    UT_ASSERT(!ck_string("-a.0").is_integer());
    UT_ASSERT(!ck_string("-0.a").is_integer());
    UT_ASSERT(!ck_string("- 42").is_integer());
    UT_ASSERT(!ck_string("42-").is_integer());

    UT_ASSERT(ck_string("0").is_integer());
    UT_ASSERT(ck_string("42").is_integer());
    UT_ASSERT(ck_string(" 42").is_integer());
    UT_ASSERT(ck_string("42 ").is_integer());
    UT_ASSERT(ck_string(" 42 ").is_integer());

    UT_ASSERT(ck_string("-0").is_integer());
    UT_ASSERT(ck_string("-42").is_integer());
    UT_ASSERT(ck_string(" -42").is_integer());
    UT_ASSERT(ck_string("-42 ").is_integer());
    UT_ASSERT(ck_string(" -42 ").is_integer());

    UT_ASSERT(!ck_string("").is_integer(false));
    UT_ASSERT(!ck_string(" ").is_integer(false));
    UT_ASSERT(!ck_string(".").is_integer(false));
    UT_ASSERT(!ck_string(".0").is_integer(false));
    UT_ASSERT(!ck_string("0.").is_integer(false));
    UT_ASSERT(!ck_string("0.0").is_integer(false));
    UT_ASSERT(!ck_string("12a").is_integer(false));
    UT_ASSERT(!ck_string("a12").is_integer(false));
    UT_ASSERT(!ck_string("a").is_integer(false));
    UT_ASSERT(!ck_string("a.0").is_integer(false));
    UT_ASSERT(!ck_string("0.a").is_integer(false));

    UT_ASSERT(!ck_string("-").is_integer(false));
    UT_ASSERT(!ck_string(" -").is_integer(false));
    UT_ASSERT(!ck_string("- ").is_integer(false));
    UT_ASSERT(!ck_string("-.").is_integer(false));
    UT_ASSERT(!ck_string("-.0").is_integer(false));
    UT_ASSERT(!ck_string("-0.").is_integer(false));
    UT_ASSERT(!ck_string("-0.0").is_integer(false));
    UT_ASSERT(!ck_string("-12a").is_integer(false));
    UT_ASSERT(!ck_string("-a12").is_integer(false));
    UT_ASSERT(!ck_string("-a").is_integer(false));
    UT_ASSERT(!ck_string("-a.0").is_integer(false));
    UT_ASSERT(!ck_string("-0.a").is_integer(false));
    UT_ASSERT(!ck_string("- 42").is_integer(false));
    UT_ASSERT(!ck_string("42-").is_integer(false));

    UT_ASSERT(ck_string("0").is_integer(false));
    UT_ASSERT(ck_string("42").is_integer(false));
    UT_ASSERT(ck_string(" 42").is_integer(false));
    UT_ASSERT(ck_string("42 ").is_integer(false));
    UT_ASSERT(ck_string(" 42 ").is_integer(false));

    UT_ASSERT(!ck_string("-0").is_integer(false));
    UT_ASSERT(!ck_string("-42").is_integer(false));
    UT_ASSERT(!ck_string(" -42").is_integer(false));
    UT_ASSERT(!ck_string("-42 ").is_integer(false));
    UT_ASSERT(!ck_string(" -42 ").is_integer(false));
}

void ck_string_test::test_is_double()
{
    UT_ASSERT(!ck_string("").is_double());
    UT_ASSERT(!ck_string(" ").is_double());
    UT_ASSERT(!ck_string(".").is_double());
    UT_ASSERT(!ck_string("12a").is_double());
    UT_ASSERT(!ck_string("a12").is_double());
    UT_ASSERT(!ck_string("a").is_double());
    UT_ASSERT(!ck_string("a.0").is_double());
    UT_ASSERT(!ck_string("0.a").is_double());

    UT_ASSERT(!ck_string("-").is_double());
    UT_ASSERT(!ck_string(" -").is_double());
    UT_ASSERT(!ck_string("- ").is_double());
    UT_ASSERT(!ck_string("-.").is_double());
    UT_ASSERT(!ck_string("-12a").is_double());
    UT_ASSERT(!ck_string("-a12").is_double());
    UT_ASSERT(!ck_string("-a").is_double());
    UT_ASSERT(!ck_string("-a.0").is_double());
    UT_ASSERT(!ck_string("-0.a").is_double());
    UT_ASSERT(!ck_string("- 42").is_double());
    UT_ASSERT(!ck_string("42-").is_double());

    UT_ASSERT(ck_string("0").is_double());
    UT_ASSERT(ck_string("42").is_double());
    UT_ASSERT(ck_string(" 42").is_double());
    UT_ASSERT(ck_string("42 ").is_double());
    UT_ASSERT(ck_string(" 42 ").is_double());
    UT_ASSERT(ck_string("0.").is_double());
    UT_ASSERT(ck_string(".0").is_double());
    UT_ASSERT(ck_string("42.1").is_double());
    UT_ASSERT(ck_string(" 42.1").is_double());
    UT_ASSERT(ck_string("42.1 ").is_double());
    UT_ASSERT(ck_string(" 42.1 ").is_double());

    UT_ASSERT(ck_string("-0").is_double());
    UT_ASSERT(ck_string("-42").is_double());
    UT_ASSERT(ck_string(" -42").is_double());
    UT_ASSERT(ck_string("-42 ").is_double());
    UT_ASSERT(ck_string(" -42 ").is_double());
    UT_ASSERT(ck_string("-0.").is_double());
    UT_ASSERT(ck_string("-.0").is_double());
    UT_ASSERT(ck_string("-42.1").is_double());
    UT_ASSERT(ck_string(" -42.1").is_double());
    UT_ASSERT(ck_string("-42.1 ").is_double());
    UT_ASSERT(ck_string(" -42.1 ").is_double());
}

void ck_string_test::test_lstrip()
{

    ck_string a = "This is a non-stripping\nstring";
    ck_string b = "\n\nThis ck_string Needs Stripping\n \n";
    ck_string c = "Only Stripping on \nthe\n\nright   \n";
    ck_string d = "NOSPACES";
    ck_string e = "";
    ck_string f(CHINESE_UNICODE_WITH_LEFT_SAPCE);
    ck_string g(CHINESE_UNICODE_WITH_NO_SAPCE);

    ck_string retval;

    retval = a.lstrip();
    UT_ASSERT("This is a non-stripping\nstring" == retval);

    retval = b.lstrip();
    UT_ASSERT("This ck_string Needs Stripping\n \n" == retval);

    retval = c.lstrip();
    UT_ASSERT("Only Stripping on \nthe\n\nright   \n" == retval);

    retval = d.lstrip();
    UT_ASSERT("NOSPACES" == retval);

    retval = e.lstrip();
    UT_ASSERT("" == retval);

    retval = g.lstrip();
    UT_ASSERT(retval == g);

    retval = f.lstrip();
    UT_ASSERT(retval == g);
}

void ck_string_test::test_rstrip()
{
    ck_string a = "This is a non-stripping\nstring";
    ck_string b = "\n\nThis ck_string Needs Stripping\n \n";
    ck_string c = "Only Stripping on \nthe\n\nright   \n";
    ck_string d = "NOSPACES";
    ck_string e = "";
    ck_string f(CHINESE_UNICODE_WITH_SAPCE_AT_ENDS);
    ck_string g(CHINESE_UNICODE_WITH_LEFT_SAPCE);

    ck_string retval;

    retval = a.rstrip();
    UT_ASSERT("This is a non-stripping\nstring" == retval);

    retval = b.rstrip();
    UT_ASSERT("\n\nThis ck_string Needs Stripping" == retval);

    retval = c.rstrip();
    UT_ASSERT("Only Stripping on \nthe\n\nright" == retval);

    retval = d.rstrip();
    UT_ASSERT("NOSPACES" == retval);

    retval = e.rstrip();
    UT_ASSERT("" == retval);

    retval = f.rstrip();
    UT_ASSERT(retval == g);
}

void ck_string_test::test_strip()
{
    ck_string a = "This is a non-stripping\nstring";
    ck_string b = "\n\nThis ck_string Needs Stripping\n \n";
    ck_string c = "Only Stripping on \nthe\n\nright   \n";
    ck_string d = "NOSPACES";
    ck_string e = "";
    ck_string f(ARABIC_UNICODE_TO_BE_STRIPPED);
    ck_string g(ARABIC_UNICODE_STRIPPED);

    ck_string retval;

    retval = a.strip();
    UT_ASSERT("This is a non-stripping\nstring" == retval);

    retval = b.strip();
    UT_ASSERT("This ck_string Needs Stripping" == retval);

    retval = c.strip();
    UT_ASSERT("Only Stripping on \nthe\n\nright" == retval);

    retval = d.strip();
    UT_ASSERT("NOSPACES" == retval);

    retval = e.strip();
    UT_ASSERT("" == retval);

    retval = f.strip();
    UT_ASSERT(retval == g);
}

void ck_string_test::test_strip_eol()
{
    UT_ASSERT(ck_string("hello").strip_eol() == "hello");
    UT_ASSERT(ck_string("hello\n").strip_eol() == "hello");
    UT_ASSERT(ck_string("hello\r").strip_eol() == "hello\r");
    UT_ASSERT(ck_string("hello\r\n").strip_eol() == "hello");
    UT_ASSERT(ck_string("hello\n\r").strip_eol() == "hello\n\r");
    UT_ASSERT(ck_string("hello\n\n").strip_eol() == "hello\n");
    UT_ASSERT(ck_string("hello\r\r").strip_eol() == "hello\r\r");
    UT_ASSERT(ck_string("hello\nxxx\n").strip_eol() == "hello\nxxx");
}

void ck_string_test::test_format()
{
    const ck_string a = ck_string::format("I have %d friends, %d faithful friends, more faithful could not be; and every night, by the dim firelight, they come to sit with me.", 2, 2);
    UT_ASSERT(a == "I have 2 friends, 2 faithful friends, more faithful could not be; and every night, by the dim firelight, they come to sit with me.");

    const ck_string b = ck_string::format("%s/%s/%s", "/usr/local", "bin", "myexecutable");
    UT_ASSERT(b == "/usr/local/bin/myexecutable");

    const ck_string c = ck_string::format("I32d I32u I32x");
    UT_ASSERT(c == "I32d I32u I32x");

    const ck_string d = ck_string::format("I64d I64u I64x");
    UT_ASSERT(d == "I64d I64u I64x");

    const ck_string e = ck_string::format("%%I32d %%I32u %%I32x %%d");
    UT_ASSERT(e == "%I32d %I32u %I32x %d");

    const ck_string f = ck_string::format("%%I64d %%I64u %%I64x %%d");
    UT_ASSERT(f == "%I64d %I64u %I64x %d");

    const ck_string g = ck_string::format("%I32d %I32u %I32x", (int32_t)-42, (int32_t)42, (int32_t)42);
    UT_ASSERT(g == "-42 42 2a");

    const ck_string h = ck_string::format("%I64d %I64u %I64x", (int64_t)-42, (int64_t)42, (int64_t)42);
    UT_ASSERT(h == "-42 42 2a");

    const ck_string i = ck_string::format("%d %f", 42, 42.1);
    UT_ASSERT(i == "42 42.100000");
}

void ck_string_test::test_operator_plus()
{
   {
        const ck_string a = "the first half ";
        const ck_string b = "of the sentence.";
        ck_string c;

        c = a + b;

        UT_ASSERT(c == "the first half of the sentence.");
    }
    {
        const ck_string a = "the first half ";
        const char* b = "of the sentence.";
        ck_string c;

        c = a + b;

        UT_ASSERT(c == "the first half of the sentence.");
    }
    {
        const char* a = "the first half ";
        const ck_string b = "of the sentence.";
        ck_string c;

        c = a + b;

        UT_ASSERT(c == "the first half of the sentence.");
    }
    {
        const ck_string a = "hello";
        const char b = '.';
        const ck_string c = a + b;

        UT_ASSERT(c == "hello.");
    }
    {
        const ck_string a = "ello.";
        const char b = 'h';
        const ck_string c = b + a;

        UT_ASSERT(c == "hello.");
    }
}

void ck_string_test::test_plus_equals()
{
    ck_string a = "the first half ";

    a += "of the sentence.";

    UT_ASSERT(a == "the first half of the sentence.");
}

void ck_string_test::test_sub_string()
{
    const ck_string a = "Find the weasel.";
    const ck_string sub = a.substr(5, 3);

    UT_ASSERT(sub == "the");

    const ck_string russian = RUSSIAN_UNICODE_TO_BE_SEARCHED;
    const ck_string sought = RUSSIAN_UNICODE_TO_BE_FOUND;
    const ck_string subStr = russian.substr(russian.find(sought),sought.length());
    UT_ASSERT(sought == subStr);
}

void ck_string_test::test_split()
{
    {
        const ck_string a = "Find the weasel.";

        vector<ck_string> parts = a.split(" ");
        UT_ASSERT(parts.size() == 3);

        auto iter = parts.begin();

        UT_ASSERT(*iter == "Find");
        ++iter;
        UT_ASSERT(*iter == "the");
        ++iter;
        UT_ASSERT(*iter == "weasel.");
    }

    {
        const ck_string a = "abcdefabcdefabcdef";

        vector<ck_string> parts = a.split("ab");
        UT_ASSERT(parts.size() == 3);

        auto iter = parts.begin();

        UT_ASSERT(*iter == "cdef");
        ++iter;
        UT_ASSERT(*iter == "cdef");
        ++iter;
        UT_ASSERT(*iter == "cdef");
    }

    {
        const ck_string a = "abcdefabcdefabcdef";

        ck_string str = "bc";
        vector<ck_string> parts = a.split(str);
        UT_ASSERT(parts.size() == 4);

        auto iter = parts.begin();

        UT_ASSERT(*iter == "a");
        ++iter;
        UT_ASSERT(*iter == "defa");
        ++iter;
        UT_ASSERT(*iter == "defa");
        ++iter;
        UT_ASSERT(*iter == "def");
    }

    {
        const ck_string a = "abcdefabcdefabcdef";

        std::string stdstr = "ef";
        vector<ck_string> parts = a.split(stdstr);
        UT_ASSERT(parts.size() == 3);

        auto iter = parts.begin();

        UT_ASSERT(*iter == "abcd");
        ++iter;
        UT_ASSERT(*iter == "abcd");
        ++iter;
        UT_ASSERT(*iter == "abcd");
    }

    {
        const ck_string a = "abcdefabcdefabcdef";

        std::string stdstr = "ag";
        vector<ck_string> parts = a.split(stdstr);
        UT_ASSERT(parts.size() == 1);

        UT_ASSERT(*parts.begin() == "abcdefabcdefabcdef");
    }
}

void ck_string_test::test_replace_all()
{
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const ck_string b = a.replace_all(' ', '_');
        UT_ASSERT(b == "I_swear,_by_my_life_and_my_love_of_it...");
    }

    {
        const ck_string a = "I swear, by my life and my love of it...";
        const ck_string b = a.replace_all("my", "your");
        UT_ASSERT(b == "I swear, by your life and your love of it...");
    }

    {
        const ck_string a = "I swear, by your life and your love of it...";
        const ck_string b = a.replace_all("your", "my");
        UT_ASSERT(b == "I swear, by my life and my love of it...");
    }
}

void ck_string_test::test_erase_all()
{
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const ck_string b = a.erase_all('e');
        UT_ASSERT(b == "I swar, by my lif and my lov of it...");
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        wchar_t c = 'e';
        const ck_string b = a.erase_all(c);
        UT_ASSERT(b == "I swar, by my lif and my lov of it...");
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const ck_string b = a.erase_all("my");
        UT_ASSERT(b == "I swear, by  life and  love of it...");
    }
}

void ck_string_test::test_from_int()
{
    {
        const ck_string a = ck_string::from_int(10);
        UT_ASSERT(a == "10");
    }
    {
        const ck_string a = ck_string::from_int(-10);
        UT_ASSERT(a == "-10");
    }
}

void ck_string_test::test_from_uint()
{
    {
        const ck_string a = ck_string::from_uint(10U);
        UT_ASSERT(a == "10");
    }
    {
        const ck_string a = ck_string::from_uint(10U, 16);
        UT_ASSERT(a == "a");
    }

    UT_ASSERT_THROWS(ck_string::from_uint(10U, 8), cppkit::ck_exception);
}

void ck_string_test::test_from_int8()
{
    {
        const ck_string a = ck_string::from_int8(10);
        UT_ASSERT(a == "10");
    }
    {
        const ck_string a = ck_string::from_int8(-10);
        UT_ASSERT(a == "-10");
    }
}

void ck_string_test::test_from_uint8()
{
    {
        const ck_string a = ck_string::from_uint8(10U);
        UT_ASSERT(a == "10");
    }
    {
        const ck_string a = ck_string::from_uint8(10U, 16);
        UT_ASSERT(a == "a");
    }

    UT_ASSERT_THROWS(ck_string::from_uint8(10U, 8), cppkit::ck_exception);
}

void ck_string_test::test_from_int16()
{
    {
        const ck_string a = ck_string::from_int16(10);
        UT_ASSERT(a == "10");
    }
    {
        const ck_string a = ck_string::from_int16(-10);
        UT_ASSERT(a == "-10");
    }
}

void ck_string_test::test_from_uint16()
{
    {
        const ck_string a = ck_string::from_uint16(10U);
        UT_ASSERT(a == "10");
    }
    {
        const ck_string a = ck_string::from_uint16(10U, 16);
        UT_ASSERT(a == "a");
    }

    UT_ASSERT_THROWS(ck_string::from_uint16(10U, 8), cppkit::ck_exception);
}

void ck_string_test::test_from_int32()
{
    {
        const ck_string a = ck_string::from_int32(10);
        UT_ASSERT(a == "10");
    }
    {
        const ck_string a = ck_string::from_int32(-10);
        UT_ASSERT(a == "-10");
    }

    {
        const ck_string a = ck_string::from_int32(1073741828);
        UT_ASSERT(a == "1073741828");
    }
    {
        const ck_string a = ck_string::from_int32(-1073741828);
        UT_ASSERT(a == "-1073741828");
    }
}

void ck_string_test::test_from_uint32()
{
    {
        const ck_string a = ck_string::from_uint32(10U);
        UT_ASSERT(a == "10");
    }
    {
        const ck_string a = ck_string::from_uint32(2147483652UL);
        UT_ASSERT(a == "2147483652");
    }
    {
        const ck_string a = ck_string::from_uint32(10U, 16);
        UT_ASSERT(a == "a");
    }

    UT_ASSERT_THROWS( ck_string::from_uint32(10U, 8), cppkit::ck_exception);
}

void ck_string_test::test_from_int64()
{
    {
        const ck_string a = ck_string::from_int64(10);
        UT_ASSERT(a == "10");
    }
    {
        const ck_string a = ck_string::from_int64(-10);
        UT_ASSERT(a == "-10");
    }

    {
        const ck_string a = ck_string::from_int64(4611686018427387908LL);
        UT_ASSERT(a == "4611686018427387908");
    }
    {
        const ck_string a = ck_string::from_int64(-4611686018427387908LL);
        UT_ASSERT(a == "-4611686018427387908");
    }
}

void ck_string_test::test_from_uint64()
{
    {
        const ck_string a = ck_string::from_uint64(10U);
        UT_ASSERT(a == "10");
    }
    {
        const ck_string a = ck_string::from_uint64(9223372036854775812ULL);
        UT_ASSERT(a == "9223372036854775812");
    }
    {
        const ck_string a = ck_string::from_uint64(10U, 16);
        UT_ASSERT(a == "a");
    }

    UT_ASSERT_THROWS(ck_string::from_uint64(10U, 8), cppkit::ck_exception);
}

void ck_string_test::test_from_double()
{
    {
        const ck_string a = ck_string::from_double(10.0);
        UT_ASSERT(a == "10.000000");
    }
    {
        const ck_string a = ck_string::from_double(-10.0);
        UT_ASSERT(a == "-10.000000");
    }
}

void ck_string_test::test_to_int()
{
    UT_ASSERT((ck_string("10").to_int() * 10) == 100);
    UT_ASSERT((ck_string("-10").to_int() * 10) == -100);
    UT_ASSERT((ck_string("10.7").to_int(true) * 10) == 100);
    UT_ASSERT((ck_string("-10.7").to_int(true) * 10) == -100);
    UT_ASSERT((ck_string("10").to_int(16) * 10) == 160);
    UT_ASSERT((ck_string("-10").to_int(16) * 10) == -160);

    UT_ASSERT_THROWS(ck_string("10.7").to_int(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10.7").to_int(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("10").to_int(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10").to_int(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("ten").to_int(), cppkit::ck_exception);
}

void ck_string_test::test_to_uint()
{
    UT_ASSERT((ck_string("10").to_uint() * 10) == 100);
    UT_ASSERT((ck_string("10.7").to_uint(true) * 10) == 100);
    UT_ASSERT((ck_string("10").to_uint(16) * 10) == 160);

    UT_ASSERT_THROWS(ck_string("-10").to_uint(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("10.7").to_uint(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10.7").to_uint(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("10").to_uint(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10").to_uint(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("ten").to_uint(), cppkit::ck_exception);
}

void ck_string_test::test_to_int8()
{
    UT_ASSERT((ck_string("10").to_int8() * 10) == 100);
    UT_ASSERT((ck_string("-10").to_int8() * 10) == -100);
    UT_ASSERT((ck_string("10.7").to_int8(true) * 10) == 100);
    UT_ASSERT((ck_string("-10.7").to_int8(true) * 10) == -100);
    UT_ASSERT((ck_string("10").to_int8(16) * 10) == 160);
    UT_ASSERT((ck_string("-10").to_int8(16) * 10) == -160);

    UT_ASSERT_THROWS(ck_string("10.7").to_int8(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10.7").to_int8(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("10").to_int8(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10").to_int8(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("ten").to_int8(), cppkit::ck_exception);
}

void ck_string_test::test_to_uint8()
{
    UT_ASSERT((ck_string("10").to_uint8() * 10) == 100);
    UT_ASSERT((ck_string("10.7").to_uint8(true) * 10) == 100);
    UT_ASSERT((ck_string("10").to_uint8(16) * 10) == 160);

    UT_ASSERT_THROWS(ck_string("-10").to_uint8(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("10.7").to_uint8(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10.7").to_uint8(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("10").to_uint8(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10").to_uint8(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("ten").to_uint8(), cppkit::ck_exception);
}

void ck_string_test::test_to_int16()
{
    UT_ASSERT((ck_string("10").to_int16() * 10) == 100);
    UT_ASSERT((ck_string("-10").to_int16() * 10) == -100);
    UT_ASSERT((ck_string("10.7").to_int16(true) * 10) == 100);
    UT_ASSERT((ck_string("-10.7").to_int16(true) * 10) == -100);
    UT_ASSERT((ck_string("10").to_int16(16) * 10) == 160);
    UT_ASSERT((ck_string("-10").to_int16(16) * 10) == -160);

    UT_ASSERT_THROWS(ck_string("10.7").to_int16(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10.7").to_int16(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("10").to_int16(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10").to_int16(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("ten").to_int16(), cppkit::ck_exception);
}

void ck_string_test::test_to_uint16()
{
    UT_ASSERT((ck_string("10").to_uint16() * 10) == 100);
    UT_ASSERT((ck_string("10.7").to_uint16(true) * 10) == 100);
    UT_ASSERT((ck_string("10").to_uint16(16) * 10) == 160);

    UT_ASSERT_THROWS(ck_string("-10").to_uint16(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("10.7").to_uint16(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10.7").to_uint16(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("10").to_uint16(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10").to_uint16(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("ten").to_uint16(), cppkit::ck_exception);
}

void ck_string_test::test_to_int32()
{
    UT_ASSERT((ck_string("10").to_int32() * 10) == 100);
    UT_ASSERT((ck_string("-10").to_int32() * 10) == -100);
    UT_ASSERT((ck_string("1073741828").to_int32()) == 1073741828);
    UT_ASSERT((ck_string("-1073741828").to_int32()) == -1073741828);
    UT_ASSERT((ck_string("10.7").to_int32(true) * 10) == 100);
    UT_ASSERT((ck_string("-10.7").to_int32(true) * 10) == -100);
    UT_ASSERT((ck_string("10").to_int32(16) * 10) == 160);
    UT_ASSERT((ck_string("-10").to_int32(16) * 10) == -160);

    UT_ASSERT_THROWS(ck_string("10.7").to_int32(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10.7").to_int32(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("10").to_int32(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10").to_int32(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("ten").to_int32(), cppkit::ck_exception);
}

void ck_string_test::test_to_uint32()
{
    UT_ASSERT((ck_string("10").to_uint32() * 10) == 100);
    UT_ASSERT((ck_string("4294967295").to_uint32()) == (uint32_t)-1);
    UT_ASSERT((ck_string("10.7").to_uint32(true) * 10) == 100);
    UT_ASSERT((ck_string("10").to_uint32(16) * 10) == 160);

    UT_ASSERT_THROWS(ck_string("-10").to_uint32(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("10.7").to_uint32(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10.7").to_uint32(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("10").to_uint32(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10").to_uint32(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("ten").to_uint32(), cppkit::ck_exception);
}

void ck_string_test::test_to_int64()
{
    UT_ASSERT((ck_string("10").to_int64() * 10) == 100);
    UT_ASSERT((ck_string("-10").to_int64() * 10) == -100);
    UT_ASSERT((ck_string("4611686018427387908").to_int64()) == 4611686018427387908LL);
    UT_ASSERT((ck_string("-4611686018427387908").to_int64()) == -4611686018427387908LL);
    UT_ASSERT((ck_string("10.7").to_int64(true) * 10) == 100);
    UT_ASSERT((ck_string("-10.7").to_int64(true) * 10) == -100);
    UT_ASSERT((ck_string("10").to_int64(16) * 10) == 160);
    UT_ASSERT((ck_string("-10").to_int64(16) * 10) == -160);

    UT_ASSERT_THROWS(ck_string("10.7").to_int64(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10.7").to_int64(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("10").to_int64(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10").to_int64(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("ten").to_int64(), cppkit::ck_exception);
}

void ck_string_test::test_to_uint64()
{
    UT_ASSERT((ck_string("10").to_uint64() * 10) == 100);
    UT_ASSERT((ck_string("9223372036854775812").to_uint64()) == 9223372036854775812ULL);
    UT_ASSERT((ck_string("10.7").to_uint64(true) * 10) == 100);
    UT_ASSERT((ck_string("10").to_uint64(16) * 10) == 160);

    UT_ASSERT_THROWS(ck_string("-10").to_uint64(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("10.7").to_uint64(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10.7").to_uint64(), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("10").to_uint64(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("-10").to_uint64(8), cppkit::ck_exception);
    UT_ASSERT_THROWS(ck_string("ten").to_uint64(), cppkit::ck_exception);
}

void ck_string_test::test_to_double()
{
    UT_ASSERT(ck_string("10").to_double() == 10);
    UT_ASSERT(ck_string("10.7").to_double() == 10.7);
    UT_ASSERT(ck_string("-10.7").to_double() == -10.7);

    UT_ASSERT_THROWS(ck_string("ten").to_double(), cppkit::ck_exception);
}

void ck_string_test::test_to_lower()
{
    const ck_string a = "I swear, by my life and my love of it...";
    const ck_string b = a.to_lower();
    UT_ASSERT(b == "i swear, by my life and my love of it...");
}

void ck_string_test::test_to_upper()
{
    const ck_string a = "I swear, by my life and my love of it...";
    const ck_string b = a.to_upper();
    UT_ASSERT(b == "I SWEAR, BY MY LIFE AND MY LOVE OF IT...");
}

void ck_string_test::test_base64_encode()
{
    uint8_t buf[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    const ck_string encodedData = ck_string::to_base64(&buf[0], sizeof(buf));

    UT_ASSERT(encodedData == "AQIDBAUGBwgJCg==");
}

void ck_string_test::test_base64_decode()
{
    uint8_t buf[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    const ck_string encodedData = "AQIDBAUGBwgJCg==";
    shared_ptr<ck_memory> decoded = encodedData.from_base64();

    UT_ASSERT(memcmp(buf, decoded->map_data().get_ptr(), 10) == 0);
}

void ck_string_test::test_find_first()
{
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const ck_string toFind = "my";
        const int forwardPos = a.find(toFind);

        UT_ASSERT(forwardPos == 12);
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const std::string toFind = "my";
        const int forwardPos = a.find(toFind);

        UT_ASSERT(forwardPos == 12);
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const int forwardPos = a.find("my");

        UT_ASSERT(forwardPos == 12);
    }
}

void ck_string_test::test_find_last()
{
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const ck_string toFind = "my";
        const int backwardPos = a.rfind(toFind);

        UT_ASSERT(backwardPos == 24);
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const std::string toFind = "my";
        const int backwardPos = a.rfind(toFind);

        UT_ASSERT(backwardPos == 24);
    }
    {
        const ck_string a = "I swear, by my life and my love of it...";
        const int backwardPos = a.rfind("my");

        UT_ASSERT(backwardPos == 24);
    }
}

void ck_string_test::test_uri_encode()
{
    {
        const ck_string inputStr = "a & Z+ 26";
        const ck_string encoded = inputStr.uri_encode();
        UT_ASSERT(encoded == "a%20%26%20Z%2B%2026");
    }

    UT_ASSERT(ck_string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-._~").uri_encode() == ck_string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-._~"));

    for(char i = 1; i < '-'; ++i)
        UT_ASSERT(ck_string::format("%c", i).uri_encode() == ck_string::format("%%%02X", i));

    // + 1 is '.'
    for(char i = '-' + 2 ; i < '0'; ++i)
        UT_ASSERT(ck_string::format("%c", i).uri_encode() == ck_string::format("%%%02X", i));

    for(char i = '9' + 1 + 1; i < 'A'; ++i)
        UT_ASSERT(ck_string::format("%c", i).uri_encode() == ck_string::format("%%%02X", i));

    for(char i = 'Z' + 1; i < '_'; ++i)
        UT_ASSERT(ck_string::format("%c", i).uri_encode() == ck_string::format("%%%02X", i));

    for(char i = '_' + 1; i < 'a'; ++i)
        UT_ASSERT(ck_string::format("%c", i).uri_encode() == ck_string::format("%%%02X", i));

    for(char i = 'z' + 1; i < '~'; ++i)
        UT_ASSERT(ck_string::format("%c", i).uri_encode() == ck_string::format("%%%02X", i));

    UT_ASSERT(ck_string::format("%c", 127).uri_encode() == "%7F");
}

void ck_string_test::test_uri_decode()
{
    {
        const ck_string input_str = "aBcdEfGh+%26%20Z%2B+26%26";
        const ck_string decoded = input_str.uri_decode();
        UT_ASSERT(decoded == "aBcdEfGh & Z+ 26&");

        ck_string malformed_str = "a+%26+Z%2+26%26";
        UT_ASSERT_THROWS(malformed_str.uri_decode(), cppkit::ck_exception);
    }
    {
        ck_string input_str = "aBcdEfGh+%26%20Z%2B+26%26";
        input_str = input_str.uri_decode();
        UT_ASSERT(input_str == "aBcdEfGh & Z+ 26&");

        ck_string malformed_str = "a+%26+Z%2+26%26";
        UT_ASSERT_THROWS(malformed_str.uri_decode(), cppkit::ck_exception);
    }
    {
        ck_string input_str = "%2F%2f%2d%2D";
        input_str = input_str.uri_decode();
        UT_ASSERT(input_str == "//--");
    }

    UT_ASSERT(ck_string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-._~").uri_decode() ==
                   ck_string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-._~"));

    //Wraps to -128 after incrementing past 127.
    for(char i = 1; i > 0; ++i)
        UT_ASSERT(ck_string::format("%%%02X", i).uri_decode() == ck_string::format("%c", i));

    for(char i = 1; i > 0; ++i)
        UT_ASSERT(ck_string::format("%%%02x", i).uri_decode() == ck_string::format("%c", i));
}

void ck_string_test::test_constructor_unicode()
{
    const ck_string str(CHINESE_UNICODE_STRING_1);
    UT_ASSERT(!str.empty());
    UT_ASSERT( str == CHINESE_UNICODE_STRING_1 );
}

void ck_string_test::test_copy_constructor_unicode()
{
    const ck_string c = CHINESE_UNICODE_STRING_2;
    const ck_string d = c;

    UT_ASSERT(!c.empty());
    UT_ASSERT(!d.empty());
    UT_ASSERT(c.size() == CHINESE_UNICODE_STRING_2_SIZE);
    UT_ASSERT(d.size() == CHINESE_UNICODE_STRING_2_SIZE);
    UT_ASSERT(c == d);
}

void ck_string_test::test_assignment_operator_unicode()
{
    ck_string a;
    ck_string b;
    ck_string c;
    std::string str = "Surround Wei to rescue Zhao";

    a = CHINESE_UNICODE_STRING_3;
    UT_ASSERT(a == CHINESE_UNICODE_STRING_3);

    b = a;
    UT_ASSERT(a == b);

    c = (str = b);
    UT_ASSERT(c == str);
}

void ck_string_test::test_ends_with_unicode_version()
{
    const ck_string a(CHINESE_UNICODE_STRING_1);
    const std::string endStdString = ck_string::convert_wide_string_to_multi_byte_string(CHINESE_UNICODE_STRING_1_END);
    const ck_string endXString(CHINESE_UNICODE_STRING_1_END);

    const std::string middleStdString = ck_string::convert_wide_string_to_multi_byte_string(CHINESE_UNICODE_STRING_1_MIDDLE);
    const ck_string middleXString(CHINESE_UNICODE_STRING_1_MIDDLE);

    const std::string beginStdString = ck_string::convert_wide_string_to_multi_byte_string(CHINESE_UNICODE_STRING_1_BEGIN);
    const ck_string beginXString(CHINESE_UNICODE_STRING_1_BEGIN);

    UT_ASSERT(a.ends_with(CHINESE_UNICODE_STRING_1_END));
    UT_ASSERT(a.ends_with(endStdString));
    UT_ASSERT(a.ends_with(endXString));

    UT_ASSERT(!a.ends_with(beginXString));
    UT_ASSERT(!a.ends_with(CHINESE_UNICODE_STRING_1_BEGIN));
    UT_ASSERT(!a.ends_with(beginStdString));

    UT_ASSERT(!a.ends_with(middleStdString));
    UT_ASSERT(!a.ends_with(CHINESE_UNICODE_STRING_1_MIDDLE));
    UT_ASSERT(!a.ends_with(middleXString));
}

void ck_string_test::test_starts_with_unicode_version()
{
    const ck_string a(CHINESE_UNICODE_STRING_1);
    const std::string endStdString = ck_string::convert_wide_string_to_multi_byte_string(CHINESE_UNICODE_STRING_1_END);
    const ck_string endXString(CHINESE_UNICODE_STRING_1_END);

    const std::string middleStdString = ck_string::convert_wide_string_to_multi_byte_string(CHINESE_UNICODE_STRING_1_MIDDLE);
    const ck_string middleXString(CHINESE_UNICODE_STRING_1_MIDDLE);

    const std::string beginStdString = ck_string::convert_wide_string_to_multi_byte_string(CHINESE_UNICODE_STRING_1_BEGIN);
    const ck_string beginXString(CHINESE_UNICODE_STRING_1_BEGIN);

    UT_ASSERT(!a.starts_with(CHINESE_UNICODE_STRING_1_END));
    UT_ASSERT(!a.starts_with(endStdString));
    UT_ASSERT(!a.starts_with(endXString));

    UT_ASSERT(a.starts_with(beginXString));
    UT_ASSERT(a.starts_with(CHINESE_UNICODE_STRING_1_BEGIN));
    UT_ASSERT(a.starts_with(beginStdString));

    UT_ASSERT(!a.starts_with(middleStdString));
    UT_ASSERT(!a.starts_with(CHINESE_UNICODE_STRING_1_MIDDLE));
    UT_ASSERT(!a.starts_with(middleXString));
}

void ck_string_test::test_contains_unicode_version()
{
    const ck_string a(CHINESE_UNICODE_STRING_1);
    const std::string endStdString = ck_string::convert_wide_string_to_multi_byte_string(CHINESE_UNICODE_STRING_1_END);
    const ck_string endXString(CHINESE_UNICODE_STRING_1_END);

    const std::string middleStdString = ck_string::convert_wide_string_to_multi_byte_string(CHINESE_UNICODE_STRING_1_MIDDLE);
    const ck_string middleXString(CHINESE_UNICODE_STRING_1_MIDDLE);

    const std::string beginStdString = ck_string::convert_wide_string_to_multi_byte_string(CHINESE_UNICODE_STRING_1_BEGIN);
    const ck_string beginXString(CHINESE_UNICODE_STRING_1_BEGIN);

    const std::string notContanedStdString = ck_string::convert_wide_string_to_multi_byte_string(CHINESE_UNICODE_STRING_1_REORDERED);
    const ck_string notContanedXString(CHINESE_UNICODE_STRING_1_REORDERED);

    UT_ASSERT(a.contains(CHINESE_UNICODE_STRING_1_END));
    UT_ASSERT(a.contains(endStdString));
    UT_ASSERT(a.contains(endXString));

    UT_ASSERT(a.contains(beginXString));
    UT_ASSERT(a.contains(CHINESE_UNICODE_STRING_1_BEGIN));
    UT_ASSERT(a.contains(beginStdString));

    UT_ASSERT(a.contains(middleStdString));
    UT_ASSERT(a.contains(CHINESE_UNICODE_STRING_1_MIDDLE));
    UT_ASSERT(a.contains(middleXString));

    UT_ASSERT(!a.contains(CHINESE_UNICODE_STRING_1_REORDERED));
    UT_ASSERT(!a.contains(notContanedStdString));
    UT_ASSERT(!a.contains(notContanedXString));
}

void ck_string_test::test_operator_plus_unicode_version()
{
    const ck_string full(LONG_CHINESE_UNICODE_STRING);
    const ck_string begining(LONG_CHINESE_UNICODE_STRING_BEGINING);
    const wchar_t* endWCharT = LONG_CHINESE_UNICODE_STRING_ENDING;
    const std::string endStdString = ck_string::convert_wide_string_to_multi_byte_string(LONG_CHINESE_UNICODE_STRING_ENDING);
    const ck_string endXString = LONG_CHINESE_UNICODE_STRING_ENDING;
    const char comma = ',';

    ck_string result;
    result = begining + comma + endWCharT;
    UT_ASSERT(result == full);
    result = begining + comma + endStdString;
    UT_ASSERT(result == full);
    result = begining + comma + endXString;
    UT_ASSERT(result == full);
}

void ck_string_test::test_operator_plus_equals_unicode_version()
{
    const ck_string full(LONG_CHINESE_UNICODE_STRING);
    const ck_string begining(LONG_CHINESE_UNICODE_STRING_BEGINING);
    const wchar_t* endWCharT = LONG_CHINESE_UNICODE_STRING_ENDING;
    const std::string endStdString = ck_string::convert_wide_string_to_multi_byte_string(LONG_CHINESE_UNICODE_STRING_ENDING);
    const ck_string endXString = LONG_CHINESE_UNICODE_STRING_ENDING;

    {
        ck_string result(begining);
        result += ',';
        result += endWCharT;
        UT_ASSERT(result == full);
    }
    {
        ck_string result(begining);
        result += ',';
        result += endXString;
        UT_ASSERT(result == full);
    }
    {
        ck_string result(begining);
        result += ',';
        result += endStdString;
        UT_ASSERT(result == full);
    }
}

void ck_string_test::test_erase_all_unicode()
{
    ck_string erased(UNICODE_STRING_AFTER_ERASE);
    ck_string a(UNICODE_STRING_TO_ERASE_FROM);
    ck_string b = a.erase_all(UNICODE_CHARACTER_TO_BE_ERASED);

    UT_ASSERT(b == erased);
}

void ck_string_test::test_replace_all_unicode()
{
    ck_string replaced(UNICODE_STRING_AFTER_REPLACING);
    ck_string a(UNICODE_STRING_BEFORE_REPLACING);
    ck_string b = a.replace_all(UNICODE_CHARACTER_TO_BE_REPLACED,UNICODE_CHARACTER_TO_REPLACE_WITH);
    UT_ASSERT(b == replaced);
}

void ck_string_test::test_split_unicode()
{
    {
        const ck_string a = UNICODE_STRING_TO_SPLIT;
        const ck_string piece0 = UNICODE_STRING_SPLIT_BY_CHARACTER_PIECE_0;
        const ck_string piece4 = UNICODE_STRING_SPLIT_BY_CHARACTER_PIECE_4;

        vector<ck_string> parts = a.split(UNICODE_CHARACTER_TO_SPLIT_ON);
        UT_ASSERT(parts.size() == 5);


        auto iter = parts.begin();
        UT_ASSERT(*iter == piece0);
        ++iter;
        UT_ASSERT(*iter == UNICODE_STRING_SPLIT_BY_CHARACTER_PIECE_1);
        ++iter;
        UT_ASSERT(*iter == UNICODE_STRING_SPLIT_BY_CHARACTER_PIECE_2);
        ++iter;
        UT_ASSERT(*iter == UNICODE_STRING_SPLIT_BY_CHARACTER_PIECE_3);
        ++iter;
        UT_ASSERT(*iter == piece4);
    }

    {
        const ck_string a = UNICODE_STRING_TO_SPLIT;
        const ck_string piece1 = UNICODE_STRING_SPLIT_BY_STRING_PIECE_1;
        vector<ck_string> parts = a.split(UNICODE_STRING_TO_SPLIT_ON);
        UT_ASSERT(parts.size() == 3);


        auto iter = parts.begin();
        UT_ASSERT(*iter == UNICODE_STRING_SPLIT_BY_STRING_PIECE_0);
        ++iter;
        UT_ASSERT(*iter == piece1);
        ++iter;
        UT_ASSERT(*iter == UNICODE_STRING_SPLIT_BY_STRING_PIECE_2);
    }

}

void ck_string_test::test_find_first_unicode()
{
    const ck_string a = UNICODE_STRING_FOR_FINDING_VALUES_IN;

    {
        const ck_string toFind = UNICODE_STRING_FOR_FINDING;
        const int forwardPos = a.find(toFind);

        UT_ASSERT(forwardPos == 6);
    }
    {
        const int forwardPos = a.find(UNICODE_STRING_FOR_FINDING);
        UT_ASSERT(forwardPos == 6);
    }
    {
        const int forwardPos = a.find(UNICODE_CHARACTER_FOR_FINDING);

        UT_ASSERT(forwardPos == 6);
    }
}

void ck_string_test::test_find_last_unicode()
{
    const ck_string a = UNICODE_STRING_FOR_FINDING_VALUES_IN;

    {
        const ck_string toFind = UNICODE_STRING_FOR_FINDING;
        const int backwardPos = a.rfind(toFind);

        UT_ASSERT(backwardPos == 12);
    }
    {
        const int backwardPos = a.rfind(UNICODE_STRING_FOR_FINDING);

        UT_ASSERT(backwardPos == 12);
    }
    {
        const int backwardPos = a.rfind(UNICODE_CHARACTER_FOR_FINDING);

        UT_ASSERT(backwardPos == 12);
    }
}
