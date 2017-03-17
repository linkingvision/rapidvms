
#include "XStringTest.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "XSDK/XString.h"
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

REGISTER_TEST_FIXTURE(XStringTest);

void XStringTest::setup()
{
}

void XStringTest::teardown()
{
}

void XStringTest::TestConstructor()
{
    {
        const XString a;
        UT_ASSERT(a.size() == 0);
        UT_ASSERT(a.empty());
    }

    {
        const std::string str = "Foo";
        const XString a(str);

        UT_ASSERT(a.size() == 3);
        UT_ASSERT(!a.empty());
        UT_ASSERT(a == str);
    }

    {
        const char* cstr = "Foo";
        const XString a(cstr);

        UT_ASSERT(a.size() == 3);
        UT_ASSERT(!a.empty());
        UT_ASSERT(a == cstr);
    }
}


void XStringTest::TestConstructorUnicode()
{
    const XString str(CHINESE_UNICODE_STRING_1);
    UT_ASSERT(!str.empty());
    UT_ASSERT( str == CHINESE_UNICODE_STRING_1 );

}


void XStringTest::TestCopyConstructorUnicode()
{
    const XString c = CHINESE_UNICODE_STRING_2;
    const XString d = c;

    UT_ASSERT(!c.empty());
    UT_ASSERT(!d.empty());
    UT_ASSERT(c.size() == CHINESE_UNICODE_STRING_2_SIZE);
    UT_ASSERT(d.size() == CHINESE_UNICODE_STRING_2_SIZE);
    UT_ASSERT(c == d);
}


void XStringTest::TestCopyConstructor()
{
    const XString a = "Foo";
    const XString b = a;

    UT_ASSERT(a.size() == 3);
    UT_ASSERT(!a.empty());
    UT_ASSERT(b.size() == 3);
    UT_ASSERT(!b.empty());
    UT_ASSERT(a == b);
}

void XStringTest::TestConversionOperator()
{
    const XString a = "Foo";
    const std::string b = a;

    UT_ASSERT(b == "Foo");
}

void XStringTest::TestCString()
{
    const XString a = "I swear, by my life and my love of it...";

    UT_ASSERT(strlen(a.c_str()) == 40);
}

void XStringTest::TestWCString()
{
    XString a = LONG_CHINESE_UNICODE_STRING;
    size_t expectedLength = wcslen(LONG_CHINESE_UNICODE_STRING);
    size_t convertedSize = wcslen(a.get_wide_string().c_str());
    UT_ASSERT(convertedSize == expectedLength );
}


void XStringTest::TestAssignmentOperator()
{
    XString a;
    XString b;
    XString c;
    const std::string str = "hello";
    const char* cstr = "I swear, by my life and my love of it...";

    a = "I swear, by my life and my love of it...";
    UT_ASSERT(a == cstr);

    b = a;
    UT_ASSERT(a == b);

    c = str;
    UT_ASSERT(c == str);
}


void XStringTest::TestAssignmentOperatorUnicode()
{
    XString a;
    XString b;
    XString c;
    std::string str = "Surround Wei to rescue Zhao";


    a = CHINESE_UNICODE_STRING_3;
    UT_ASSERT(a == CHINESE_UNICODE_STRING_3);

    b = a;
    UT_ASSERT(a == b);

    c = (str = b);
    UT_ASSERT(c == str);
}

