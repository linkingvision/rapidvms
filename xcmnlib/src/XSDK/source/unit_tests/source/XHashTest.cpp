#include "XHashTest.h"
#include "XSDK/XHash.h"
#include "XSDK/XString.h"

using namespace XSDK;

REGISTER_TEST_FIXTURE(XHashTest);

void XHashTest::setup()
{
}

void XHashTest::teardown()
{
}

void XHashTest::TestDefaultConstructor()
{
    try
    {
        const XHash<int> hash;
        UT_ASSERT( hash.GetElemCount() == 0 );
        UT_ASSERT(hash.Empty());
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT( false && "Test threw!" );
    }
}

void XHashTest::TestCopyConstructor()
{
    try
    {
        XHash<int> hashA1;

        hashA1.Add("foo1", 1);
        hashA1.Add("foo2", 2);
        hashA1.Add("foo3", 3);
        hashA1.Add("foo4", 4);
        hashA1.Add("foo5", 5);

        XHash<int> hashA2(hashA1);

        UT_ASSERT(hashA1.GetElemCount() == hashA2.GetElemCount());

        unsigned int num = 0;
        int sum1 = 0;
        int sum2 = 0;
        for(XHash<int>::XHashIter iter1 = hashA1.GetIterator(),
                                iter2 = hashA2.GetIterator();
            iter1.IterValid() && iter2.IterValid();
            iter1.Iterate(), iter2.Iterate(), ++num)
        {
            UT_ASSERT(iter1.IterKey() == iter2.IterKey());
            UT_ASSERT(iter1.IterData() != iter2.IterData());
            UT_ASSERT(*iter1.IterData() == *iter2.IterData());
            sum1 += *iter1.IterData();
            sum2 += *iter2.IterData();
        }

        UT_ASSERT(num == hashA1.GetElemCount());
        UT_ASSERT(sum1 == sum2);
        UT_ASSERT(sum1 == 15);
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XHashTest::TestAssignmentOperator()
{
    try
    {
        int val = 0;
        XHash<int> base;
        val = 1;
        base.Add("foo1", val );
        val = 2;
        base.Add("foo2", val );
        val = 3;
        base.Add("foo3", val );
        val = 4;
        base.Add("foo4", val );
        val = 5;
        base.Add("foo5", val );

        int sum = 0;

        x_foreach( base )
        {
            sum += *base.IterData();
        }

        UT_ASSERT( sum == 15 );

        sum = 0;

        {
            XHash<int> other;

            // assignment...
            other = base;

            x_foreach( other )
            {
                sum += *other.IterData();
            }

            UT_ASSERT( sum == 15 );
        }
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT( false && "Test threw!" );
    }
}

void XHashTest::TestAdd()
{
    try
    {
        int val = 0;
        XHash<int> base;
        val = 1;
        base.Add("foo1", val );
        UT_ASSERT( base.GetElemCount() == 1 );
        UT_ASSERT(!base.Empty());
        val = 2;
        base.Add("foo2", val );
        UT_ASSERT( base.GetElemCount() == 2 );
        UT_ASSERT(!base.Empty());
        val = 3;
        base.Add("foo3", val );
        UT_ASSERT( base.GetElemCount() == 3 );
        UT_ASSERT(!base.Empty());
        val = 4;
        base.Add("foo4", val );
        const XString key = "foo5";
        val = 5;
        base.Add( key, val );
        UT_ASSERT( base.GetElemCount() == 5 );
        UT_ASSERT(!base.Empty());

    }

    catch(XSDK::XException&)
    {
        UT_ASSERT( false && "Test threw!" );
    }
}

void XHashTest::TestFind()
{
    try
    {
        int val = 0;
        XHash<int> base;
        val = 1;
        base.Add("foo1", val );
        UT_ASSERT( base.GetElemCount() == 1 );
        UT_ASSERT(!base.Empty());
        val = 2;
        base.Add("foo2", val );
        UT_ASSERT( base.GetElemCount() == 2 );
        UT_ASSERT(!base.Empty());
        val = 3;
        base.Add("foo3", val );
        UT_ASSERT( base.GetElemCount() == 3 );
        UT_ASSERT(!base.Empty());
        val = 4;
        base.Add("foo4", val );
        XString key = "foo5";
        val = 5;
        base.Add( key, val );
        UT_ASSERT( base.GetElemCount() == 5 );
        UT_ASSERT(!base.Empty());

        UT_ASSERT(base.Find("foo2"));
        UT_ASSERT(base.Find("foo3" ));
        UT_ASSERT(base.Find("FAKE KEY") == NULL );

        key = "foo1";
        UT_ASSERT( base.Find( key ) );

    }

    catch(XSDK::XException&)
    {
        UT_ASSERT( false && "Test threw!" );
    }
}


void XHashTest::TestRemove()
{
    try
    {
        int val = 0;
        XHash<int> base;
        val = 1;
        base.Add("foo1", val );
        UT_ASSERT( base.GetElemCount() == 1 );
        UT_ASSERT(!base.Empty());
        val = 2;
        base.Add("foo2", val );
        UT_ASSERT( base.GetElemCount() == 2 );
        UT_ASSERT(!base.Empty());
        val = 3;
        base.Add("foo3", val );
        UT_ASSERT( base.GetElemCount() == 3 );
        UT_ASSERT(!base.Empty());
        val = 4;
        base.Add("foo4", val );
        XString key = "foo5";
        val = 5;
        base.Add( key, val );
        UT_ASSERT( base.GetElemCount() == 5 );
        UT_ASSERT(!base.Empty());

        base.Remove("foo2" );
        key = "foo5";
        base.Remove( key );

        UT_ASSERT(base.Find("foo2") == NULL);
        UT_ASSERT( base.Find( key ) == NULL );

        UT_ASSERT(base.Find("foo1"));
        UT_ASSERT(base.Find("foo3"));
        UT_ASSERT(base.Find("foo4" ));
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT( false && "Test threw!" );
    }
}

void XHashTest::TestClear()
{
    try
    {
        int val = 0;
        XHash<int> base;
        val = 1;
        base.Add("foo1", val);
        UT_ASSERT( base.GetElemCount() == 1 );
        UT_ASSERT(!base.Empty());
        val = 2;
        base.Add("foo2", val);
        UT_ASSERT( base.GetElemCount() == 2 );
        UT_ASSERT(!base.Empty());
        val = 3;
        base.Add("foo3", val);
        UT_ASSERT( base.GetElemCount() == 3 );
        UT_ASSERT(!base.Empty());
        val = 4;
        base.Add("foo4", val);
        const XString key = "foo5";
        val = 5;
        base.Add( key, val );
        UT_ASSERT( base.GetElemCount() == 5 );
        UT_ASSERT(!base.Empty());

        base.Clear();

        UT_ASSERT( base.GetElemCount() == 0 );
        UT_ASSERT(base.Empty());

    }

    catch(XSDK::XException&)
    {
        UT_ASSERT( false && "Test threw!" );
    }
}

void XHashTest::TestIterationExternal()
{
    try
    {
        XHash<int> base;

        base.Add("foo1", 1);
        base.Add("foo2", 2);
        base.Add("foo3", 3);
        base.Add("foo4", 4);
        base.Add("foo5", 5);

        UT_ASSERT(base.GetIterator().IterValid());
        UT_ASSERT(base.GetIterator(false).IterValid());

        int sum = 0;

        for(XHash<int>::XHashIter iter = base.GetIterator();
            iter.IterValid();
            iter.Iterate())
        {
            sum += *iter.IterData();
        }

        UT_ASSERT(sum == 15);

        sum = 0;

        for(XHash<int>::XHashIter iter = base.GetIterator(false);
            iter.IterValid();
            iter.Iterate())
        {
            sum += *iter.IterData();
        }

        UT_ASSERT(sum == 15);

    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }

    try
    {
        XHash<int> base(Compare);

        base.Add("foo1", 1);
        base.Add("foo2", 2);
        base.Add("foo3", 3);
        base.Add("foo4", 4);
        base.Add("foo5", 5);

        UT_ASSERT(base.GetIterator().IterValid());
        UT_ASSERT(base.GetIterator(false).IterValid());

        int sum = 0;

        for(XHash<int>::XHashIter iter = base.GetIterator();
            iter.IterValid();
            iter.Iterate())
        {
            sum += *iter.IterData();
        }

        UT_ASSERT(sum == 15);

        sum = 0;

        for(XHash<int>::XHashIter iter = base.GetIterator(false);
            iter.IterValid();
            iter.Iterate())
        {
            sum += *iter.IterData();
        }

        UT_ASSERT(sum == 15);

    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }

    try
    {
        XHash<int> base;

        UT_ASSERT(!base.GetIterator().IterValid());
        UT_ASSERT(!base.GetIterator(false).IterValid());

        for(XHash<int>::XHashIter iter = base.GetIterator();
            iter.IterValid();
            iter.Iterate())
        {
            UT_ASSERT(false);
        }

        for(XHash<int>::XHashIter iter = base.GetIterator(false);
            iter.IterValid();
            iter.Iterate())
        {
            UT_ASSERT(false);
        }
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }

    try
    {
        XHash<int> base(Compare);

        UT_ASSERT(!base.GetIterator().IterValid());
        UT_ASSERT(!base.GetIterator(false).IterValid());

        for(XHash<int>::XHashIter iter = base.GetIterator();
            iter.IterValid();
            iter.Iterate())
        {
            UT_ASSERT(false);
        }

        for(XHash<int>::XHashIter iter = base.GetIterator(false);
            iter.IterValid();
            iter.Iterate())
        {
            UT_ASSERT(false);
        }
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XHashTest::TestIterationInternal()
{
    try
    {
        XHash<int> base;

        base.Add("foo1", 1);
        base.Add("foo2", 2);
        base.Add("foo3", 3);
        base.Add("foo4", 4);
        base.Add("foo5", 5);

        int sum = 0;

        x_foreach(base)
        {
            sum += *base.IterData();
        }

        UT_ASSERT(sum == 15);

        sum = 0;

        x_foreach_backward(base)
        {
            sum += *base.IterData();
        }

        UT_ASSERT(sum == 15);

    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }

    try
    {
        XHash<int> base(Compare);

        base.Add("foo1", 1);
        base.Add("foo2", 2);
        base.Add("foo3", 3);
        base.Add("foo4", 4);
        base.Add("foo5", 5);

        int sum = 0;

        x_foreach(base)
        {
            sum += *base.IterData();
        }

        UT_ASSERT(sum == 15);

        sum = 0;

        x_foreach_backward(base)
        {
            sum += *base.IterData();
        }

        UT_ASSERT(sum == 15);

    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }

    try
    {
        XHash<int> base;

        x_foreach(base)
        {
            UT_ASSERT(false);
        }

        x_foreach_backward(base)
        {
            UT_ASSERT(false);
        }
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }

    try
    {
        XHash<int> base(Compare);

        x_foreach(base)
        {
            UT_ASSERT(false);
        }

        x_foreach_backward(base)
        {
            UT_ASSERT(false);
        }
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XHashTest::TestHash()
{
    XHash<foo> fooHash;

    const foo a_foo(10);
    const foo b_foo(20);
    const foo c_foo(30);
    const foo d_foo(40);
    const foo e_foo(50);

    fooHash.Add("a_key", a_foo);
    fooHash.Add("b_key", b_foo );
    fooHash.Add("c_key", c_foo);
    fooHash.Add("d_key", d_foo);
    fooHash.Add("e_key", e_foo);

    UT_ASSERT(fooHash.Find("a_key"));
    UT_ASSERT(fooHash.Find("c_key"));
    UT_ASSERT(fooHash.Find("e_key"));
    UT_ASSERT(fooHash.Find("d_key"));
    UT_ASSERT(fooHash.Find("b_key"));

    UT_ASSERT( fooHash.GetElemCount() == 5 );
    UT_ASSERT(!fooHash.Empty());

    x_foreach(fooHash)
    {
        const XString key = fooHash.IterKey();
        UT_ASSERT( fooHash.Find( key ) );
    }

    fooHash.Remove("b_key");
    UT_ASSERT(fooHash.Find("b_key") == NULL);
    UT_ASSERT(fooHash.GetElemCount() == 4);
    UT_ASSERT(!fooHash.Empty());

    fooHash.Remove("d_key");
    UT_ASSERT(fooHash.Find("d_key") == NULL);
    UT_ASSERT(fooHash.GetElemCount() == 3);
    UT_ASSERT(!fooHash.Empty());

    fooHash.Add("b_key", b_foo);
    fooHash.Add("d_key", d_foo);
    UT_ASSERT( fooHash.GetElemCount() == 5 );
    UT_ASSERT(!fooHash.Empty());
}

void XHashTest::TestIterationOrderUnsorted()
{
    try
    {
        XHash<int> base;

        base.Add("foo1", 1);
        base.Add("foo5", 5);
        base.Add("foo3", 3);
        base.Add("foo2", 2);
        base.Add("foo4", 4);

        {
            XHash<int>::XHashIter iter = base.GetIterator();

            UT_ASSERT(iter.IterKey() == "foo1");
            UT_ASSERT(*iter.IterData() == 1);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo5");
            UT_ASSERT(*iter.IterData() == 5);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo3");
            UT_ASSERT(*iter.IterData() == 3);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo2");
            UT_ASSERT(*iter.IterData() == 2);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo4");
            UT_ASSERT(*iter.IterData() == 4);
            iter.Iterate();

            UT_ASSERT(!iter.IterValid());
        }
        {
            XHash<int>::XHashIter iter = base.GetIterator(false);

            UT_ASSERT(iter.IterKey() == "foo4");
            UT_ASSERT(*iter.IterData() == 4);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo2");
            UT_ASSERT(*iter.IterData() == 2);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo3");
            UT_ASSERT(*iter.IterData() == 3);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo5");
            UT_ASSERT(*iter.IterData() == 5);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo1");
            UT_ASSERT(*iter.IterData() == 1);
            iter.Iterate();

            UT_ASSERT(!iter.IterValid());
        }
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XHashTest::TestIterationOrderSorted()
{
    try
    {
        XHash<int> base(Compare);

        base.Add("foo1", 1);
        base.Add("foo5", 5);
        base.Add("foo3", 3);
        base.Add("foo2", 2);
        base.Add("foo4", 4);

        {
            XHash<int>::XHashIter iter = base.GetIterator();

            UT_ASSERT(iter.IterKey() == "foo1");
            UT_ASSERT(*iter.IterData() == 1);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo2");
            UT_ASSERT(*iter.IterData() == 2);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo3");
            UT_ASSERT(*iter.IterData() == 3);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo4");
            UT_ASSERT(*iter.IterData() == 4);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo5");
            UT_ASSERT(*iter.IterData() == 5);
            iter.Iterate();

            UT_ASSERT(!iter.IterValid());
        }
        {
            XHash<int>::XHashIter iter = base.GetIterator(false);

            UT_ASSERT(iter.IterKey() == "foo5");
            UT_ASSERT(*iter.IterData() == 5);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo4");
            UT_ASSERT(*iter.IterData() == 4);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo3");
            UT_ASSERT(*iter.IterData() == 3);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo2");
            UT_ASSERT(*iter.IterData() == 2);
            iter.Iterate();

            UT_ASSERT(iter.IterKey() == "foo1");
            UT_ASSERT(*iter.IterData() == 1);
            iter.Iterate();

            UT_ASSERT(!iter.IterValid());
        }
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XHashTest::TestAddUnicode()
{
   try
    {
        int val = 0;
        XHash<int> base;
        val = 1;
        base.Add(L"借刀杀人", val );
        UT_ASSERT( base.GetElemCount() == 1 );
        UT_ASSERT(!base.Empty());
        val = 2;
        base.Add(L"趁火打劫", val );
        UT_ASSERT( base.GetElemCount() == 2 );
        UT_ASSERT(!base.Empty());
        val = 3;
        base.Add(L"隔岸观火", val );
        UT_ASSERT( base.GetElemCount() == 3 );
        UT_ASSERT(!base.Empty());
        val = 4;
        base.Add(L"打草惊蛇", val );
        const XString key = L"指桑骂槐";
        val = 5;
        base.Add( key, val );
        UT_ASSERT( base.GetElemCount() == 5 );
        UT_ASSERT(!base.Empty());

    }

    catch(XSDK::XException&)
    {
        UT_ASSERT( false && "Test threw!" );
    }
}

void XHashTest::TestFindUnicode()
{
    try
    {
        int val = 0;
        XHash<int> base;
        val = 1;
        base.Add(L"借刀杀人", val );
        UT_ASSERT( base.GetElemCount() == 1 );
        UT_ASSERT(!base.Empty());
        val = 2;
        base.Add(L"Θα όλοι γνωρίζουμε είναι ότι δεν ξέρω τίποτα", val );
        UT_ASSERT( base.GetElemCount() == 2 );
        UT_ASSERT(!base.Empty());
        val = 3;
        base.Add(L"Гарантированного взаимного уничтожения", val );
        UT_ASSERT( base.GetElemCount() == 3 );
        UT_ASSERT(!base.Empty());
        val = 4;
        base.Add(L"打草惊蛇", val );
        XString key = L"指桑骂槐";
        val = 5;
        base.Add( key, val );
        UT_ASSERT( base.GetElemCount() == 5 );
        UT_ASSERT(!base.Empty());

        UT_ASSERT(base.Find(key)!=NULL);
        UT_ASSERT(base.Find(L"Гарантированного взаимного уничтожения" )!=NULL);
        UT_ASSERT(base.Find(L"趁火打劫") == NULL );


    }

    catch(XSDK::XException&)
    {
        UT_ASSERT( false && "Test threw!" );
    }
}


void XHashTest::TestRemoveUnicode()
{
    try
    {
        int val = 0;
        XHash<int> base;
        val = 1;
        base.Add(L"借刀杀人", val );
        UT_ASSERT( base.GetElemCount() == 1 );
        UT_ASSERT(!base.Empty());
        val = 2;
        base.Add(L"Θα όλοι γνωρίζουμε είναι ότι δεν ξέρω τίποτα", val );
        UT_ASSERT( base.GetElemCount() == 2 );
        UT_ASSERT(!base.Empty());
        val = 3;
        base.Add(L"Гарантированного взаимного уничтожения", val );
        UT_ASSERT( base.GetElemCount() == 3 );
        UT_ASSERT(!base.Empty());
        val = 4;
        base.Add(L"打草惊蛇", val );
        XString key = L"指桑骂槐";
        val = 5;
        base.Add( key, val );
        UT_ASSERT( base.GetElemCount() == 5 );
        UT_ASSERT(!base.Empty());

        UT_ASSERT(base.Find(key)!=NULL);
        UT_ASSERT(base.Find(L"Гарантированного взаимного уничтожения" )!=NULL);
        UT_ASSERT(base.Find(L"趁火打劫") == NULL );

        base.Remove(key);
        UT_ASSERT(base.Find(key) == NULL);
        UT_ASSERT( base.GetElemCount() == 4 );

                base.Remove(L"Θα όλοι γνωρίζουμε είναι ότι δεν ξέρω τίποτα");
        UT_ASSERT( base.Find( key ) == NULL );
        UT_ASSERT( base.GetElemCount() == 3 );


        UT_ASSERT(base.Find(L"打草惊蛇")!=NULL);
        UT_ASSERT(base.Find(L"借刀杀人")!=NULL);
        UT_ASSERT(base.Find(L"Гарантированного взаимного уничтожения" )!=NULL);
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT( false && "Test threw!" );
    }
}

void XHashTest::TestSubscriptOperator()
{
    XHash<int> hash;

    hash.Add( "1", 1 );
    hash.Add( "2", 2 );
    hash.Add( "3", 3 );

    int result = hash["1"] + hash["2"] + hash["3"];

    // subscript operator returns a copy of T by value, so the following line of code
    // would result in a compile error (complains about an lvalue required as left
    // operand of assignment).
    // hash["1"] = 5;

    UT_ASSERT( result == 6 );

    UT_ASSERT_THROWS( hash["42"], XException );
}