void XStringTest::TestEqualityOperator()
{
    const XString a = "I swear, by my life and my love of it...";
    const XString b = "foo";
    const XString c = "Hello World";
    const XString d = "I swear, by my life and my love of it...";
    const std::string str = "Hello World";
    const char* cstr = "foo";
    static const char cstr2[] = "foo";
    const XString e = JAPANESE_UNICODE_STRING_1;
    const XString f(JAPANESE_UNICODE_STRING_1);

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

void XStringTest::TestInequalityOperator()
{
    const XString a = "I swear, by my life and my love of it...";
    const XString b = "foo";
    const XString c = "Hello World";
    const XString d = "I swear, by my life and my love of it...";
    const std::string str = "Hello World";
    const char* cstr = "foo";
    const XString e = JAPANESE_UNICODE_STRING_1;
    const XString f = JAPANESE_UNICODE_STRING_1;


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


void XStringTest::TestEndsWithUnicodeVersion()
{
    const XString a(CHINESE_UNICODE_STRING_1);
    const std::string endStdString = XString::ConvertWideStringToMultiByteString(CHINESE_UNICODE_STRING_1_END);
    const XString endXString(CHINESE_UNICODE_STRING_1_END);

    const std::string middleStdString = XString::ConvertWideStringToMultiByteString(CHINESE_UNICODE_STRING_1_MIDDLE);
    const XString middleXString(CHINESE_UNICODE_STRING_1_MIDDLE);

    const std::string beginStdString = XString::ConvertWideStringToMultiByteString(CHINESE_UNICODE_STRING_1_BEGIN);
    const XString beginXString(CHINESE_UNICODE_STRING_1_BEGIN);

    UT_ASSERT(a.EndsWith(CHINESE_UNICODE_STRING_1_END));
    UT_ASSERT(a.EndsWith(endStdString));
    UT_ASSERT(a.EndsWith(endXString));

    UT_ASSERT(!a.EndsWith(beginXString));
    UT_ASSERT(!a.EndsWith(CHINESE_UNICODE_STRING_1_BEGIN));
    UT_ASSERT(!a.EndsWith(beginStdString));

    UT_ASSERT(!a.EndsWith(middleStdString));
    UT_ASSERT(!a.EndsWith(CHINESE_UNICODE_STRING_1_MIDDLE));
    UT_ASSERT(!a.EndsWith(middleXString));
}

void XStringTest::TestEndsWith()
{
    {
        const XString a = "I swear, by my life and my love of it...";
        const XString end = "love of it...";
        UT_ASSERT(a.EndsWith(end));
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        const std::string end = "love of it...";
        UT_ASSERT(a.EndsWith(end));
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        UT_ASSERT(a.EndsWith("love of it..."));
    }

    {
        const XString a = "I swear, by my life and my love of it...";
        const XString end = "love of it..";
        UT_ASSERT(!a.EndsWith(end));
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        const std::string end = "love of it..";
        UT_ASSERT(!a.EndsWith(end));
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        UT_ASSERT(!a.EndsWith("love of it.."));
    }
}


void XStringTest::TestStartsWithUnicodeVersion()
{
    const XString a(CHINESE_UNICODE_STRING_1);
    const std::string endStdString = XString::ConvertWideStringToMultiByteString(CHINESE_UNICODE_STRING_1_END);
    const XString endXString(CHINESE_UNICODE_STRING_1_END);

    const std::string middleStdString = XString::ConvertWideStringToMultiByteString(CHINESE_UNICODE_STRING_1_MIDDLE);
    const XString middleXString(CHINESE_UNICODE_STRING_1_MIDDLE);

    const std::string beginStdString = XString::ConvertWideStringToMultiByteString(CHINESE_UNICODE_STRING_1_BEGIN);
    const XString beginXString(CHINESE_UNICODE_STRING_1_BEGIN);

    UT_ASSERT(!a.StartsWith(CHINESE_UNICODE_STRING_1_END));
    UT_ASSERT(!a.StartsWith(endStdString));
    UT_ASSERT(!a.StartsWith(endXString));

    UT_ASSERT(a.StartsWith(beginXString));
    UT_ASSERT(a.StartsWith(CHINESE_UNICODE_STRING_1_BEGIN));
    UT_ASSERT(a.StartsWith(beginStdString));

    UT_ASSERT(!a.StartsWith(middleStdString));
    UT_ASSERT(!a.StartsWith(CHINESE_UNICODE_STRING_1_MIDDLE));
    UT_ASSERT(!a.StartsWith(middleXString));
}

void XStringTest::TestStartsWith()
{
    {
        const XString a = "mystring";
        const XString b = "mystring";
        UT_ASSERT(a.StartsWith(b));
        UT_ASSERT(b.StartsWith(a));
    }

    {
        const XString a = "I swear, by my life and my love of it...";
        const XString start = "I swear, by";
        UT_ASSERT(a.StartsWith(start));
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        const std::string start = "I swear, by";
        UT_ASSERT(a.StartsWith(start));
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        UT_ASSERT(a.StartsWith("I swear, by"));
    }

    {
        const XString a = "I swear, by my life and my love of it...";
        const XString start = " swear, by";
        UT_ASSERT(!a.StartsWith(start));
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        const std::string start = " swear, by";
        UT_ASSERT(!a.StartsWith(start));
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        UT_ASSERT(!a.StartsWith(" swear, by"));
    }
}


void XStringTest::TestContainsUnicodeVersion()
{
    const XString a(CHINESE_UNICODE_STRING_1);
    const std::string endStdString = XString::ConvertWideStringToMultiByteString(CHINESE_UNICODE_STRING_1_END);
    const XString endXString(CHINESE_UNICODE_STRING_1_END);

    const std::string middleStdString = XString::ConvertWideStringToMultiByteString(CHINESE_UNICODE_STRING_1_MIDDLE);
    const XString middleXString(CHINESE_UNICODE_STRING_1_MIDDLE);

    const std::string beginStdString = XString::ConvertWideStringToMultiByteString(CHINESE_UNICODE_STRING_1_BEGIN);
    const XString beginXString(CHINESE_UNICODE_STRING_1_BEGIN);

    const std::string notContanedStdString = XString::ConvertWideStringToMultiByteString(CHINESE_UNICODE_STRING_1_REORDERED);
    const XString notContanedXString(CHINESE_UNICODE_STRING_1_REORDERED);


    UT_ASSERT(a.Contains(CHINESE_UNICODE_STRING_1_END));
    UT_ASSERT(a.Contains(endStdString));
    UT_ASSERT(a.Contains(endXString));

    UT_ASSERT(a.Contains(beginXString));
    UT_ASSERT(a.Contains(CHINESE_UNICODE_STRING_1_BEGIN));
    UT_ASSERT(a.Contains(beginStdString));

    UT_ASSERT(a.Contains(middleStdString));
    UT_ASSERT(a.Contains(CHINESE_UNICODE_STRING_1_MIDDLE));
    UT_ASSERT(a.Contains(middleXString));

    UT_ASSERT(!a.Contains(CHINESE_UNICODE_STRING_1_REORDERED));
    UT_ASSERT(!a.Contains(notContanedStdString));
    UT_ASSERT(!a.Contains(notContanedXString));
}

void XStringTest::TestContains()
{
    {
        const XString a = "I swear, by my life and my love of it...";
        const XString b = " my life and my ";
        UT_ASSERT(a.Contains(b));
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        const std::string b = " my life and my ";
        UT_ASSERT(a.Contains(b));
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        UT_ASSERT(a.Contains(" my life and my "));
    }

    {
        const XString a = "I swear, by my life and my love of it...";
        const XString b = " my life and mylove";
        UT_ASSERT(!a.Contains(b));
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        const std::string b = " my life and mylove";
        UT_ASSERT(!a.Contains(b));
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        UT_ASSERT(!a.Contains(" my life and mylove"));
    }
}

void XStringTest::TestIsInteger()
{
    UT_ASSERT(!XString("").IsInteger());
    UT_ASSERT(!XString(" ").IsInteger());
    UT_ASSERT(!XString(".").IsInteger());
    UT_ASSERT(!XString(".0").IsInteger());
    UT_ASSERT(!XString("0.").IsInteger());
    UT_ASSERT(!XString("0.0").IsInteger());
    UT_ASSERT(!XString("12a").IsInteger());
    UT_ASSERT(!XString("a12").IsInteger());
    UT_ASSERT(!XString("a").IsInteger());
    UT_ASSERT(!XString("a.0").IsInteger());
    UT_ASSERT(!XString("0.a").IsInteger());

    UT_ASSERT(!XString("-").IsInteger());
    UT_ASSERT(!XString(" -").IsInteger());
    UT_ASSERT(!XString("- ").IsInteger());
    UT_ASSERT(!XString("-.").IsInteger());
    UT_ASSERT(!XString("-.0").IsInteger());
    UT_ASSERT(!XString("-0.").IsInteger());
    UT_ASSERT(!XString("-0.0").IsInteger());
    UT_ASSERT(!XString("-12a").IsInteger());
    UT_ASSERT(!XString("-a12").IsInteger());
    UT_ASSERT(!XString("-a").IsInteger());
    UT_ASSERT(!XString("-a.0").IsInteger());
    UT_ASSERT(!XString("-0.a").IsInteger());
    UT_ASSERT(!XString("- 42").IsInteger());
    UT_ASSERT(!XString("42-").IsInteger());

    UT_ASSERT(XString("0").IsInteger());
    UT_ASSERT(XString("42").IsInteger());
    UT_ASSERT(XString(" 42").IsInteger());
    UT_ASSERT(XString("42 ").IsInteger());
    UT_ASSERT(XString(" 42 ").IsInteger());

    UT_ASSERT(XString("-0").IsInteger());
    UT_ASSERT(XString("-42").IsInteger());
    UT_ASSERT(XString(" -42").IsInteger());
    UT_ASSERT(XString("-42 ").IsInteger());
    UT_ASSERT(XString(" -42 ").IsInteger());

    UT_ASSERT(!XString("").IsInteger(false));
    UT_ASSERT(!XString(" ").IsInteger(false));
    UT_ASSERT(!XString(".").IsInteger(false));
    UT_ASSERT(!XString(".0").IsInteger(false));
    UT_ASSERT(!XString("0.").IsInteger(false));
    UT_ASSERT(!XString("0.0").IsInteger(false));
    UT_ASSERT(!XString("12a").IsInteger(false));
    UT_ASSERT(!XString("a12").IsInteger(false));
    UT_ASSERT(!XString("a").IsInteger(false));
    UT_ASSERT(!XString("a.0").IsInteger(false));
    UT_ASSERT(!XString("0.a").IsInteger(false));

    UT_ASSERT(!XString("-").IsInteger(false));
    UT_ASSERT(!XString(" -").IsInteger(false));
    UT_ASSERT(!XString("- ").IsInteger(false));
    UT_ASSERT(!XString("-.").IsInteger(false));
    UT_ASSERT(!XString("-.0").IsInteger(false));
    UT_ASSERT(!XString("-0.").IsInteger(false));
    UT_ASSERT(!XString("-0.0").IsInteger(false));
    UT_ASSERT(!XString("-12a").IsInteger(false));
    UT_ASSERT(!XString("-a12").IsInteger(false));
    UT_ASSERT(!XString("-a").IsInteger(false));
    UT_ASSERT(!XString("-a.0").IsInteger(false));
    UT_ASSERT(!XString("-0.a").IsInteger(false));
    UT_ASSERT(!XString("- 42").IsInteger(false));
    UT_ASSERT(!XString("42-").IsInteger(false));

    UT_ASSERT(XString("0").IsInteger(false));
    UT_ASSERT(XString("42").IsInteger(false));
    UT_ASSERT(XString(" 42").IsInteger(false));
    UT_ASSERT(XString("42 ").IsInteger(false));
    UT_ASSERT(XString(" 42 ").IsInteger(false));

    UT_ASSERT(!XString("-0").IsInteger(false));
    UT_ASSERT(!XString("-42").IsInteger(false));
    UT_ASSERT(!XString(" -42").IsInteger(false));
    UT_ASSERT(!XString("-42 ").IsInteger(false));
    UT_ASSERT(!XString(" -42 ").IsInteger(false));
}

void XStringTest::TestIsDouble()
{
    UT_ASSERT(!XString("").IsDouble());
    UT_ASSERT(!XString(" ").IsDouble());
    UT_ASSERT(!XString(".").IsDouble());
    UT_ASSERT(!XString("12a").IsDouble());
    UT_ASSERT(!XString("a12").IsDouble());
    UT_ASSERT(!XString("a").IsDouble());
    UT_ASSERT(!XString("a.0").IsDouble());
    UT_ASSERT(!XString("0.a").IsDouble());

    UT_ASSERT(!XString("-").IsDouble());
    UT_ASSERT(!XString(" -").IsDouble());
    UT_ASSERT(!XString("- ").IsDouble());
    UT_ASSERT(!XString("-.").IsDouble());
    UT_ASSERT(!XString("-12a").IsDouble());
    UT_ASSERT(!XString("-a12").IsDouble());
    UT_ASSERT(!XString("-a").IsDouble());
    UT_ASSERT(!XString("-a.0").IsDouble());
    UT_ASSERT(!XString("-0.a").IsDouble());
    UT_ASSERT(!XString("- 42").IsDouble());
    UT_ASSERT(!XString("42-").IsDouble());

    UT_ASSERT(XString("0").IsDouble());
    UT_ASSERT(XString("42").IsDouble());
    UT_ASSERT(XString(" 42").IsDouble());
    UT_ASSERT(XString("42 ").IsDouble());
    UT_ASSERT(XString(" 42 ").IsDouble());
    UT_ASSERT(XString("0.").IsDouble());
    UT_ASSERT(XString(".0").IsDouble());
    UT_ASSERT(XString("42.1").IsDouble());
    UT_ASSERT(XString(" 42.1").IsDouble());
    UT_ASSERT(XString("42.1 ").IsDouble());
    UT_ASSERT(XString(" 42.1 ").IsDouble());

    UT_ASSERT(XString("-0").IsDouble());
    UT_ASSERT(XString("-42").IsDouble());
    UT_ASSERT(XString(" -42").IsDouble());
    UT_ASSERT(XString("-42 ").IsDouble());
    UT_ASSERT(XString(" -42 ").IsDouble());
    UT_ASSERT(XString("-0.").IsDouble());
    UT_ASSERT(XString("-.0").IsDouble());
    UT_ASSERT(XString("-42.1").IsDouble());
    UT_ASSERT(XString(" -42.1").IsDouble());
    UT_ASSERT(XString("-42.1 ").IsDouble());
    UT_ASSERT(XString(" -42.1 ").IsDouble());
}

void XStringTest::TestLStripInPlace()
{
    try
    {
        XString a = "This is a non-stripping\nstring";
        XString b = "\n\nThis XString Needs Stripping\n \n";
        XString c = "Only Stripping on \nthe\n\nright   \n";
        XString d = "NOSPACES";
        XString e = "";

        a.LStripInPlace();
        UT_ASSERT("This is a non-stripping\nstring" == a);

        b.LStripInPlace();
        UT_ASSERT("This XString Needs Stripping\n \n" == b);

        c.LStripInPlace();
        UT_ASSERT("Only Stripping on \nthe\n\nright   \n" == c);

        d.LStripInPlace();
        UT_ASSERT("NOSPACES" == d);

        e.LStripInPlace();
        UT_ASSERT("" == e);
    }
    catch(std::bad_cast& ex)
    {
        cout << "Caught Bad Cast" << endl;
        cout << ex.what() << endl;
    }
}

void XStringTest::TestRStripInPlace()
{
    XString a = "This is a non-stripping\nstring";
    XString b = "\n\nThis XString Needs Stripping\n \n";
    XString c = "Only Stripping on \nthe\n\nright   \n";
    XString d = "NOSPACES";
    XString e = "";

    a.RStripInPlace();
    UT_ASSERT("This is a non-stripping\nstring" == a);

    b.RStripInPlace();
    UT_ASSERT("\n\nThis XString Needs Stripping" == b);

    c.RStripInPlace();
    UT_ASSERT("Only Stripping on \nthe\n\nright" == c);

    d.RStripInPlace();
    UT_ASSERT("NOSPACES" == d);

    e.RStripInPlace();
    UT_ASSERT("" == e);
}

void XStringTest::TestStripInPlace()
{
    XString a = "This is a non-stripping\nstring";
    XString b = "\n\nThis XString Needs Stripping\n \n";
    XString c = "Only Stripping on \nthe\n\nright   \n";
    XString d = "NOSPACES";
    XString e = "";

    a.StripInPlace();
    UT_ASSERT("This is a non-stripping\nstring" == a);

    b.StripInPlace();
    UT_ASSERT("This XString Needs Stripping" == b);

    c.StripInPlace();
    UT_ASSERT("Only Stripping on \nthe\n\nright" == c);

    d.StripInPlace();
    UT_ASSERT("NOSPACES" == d);

    e.StripInPlace();
    UT_ASSERT("" == e);
}

void XStringTest::TestLStrip()
{
    XString a = "This is a non-stripping\nstring";
    XString b = "\n\nThis XString Needs Stripping\n \n";
    XString c = "Only Stripping on \nthe\n\nright   \n";
    XString d = "NOSPACES";
    XString e = "";
    XString f(CHINESE_UNICODE_WITH_LEFT_SAPCE);
    XString g(CHINESE_UNICODE_WITH_NO_SAPCE);

    XString retval;

    retval = a.LStrip();
    UT_ASSERT("This is a non-stripping\nstring" == retval);

    retval = b.LStrip();
    UT_ASSERT("This XString Needs Stripping\n \n" == retval);

    retval = c.LStrip();
    UT_ASSERT("Only Stripping on \nthe\n\nright   \n" == retval);

    retval = d.LStrip();
    UT_ASSERT("NOSPACES" == retval);

    retval = e.LStrip();
    UT_ASSERT("" == retval);

    retval = g.LStrip();
    UT_ASSERT(retval == g);

    retval = f.LStrip();
    UT_ASSERT(retval == g);

}

void XStringTest::TestRStrip()
{
    XString a = "This is a non-stripping\nstring";
    XString b = "\n\nThis XString Needs Stripping\n \n";
    XString c = "Only Stripping on \nthe\n\nright   \n";
    XString d = "NOSPACES";
    XString e = "";
    XString f(CHINESE_UNICODE_WITH_SAPCE_AT_ENDS);
    XString g(CHINESE_UNICODE_WITH_LEFT_SAPCE);

    XString retval;

    retval = a.RStrip();
    UT_ASSERT("This is a non-stripping\nstring" == retval);

    retval = b.RStrip();
    UT_ASSERT("\n\nThis XString Needs Stripping" == retval);

    retval = c.RStrip();
    UT_ASSERT("Only Stripping on \nthe\n\nright" == retval);

    retval = d.RStrip();
    UT_ASSERT("NOSPACES" == retval);

    retval = e.RStrip();
    UT_ASSERT("" == retval);

    retval = f.RStrip();
    UT_ASSERT(retval == g);
}

void XStringTest::TestStrip()
{
    XString a = "This is a non-stripping\nstring";
    XString b = "\n\nThis XString Needs Stripping\n \n";
    XString c = "Only Stripping on \nthe\n\nright   \n";
    XString d = "NOSPACES";
    XString e = "";
    XString f(ARABIC_UNICODE_TO_BE_STRIPPED);
    XString g(ARABIC_UNICODE_STRIPPED);

    XString retval;

    retval = a.Strip();
    UT_ASSERT("This is a non-stripping\nstring" == retval);

    retval = b.Strip();
    UT_ASSERT("This XString Needs Stripping" == retval);

    retval = c.Strip();
    UT_ASSERT("Only Stripping on \nthe\n\nright" == retval);

    retval = d.Strip();
    UT_ASSERT("NOSPACES" == retval);

    retval = e.Strip();
    UT_ASSERT("" == retval);

    retval = f.Strip();
    UT_ASSERT(retval == g);
}

void XStringTest::TestStripEOL()
{
    UT_ASSERT(XString("hello").StripEOL() == "hello");
    UT_ASSERT(XString("hello\n").StripEOL() == "hello");
    UT_ASSERT(XString("hello\r").StripEOL() == "hello\r");
    UT_ASSERT(XString("hello\r\n").StripEOL() == "hello");
    UT_ASSERT(XString("hello\n\r").StripEOL() == "hello\n\r");
    UT_ASSERT(XString("hello\n\n").StripEOL() == "hello\n");
    UT_ASSERT(XString("hello\r\r").StripEOL() == "hello\r\r");
    UT_ASSERT(XString("hello\nxxx\n").StripEOL() == "hello\nxxx");
}

void XStringTest::TestFormat()
{
    const XString a = XString::Format("I have %d friends, %d faithful friends, more faithful could not be; and every night, by the dim firelight, they come to sit with me.", 2, 2);
    UT_ASSERT(a == "I have 2 friends, 2 faithful friends, more faithful could not be; and every night, by the dim firelight, they come to sit with me.");

    const XString b = XString::Format("%s/%s/%s", "/usr/local", "bin", "myexecutable");
    UT_ASSERT(b == "/usr/local/bin/myexecutable");

    const XString c = XString::Format("I32d I32u I32x");
    UT_ASSERT(c == "I32d I32u I32x");

    const XString d = XString::Format("I64d I64u I64x");
    UT_ASSERT(d == "I64d I64u I64x");

    const XString e = XString::Format("%%I32d %%I32u %%I32x %%d");
    UT_ASSERT(e == "%I32d %I32u %I32x %d");

    const XString f = XString::Format("%%I64d %%I64u %%I64x %%d");
    UT_ASSERT(f == "%I64d %I64u %I64x %d");

    const XString g = XString::Format("%I32d %I32u %I32x", (int32_t)-42, (int32_t)42, (int32_t)42);
    UT_ASSERT(g == "-42 42 2a");

    const XString h = XString::Format("%I64d %I64u %I64x", (int64_t)-42, (int64_t)42, (int64_t)42);
    UT_ASSERT(h == "-42 42 2a");

    const XString i = XString::Format("%d %f", 42, 42.1);
    UT_ASSERT(i == "42 42.100000");
}


void XStringTest::TestOperatorPlusUnicodeVersion()
{
    const XString full(LONG_CHINESE_UNICODE_STRING);
    const XString begining(LONG_CHINESE_UNICODE_STRING_BEGINING);
    const wchar_t* endWCharT = LONG_CHINESE_UNICODE_STRING_ENDING;
    const std::string endStdString = XString::ConvertWideStringToMultiByteString(LONG_CHINESE_UNICODE_STRING_ENDING);
    const XString endXString = LONG_CHINESE_UNICODE_STRING_ENDING;
    const char comma = ',';

    XString result;
    result = begining + comma + endWCharT;
    UT_ASSERT(result == full);
    result = begining + comma + endStdString;
    UT_ASSERT(result == full);
    result = begining + comma + endXString;
    UT_ASSERT(result == full);

}

void XStringTest::TestOperatorPlus()
{
    {
        const XString a = "the first half ";
        const XString b = "of the sentence.";
        XString c;

        c = a + b;

        UT_ASSERT(c == "the first half of the sentence.");
    }
    {
        const XString a = "the first half ";
        const char* b = "of the sentence.";
        XString c;

        c = a + b;

        UT_ASSERT(c == "the first half of the sentence.");
    }
    {
        const char* a = "the first half ";
        const XString b = "of the sentence.";
        XString c;

        c = a + b;

        UT_ASSERT(c == "the first half of the sentence.");
    }
    {
        const XString a = "hello";
        const char b = '.';
        const XString c = a + b;

        UT_ASSERT(c == "hello.");
    }
    {
        const XString a = "ello.";
        const char b = 'h';
        const XString c = b + a;

        UT_ASSERT(c == "hello.");
    }
}


void XStringTest::TestOperatorPlusEqualsUnicodeVersion()
{
    const XString full(LONG_CHINESE_UNICODE_STRING);
    const XString begining(LONG_CHINESE_UNICODE_STRING_BEGINING);
    const wchar_t* endWCharT = LONG_CHINESE_UNICODE_STRING_ENDING;
    const std::string endStdString = XString::ConvertWideStringToMultiByteString(LONG_CHINESE_UNICODE_STRING_ENDING);
    const XString endXString = LONG_CHINESE_UNICODE_STRING_ENDING;

    {
        XString result(begining);
        result += ',';
        result += endWCharT;
        UT_ASSERT(result == full);
    }
    {
        XString result(begining);
        result += ',';
        result += endXString;
        UT_ASSERT(result == full);
    }
    {
        XString result(begining);
        result += ',';
        result += endStdString;
        UT_ASSERT(result == full);
    }
}
void XStringTest::TestPlusEquals()
{
    XString a = "the first half ";

    a += "of the sentence.";

    UT_ASSERT(a == "the first half of the sentence.");
}

void XStringTest::TestSubString()
{
    const XString a = "Find the weasel.";
    const XString sub = a.substr(5, 3);

    UT_ASSERT(sub == "the");

    const XString russian = RUSSIAN_UNICODE_TO_BE_SEARCHED;
    const XString sought = RUSSIAN_UNICODE_TO_BE_FOUND;
    const XString subStr = russian.substr(russian.find(sought),sought.length());
    UT_ASSERT(sought == subStr);
}

void XStringTest::TestSplit()
{
    {
        const XString a = "Find the weasel.";

        vector<XString> parts;
        a.Split(" ", parts);
        UT_ASSERT(parts.size() == 3);

        vector<XString>::iterator iter = parts.begin();

        UT_ASSERT(*iter == "Find");
        ++iter;
        UT_ASSERT(*iter == "the");
        ++iter;
        UT_ASSERT(*iter == "weasel.");
    }

    {
        const XString a = "abcdefabcdefabcdef";

        vector<XString> parts;
        a.Split("ab", parts);
        UT_ASSERT(parts.size() == 3);

        vector<XString>::iterator iter = parts.begin();

        UT_ASSERT(*iter == "cdef");
        ++iter;
        UT_ASSERT(*iter == "cdef");
        ++iter;
        UT_ASSERT(*iter == "cdef");
    }

    {
        const XString a = "abcdefabcdefabcdef";

        vector<XString> parts;
        XString str = "bc";
        a.Split(str, parts);
        UT_ASSERT(parts.size() == 4);

        vector<XString>::iterator iter = parts.begin();

        UT_ASSERT(*iter == "a");
        ++iter;
        UT_ASSERT(*iter == "defa");
        ++iter;
        UT_ASSERT(*iter == "defa");
        ++iter;
        UT_ASSERT(*iter == "def");
    }

    {
        const XString a = "abcdefabcdefabcdef";

        vector<XString> parts;
        std::string stdstr = "ef";
        a.Split(stdstr, parts);
        UT_ASSERT(parts.size() == 3);

        vector<XString>::iterator iter = parts.begin();

        UT_ASSERT(*iter == "abcd");
        ++iter;
        UT_ASSERT(*iter == "abcd");
        ++iter;
        UT_ASSERT(*iter == "abcd");
    }

    {
        const XString a = "abcdefabcdefabcdef";

        vector<XString> parts;
        std::string stdstr = "ag";
        a.Split(stdstr, parts);
        UT_ASSERT(parts.size() == 1);

        UT_ASSERT(*parts.begin() == "abcdefabcdefabcdef");
    }
}


void XStringTest::TestSplitUnicode()
{
    {
        const XString a = UNICODE_STRING_TO_SPLIT;
        const XString piece0 = UNICODE_STRING_SPLIT_BY_CHARACTER_PIECE_0;
        const XString piece4 = UNICODE_STRING_SPLIT_BY_CHARACTER_PIECE_4;

        vector<XString> parts;
        a.Split(UNICODE_CHARACTER_TO_SPLIT_ON, parts);
        UT_ASSERT(parts.size() == 5);


        vector<XString>::iterator iter = parts.begin();
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
        const XString a = UNICODE_STRING_TO_SPLIT;
        const XString piece1 = UNICODE_STRING_SPLIT_BY_STRING_PIECE_1;
        vector<XString> parts;
        a.Split(UNICODE_STRING_TO_SPLIT_ON, parts);
        UT_ASSERT(parts.size() == 3);


        vector<XString>::iterator iter = parts.begin();
        UT_ASSERT(*iter == UNICODE_STRING_SPLIT_BY_STRING_PIECE_0);
        ++iter;
        UT_ASSERT(*iter == piece1);
        ++iter;
        UT_ASSERT(*iter == UNICODE_STRING_SPLIT_BY_STRING_PIECE_2);
    }

}

void XStringTest::TestReplaceAll()
{
    {
        const XString a = "I swear, by my life and my love of it...";
        const XString b = a.ReplaceAll(' ', '_');
        UT_ASSERT(b == "I_swear,_by_my_life_and_my_love_of_it...");
    }
    {
        XString a = "I swear, by my life and my love of it...";
        a.ReplaceAllInPlace(' ', '_');
        UT_ASSERT(a == "I_swear,_by_my_life_and_my_love_of_it...");
    }

    {
        const XString a = "I swear, by my life and my love of it...";
        const XString b = a.ReplaceAll("my", "your");
        UT_ASSERT(b == "I swear, by your life and your love of it...");
    }
    {
        XString a = "I swear, by my life and my love of it...";
        a.ReplaceAllInPlace("my", "your");
        UT_ASSERT(a == "I swear, by your life and your love of it...");
    }

    {
        const XString a = "I swear, by your life and your love of it...";
        const XString b = a.ReplaceAll("your", "my");
        UT_ASSERT(b == "I swear, by my life and my love of it...");
    }
    {
        XString a = "I swear, by your life and your love of it...";
        a.ReplaceAllInPlace("your", "my");
        UT_ASSERT(a == "I swear, by my life and my love of it...");
    }
}

void XStringTest::TestReplaceAllUnicode()
{
    XString replaced(UNICODE_STRING_AFTER_REPLACING);
    XString a(UNICODE_STRING_BEFORE_REPLACING);
    XString b = a.ReplaceAll(UNICODE_CHARACTER_TO_BE_REPLACED,UNICODE_CHARACTER_TO_REPLACE_WITH);

    UT_ASSERT(b == replaced);
    a = UNICODE_STRING_BEFORE_REPLACING;
    a.ReplaceAllInPlace(UNICODE_CHARACTER_TO_BE_REPLACED,UNICODE_CHARACTER_TO_REPLACE_WITH);
    UT_ASSERT(a == replaced);

}

void XStringTest::TestEraseAll()
{
    {
        const XString a = "I swear, by my life and my love of it...";
        const XString b = a.EraseAll('e');
        UT_ASSERT(b == "I swar, by my lif and my lov of it...");
    }
    {
        XString a = "I swear, by my life and my love of it...";
        a.EraseAllInPlace('e');
        UT_ASSERT(a == "I swar, by my lif and my lov of it...");
    }

    {
        const XString a = "I swear, by my life and my love of it...";
        wchar_t c = 'e';
        const XString b = a.EraseAll(c);
        UT_ASSERT(b == "I swar, by my lif and my lov of it...");
    }
    {
        XString a = "I swear, by my life and my love of it...";
        wchar_t c = 'e';
        a.EraseAllInPlace(c);
        UT_ASSERT(a == "I swar, by my lif and my lov of it...");
    }

    {
        const XString a = "I swear, by my life and my love of it...";
        const XString b = a.EraseAll("my");
        UT_ASSERT(b == "I swear, by  life and  love of it...");
    }
    {
        XString a = "I swear, by my life and my love of it...";
        a.EraseAllInPlace("my");
        UT_ASSERT(a == "I swear, by  life and  love of it...");
    }
}

void XStringTest::TestEraseAllUnicode()
{
    XString erased(UNICODE_STRING_AFTER_ERASE);
    XString a(UNICODE_STRING_TO_ERASE_FROM);
    XString b = a.EraseAll(UNICODE_CHARACTER_TO_BE_ERASED);

    UT_ASSERT(b == erased);
    a = UNICODE_STRING_TO_ERASE_FROM;
    a.EraseAllInPlace(UNICODE_CHARACTER_TO_BE_ERASED);
    UT_ASSERT(a == erased);
}

void XStringTest::TestFromInt()
{
    {
        const XString a = XString::FromInt(10);
        UT_ASSERT(a == "10");
    }
    {
        const XString a = XString::FromInt(-10);
        UT_ASSERT(a == "-10");
    }
}

void XStringTest::TestFromUInt()
{
    {
        const XString a = XString::FromUInt(10U);
        UT_ASSERT(a == "10");
    }
    {
        const XString a = XString::FromUInt(10U, 16);
        UT_ASSERT(a == "a");
    }

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString::FromUInt(10U, 8), XSDK::XException);
}

void XStringTest::TestFromInt8()
{
    {
        const XString a = XString::FromInt8(10);
        UT_ASSERT(a == "10");
    }
    {
        const XString a = XString::FromInt8(-10);
        UT_ASSERT(a == "-10");
    }
}

void XStringTest::TestFromUInt8()
{
    {
        const XString a = XString::FromUInt8(10U);
        UT_ASSERT(a == "10");
    }
    {
        const XString a = XString::FromUInt8(10U, 16);
        UT_ASSERT(a == "a");
    }

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString::FromUInt8(10U, 8), XSDK::XException);
}

void XStringTest::TestFromInt16()
{
    {
        const XString a = XString::FromInt16(10);
        UT_ASSERT(a == "10");
    }
    {
        const XString a = XString::FromInt16(-10);
        UT_ASSERT(a == "-10");
    }
}

void XStringTest::TestFromUInt16()
{
    {
        const XString a = XString::FromUInt16(10U);
        UT_ASSERT(a == "10");
    }
    {
        const XString a = XString::FromUInt16(10U, 16);
        UT_ASSERT(a == "a");
    }

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString::FromUInt16(10U, 8), XSDK::XException);
}

void XStringTest::TestFromInt32()
{
    {
        const XString a = XString::FromInt32(10);
        UT_ASSERT(a == "10");
    }
    {
        const XString a = XString::FromInt32(-10);
        UT_ASSERT(a == "-10");
    }

    {
        const XString a = XString::FromInt32(1073741828);
        UT_ASSERT(a == "1073741828");
    }
    {
        const XString a = XString::FromInt32(-1073741828);
        UT_ASSERT(a == "-1073741828");
    }
}

void XStringTest::TestFromUInt32()
{
    {
        const XString a = XString::FromUInt32(10U);
        UT_ASSERT(a == "10");
    }
    {
        const XString a = XString::FromUInt32(2147483652UL);
        UT_ASSERT(a == "2147483652");
    }
    {
        const XString a = XString::FromUInt32(10U, 16);
        UT_ASSERT(a == "a");
    }

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString::FromUInt32(10U, 8), XSDK::XException);
}

void XStringTest::TestFromInt64()
{
    {
        const XString a = XString::FromInt64(10);
        UT_ASSERT(a == "10");
    }
    {
        const XString a = XString::FromInt64(-10);
        UT_ASSERT(a == "-10");
    }

    {
        const XString a = XString::FromInt64(4611686018427387908LL);
        UT_ASSERT(a == "4611686018427387908");
    }
    {
        const XString a = XString::FromInt64(-4611686018427387908LL);
        UT_ASSERT(a == "-4611686018427387908");
    }
}

void XStringTest::TestFromUInt64()
{
    {
        const XString a = XString::FromUInt64(10U);
        UT_ASSERT(a == "10");
    }
    {
        const XString a = XString::FromUInt64(9223372036854775812ULL);
        UT_ASSERT(a == "9223372036854775812");
    }
    {
        const XString a = XString::FromUInt64(10U, 16);
        UT_ASSERT(a == "a");
    }

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString::FromUInt64(10U, 8), XSDK::XException);
}

void XStringTest::TestFromDouble()
{
    {
        const XString a = XString::FromDouble(10.0);
        UT_ASSERT(a == "10.000000");
    }
    {
        const XString a = XString::FromDouble(-10.0);
        UT_ASSERT(a == "-10.000000");
    }
}

void XStringTest::TestToInt()
{
    UT_ASSERT((XString("10").ToInt() * 10) == 100);
    UT_ASSERT((XString("-10").ToInt() * 10) == -100);
    UT_ASSERT((XString("10.7").ToInt(true) * 10) == 100);
    UT_ASSERT((XString("-10.7").ToInt(true) * 10) == -100);
    UT_ASSERT((XString("10").ToInt(16) * 10) == 160);
    UT_ASSERT((XString("-10").ToInt(16) * 10) == -160);

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10.7").ToInt(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10.7").ToInt(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10").ToInt(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10").ToInt(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("ten").ToInt(), XSDK::XException);
}

void XStringTest::TestToUInt()
{
    UT_ASSERT((XString("10").ToUInt() * 10) == 100);
    UT_ASSERT((XString("10.7").ToUInt(true) * 10) == 100);
    UT_ASSERT((XString("10").ToUInt(16) * 10) == 160);

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10").ToUInt(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10.7").ToUInt(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10.7").ToUInt(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10").ToUInt(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10").ToUInt(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("ten").ToUInt(), XSDK::XException);
}

void XStringTest::TestToInt8()
{
    UT_ASSERT((XString("10").ToInt8() * 10) == 100);
    UT_ASSERT((XString("-10").ToInt8() * 10) == -100);
    UT_ASSERT((XString("10.7").ToInt8(true) * 10) == 100);
    UT_ASSERT((XString("-10.7").ToInt8(true) * 10) == -100);
    UT_ASSERT((XString("10").ToInt8(16) * 10) == 160);
    UT_ASSERT((XString("-10").ToInt8(16) * 10) == -160);

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10.7").ToInt8(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10.7").ToInt8(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10").ToInt8(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10").ToInt8(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("ten").ToInt8(), XSDK::XException);
}

void XStringTest::TestToUInt8()
{
    UT_ASSERT((XString("10").ToUInt8() * 10) == 100);
    UT_ASSERT((XString("10.7").ToUInt8(true) * 10) == 100);
    UT_ASSERT((XString("10").ToUInt8(16) * 10) == 160);

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10").ToUInt8(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10.7").ToUInt8(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10.7").ToUInt8(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10").ToUInt8(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10").ToUInt8(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("ten").ToUInt8(), XSDK::XException);
}

void XStringTest::TestToInt16()
{
    UT_ASSERT((XString("10").ToInt16() * 10) == 100);
    UT_ASSERT((XString("-10").ToInt16() * 10) == -100);
    UT_ASSERT((XString("10.7").ToInt16(true) * 10) == 100);
    UT_ASSERT((XString("-10.7").ToInt16(true) * 10) == -100);
    UT_ASSERT((XString("10").ToInt16(16) * 10) == 160);
    UT_ASSERT((XString("-10").ToInt16(16) * 10) == -160);

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10.7").ToInt16(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10.7").ToInt16(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10").ToInt16(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10").ToInt16(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("ten").ToInt16(), XSDK::XException);
}

void XStringTest::TestToUInt16()
{
    UT_ASSERT((XString("10").ToUInt16() * 10) == 100);
    UT_ASSERT((XString("10.7").ToUInt16(true) * 10) == 100);
    UT_ASSERT((XString("10").ToUInt16(16) * 10) == 160);

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10").ToUInt16(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10.7").ToUInt16(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10.7").ToUInt16(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10").ToUInt16(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10").ToUInt16(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("ten").ToUInt16(), XSDK::XException);
}

void XStringTest::TestToInt32()
{
    UT_ASSERT((XString("10").ToInt32() * 10) == 100);
    UT_ASSERT((XString("-10").ToInt32() * 10) == -100);
    UT_ASSERT((XString("1073741828").ToInt32()) == 1073741828);
    UT_ASSERT((XString("-1073741828").ToInt32()) == -1073741828);
    UT_ASSERT((XString("10.7").ToInt32(true) * 10) == 100);
    UT_ASSERT((XString("-10.7").ToInt32(true) * 10) == -100);
    UT_ASSERT((XString("10").ToInt32(16) * 10) == 160);
    UT_ASSERT((XString("-10").ToInt32(16) * 10) == -160);

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10.7").ToInt32(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10.7").ToInt32(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10").ToInt32(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10").ToInt32(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("ten").ToInt32(), XSDK::XException);
}

void XStringTest::TestToUInt32()
{
    UT_ASSERT((XString("10").ToUInt32() * 10) == 100);
    UT_ASSERT((XString("4294967295").ToUInt32()) == (uint32_t)-1);
    UT_ASSERT((XString("10.7").ToUInt32(true) * 10) == 100);
    UT_ASSERT((XString("10").ToUInt32(16) * 10) == 160);

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10").ToUInt32(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10.7").ToUInt32(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10.7").ToUInt32(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10").ToUInt32(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10").ToUInt32(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("ten").ToUInt32(), XSDK::XException);
}

void XStringTest::TestToInt64()
{
    UT_ASSERT((XString("10").ToInt64() * 10) == 100);
    UT_ASSERT((XString("-10").ToInt64() * 10) == -100);
    UT_ASSERT((XString("4611686018427387908").ToInt64()) == 4611686018427387908LL);
    UT_ASSERT((XString("-4611686018427387908").ToInt64()) == -4611686018427387908LL);
    UT_ASSERT((XString("10.7").ToInt64(true) * 10) == 100);
    UT_ASSERT((XString("-10.7").ToInt64(true) * 10) == -100);
    UT_ASSERT((XString("10").ToInt64(16) * 10) == 160);
    UT_ASSERT((XString("-10").ToInt64(16) * 10) == -160);

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10.7").ToInt64(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10.7").ToInt64(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10").ToInt64(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10").ToInt64(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("ten").ToInt64(), XSDK::XException);
}

void XStringTest::TestToUInt64()
{
    UT_ASSERT((XString("10").ToUInt64() * 10) == 100);
    UT_ASSERT((XString("9223372036854775812").ToUInt64()) == 9223372036854775812ULL);
    UT_ASSERT((XString("10.7").ToUInt64(true) * 10) == 100);
    UT_ASSERT((XString("10").ToUInt64(16) * 10) == 160);

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10").ToUInt64(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10.7").ToUInt64(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10.7").ToUInt64(), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("10").ToUInt64(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("-10").ToUInt64(8), XSDK::XException);
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("ten").ToUInt64(), XSDK::XException);
}

void XStringTest::TestToDouble()
{
    UT_ASSERT(XString("10").ToDouble() == 10);
    UT_ASSERT(XString("10.7").ToDouble() == 10.7);
    UT_ASSERT(XString("-10.7").ToDouble() == -10.7);

    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", XString("ten").ToDouble(), XSDK::XException);
}

void XStringTest::TestToLower()
{
    {
        XString a = "I swear, by my life and my love of it...";
        a.ToLowerInPlace();
        UT_ASSERT(a == "i swear, by my life and my love of it...");
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        const XString b = a.ToLower();
        UT_ASSERT(b == "i swear, by my life and my love of it...");
    }
}

void XStringTest::TestToUpper()
{
    {
        const XString a = "I swear, by my life and my love of it...";
        const XString b = a.ToUpper();
        UT_ASSERT(b == "I SWEAR, BY MY LIFE AND MY LOVE OF IT...");
    }
    {
        XString a = "I swear, by my life and my love of it...";
        a.ToUpperInPlace();
        UT_ASSERT(a == "I SWEAR, BY MY LIFE AND MY LOVE OF IT...");
    }
}

void XStringTest::TestBase64Encode()
{
    uint8_t buf[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    XIRef<XMemory> membuf = new XMemory(10);
    memcpy(&membuf->Extend(10),buf,10);

    const XString encodedData = XString::ToBase64(membuf);
    const XString encodedData2 = XString::Base64Encode(membuf->Map(),membuf->GetDataSize());

    UT_ASSERT(encodedData == "AQIDBAUGBwgJCg==");
    UT_ASSERT(encodedData2 == "AQIDBAUGBwgJCg==");
}

void XStringTest::TestBase64Decode()
{
    uint8_t buf[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    const XString encodedData = "AQIDBAUGBwgJCg==";
    XIRef<XMemory> decoded;
    decoded = encodedData.FromBase64();

    UT_ASSERT(memcmp(buf, (uint8_t*)decoded->Map(), 10) == 0);
}

void XStringTest::TestFindFirst()
{
    {
        const XString a = "I swear, by my life and my love of it...";
        const XString toFind = "my";
        const int forwardPos = a.find(toFind);

        UT_ASSERT(forwardPos == 12);
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        const std::string toFind = "my";
        const int forwardPos = a.find(toFind);

        UT_ASSERT(forwardPos == 12);
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        const int forwardPos = a.find("my");

        UT_ASSERT(forwardPos == 12);
    }
}

void XStringTest::TestFindLast()
{
    {
        const XString a = "I swear, by my life and my love of it...";
        const XString toFind = "my";
        const int backwardPos = a.rfind(toFind);

        UT_ASSERT(backwardPos == 24);
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        const std::string toFind = "my";
        const int backwardPos = a.rfind(toFind);

        UT_ASSERT(backwardPos == 24);
    }
    {
        const XString a = "I swear, by my life and my love of it...";
        const int backwardPos = a.rfind("my");

        UT_ASSERT(backwardPos == 24);
    }
}


void XStringTest::TestFindFirstUnicode()
{
    const XString a = UNICODE_STRING_FOR_FINDING_VALUES_IN;

    {
        const XString toFind = UNICODE_STRING_FOR_FINDING;
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

void XStringTest::TestFindLastUnicode()
{
    const XString a = UNICODE_STRING_FOR_FINDING_VALUES_IN;

    {
        const XString toFind = UNICODE_STRING_FOR_FINDING;
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


void XStringTest::TestURIEncode()
{
    {
        const XString inputStr = "a & Z+ 26";
        const XString encoded = inputStr.URIEncode();
        UT_ASSERT(encoded == "a%20%26%20Z%2B%2026");
    }
    {
        XString inputStr = "a & Z+ 26";
        inputStr.URIEncodeInPlace();
        UT_ASSERT(inputStr == "a%20%26%20Z%2B%2026");
    }

    UT_ASSERT(XString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-._~").URIEncode() ==
                   XString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-._~"));

    for(char i = 1; i < '-'; ++i)
        UT_ASSERT(XString::Format("%c", i).URIEncode() == XString::Format("%%%02X", i));

    // + 1 is '.'
    for(char i = '-' + 2 ; i < '0'; ++i)
        UT_ASSERT(XString::Format("%c", i).URIEncode() == XString::Format("%%%02X", i));

    for(char i = '9' + 1 + 1; i < 'A'; ++i)
        UT_ASSERT(XString::Format("%c", i).URIEncode() == XString::Format("%%%02X", i));

    for(char i = 'Z' + 1; i < '_'; ++i)
        UT_ASSERT(XString::Format("%c", i).URIEncode() == XString::Format("%%%02X", i));

    for(char i = '_' + 1; i < 'a'; ++i)
        UT_ASSERT(XString::Format("%c", i).URIEncode() == XString::Format("%%%02X", i));

    for(char i = 'z' + 1; i < '~'; ++i)
        UT_ASSERT(XString::Format("%c", i).URIEncode() == XString::Format("%%%02X", i));

    UT_ASSERT(XString::Format("%c", 127).URIEncode() == "%7F");
}

void XStringTest::TestURIDecode()
{
    {
        const XString inputStr = "aBcdEfGh+%26%20Z%2B+26%26";
        const XString decoded = inputStr.URIDecode();
        UT_ASSERT(decoded == "aBcdEfGh & Z+ 26&");

        XString malformedStr = "a+%26+Z%2+26%26";
        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", malformedStr.URIDecode(), XSDK::XException);
    }
    {
        XString inputStr = "aBcdEfGh+%26%20Z%2B+26%26";
        inputStr.URIDecodeInPlace();
        UT_ASSERT(inputStr == "aBcdEfGh & Z+ 26&");

        XString malformedStr = "a+%26+Z%2+26%26";
        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", malformedStr.URIDecodeInPlace(), XSDK::XException);
    }
    {
        XString inputStr = "%2F%2f%2d%2D";
        inputStr.URIDecodeInPlace();
        UT_ASSERT(inputStr == "//--");
    }

    UT_ASSERT(XString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-._~").URIDecode() ==
                   XString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-._~"));

    //Wraps to -128 after incrementing past 127.
    for(char i = 1; i > 0; ++i)
        UT_ASSERT(XString::Format("%%%02X", i).URIDecode() == XString::Format("%c", i));

    for(char i = 1; i > 0; ++i)
        UT_ASSERT(XString::Format("%%%02x", i).URIDecode() == XString::Format("%c", i));
}


void XStringTest::TestConversions()
{
#ifdef WIN32
  //- START ---------------------- ConvertWideStringToMultiByteString
  size_t size = 0;
  //Size includes null, character, so we are removing that
  size = WideCharToMultiByte(CP_UTF8,0,CHINESE_UNICODE_STRING_1_END,-1,NULL,0,NULL,NULL);
  XMemory buffer(size*6);
  buffer.Extend(size*6);
  size_t conversionCount = 0;
  if ( (conversionCount = WideCharToMultiByte(CP_UTF8,0,CHINESE_UNICODE_STRING_1_END,-1,((char*)&buffer.Index(0)),size,NULL,NULL)) != size )
    UT_ASSERT(false);
  std::string result1(((char*)&buffer.Index(0)));
  //- END ---------------------- ConvertWideStringToMultiByteString
#else

  // I think our unicode constants need a "byte order mark" when building for ARM...
  // This test fails on ARM because wcstombs() returns -1. I tried even a very simple
  // single call to wcstombs() with a fixed buffer size and it still fails. I suspect
  // that the big endianness nature of ARM makes wide character strings more complicated...
  // Turns out there is something called a "byte order mark" defined for wide character
  // strings... I think our constants need a byter order mark... For now, just disabling
  // this test.
#if !defined(ARM)
  setlocale(LC_ALL,"en_US.UTF-8");
  char buffer[100];
  memset(buffer, 0, 100);
  size_t size = wcstombs(NULL,CHINESE_UNICODE_STRING_1_END,wcslen(CHINESE_UNICODE_STRING_1_END)+1);
  size_t sz = size;
  if( wcstombs(buffer,CHINESE_UNICODE_STRING_1_END,sz+1) != size )
      UT_ASSERT(false);
  std::string result1(buffer);
#endif

#endif

#if !defined(ARM)
  std::string result2 = XString::ConvertWideStringToMultiByteString(CHINESE_UNICODE_STRING_1_END);
  UT_ASSERT(result1 == result2);
#endif
}

void XStringTest::TestIsSpace()
{
    UT_ASSERT(!XString::IsSpace('p'));
    UT_ASSERT(!XString::IsSpace('\\'));
    UT_ASSERT(!XString::IsSpace('x'));

    UT_ASSERT(XString::IsSpace('\t'));
    UT_ASSERT(XString::IsSpace('\n'));
    UT_ASSERT(XString::IsSpace('\r'));
    UT_ASSERT(XString::IsSpace(' '));
}


























