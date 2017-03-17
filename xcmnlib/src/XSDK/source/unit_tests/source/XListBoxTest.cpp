
#include "XListBoxTest.h"
#include "XSDK/XListBox.h"
#include "XSDK/OS/STL.h"
#include <list>

REGISTER_TEST_FIXTURE(XListBoxTest);

using namespace std;
using namespace XSDK;

void XListBoxTest::setup()
{
}

void XListBoxTest::teardown()
{
}

int XListBoxTest_x_compare(const int& a, const int& b, void* context)
{
    if( a < b )
        return -1;
    else if( a > b )
        return 1;
    return 0;
}

void XListBoxTest::TestExternalIteration()
{
    try
    {
        list<int> ls;
        XListBox<int> l(ls);

        l.Append(1);
        l.Append(2);
        l.Append(3);
        l.Append(4);
        l.Append(5);

        UT_ASSERT(l.GetIterator().IterValid());
        UT_ASSERT(l.GetIterator(false).IterValid());

        int sum = 0;

        for(XListBox<int>::ListBoxIter iter = l.GetIterator(false);
            iter.IterValid();
            iter.Iterate())
        {
            sum += *iter.IterData();
        }

        UT_ASSERT(sum == 15);

        sum = 0;

        for(XListBox<int>::ListBoxIter iter = l.GetIterator();
            iter.IterValid();
            iter.Iterate())
        {
            sum += *iter.IterData();
        }

        UT_ASSERT(sum == 15);
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "List iteration test threw an exception.");
    }

    try
    {
        list<int> ls;
        XListBox<int> l(ls, XListBoxTest_x_compare);

        l.InsertSorted(1);
        l.InsertSorted(2);
        l.InsertSorted(3);
        l.InsertSorted(4);
        l.InsertSorted(5);

        UT_ASSERT(l.GetIterator().IterValid());
        UT_ASSERT(l.GetIterator(false).IterValid());

        int sum = 0;

        for(XListBox<int>::ListBoxIter iter = l.GetIterator(false);
            iter.IterValid();
            iter.Iterate())
        {
            sum += *iter.IterData();
        }

        UT_ASSERT(sum == 15);

        sum = 0;

        for(XListBox<int>::ListBoxIter iter = l.GetIterator();
            iter.IterValid();
            iter.Iterate())
        {
            sum += *iter.IterData();
        }

        UT_ASSERT(sum == 15);
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "List iteration test threw an exception.");
    }

    try
    {
        list<int> ls;
        XListBox<int> l(ls);

        UT_ASSERT(!l.GetIterator().IterValid());
        UT_ASSERT(!l.GetIterator(false).IterValid());

        for(XListBox<int>::ListBoxIter iter = l.GetIterator(false);
            iter.IterValid();
            iter.Iterate())
        {
            UT_ASSERT(false);
        }

        for(XListBox<int>::ListBoxIter iter = l.GetIterator();
            iter.IterValid();
            iter.Iterate())
        {
            UT_ASSERT(false);
        }
    }
    catch( XSDK::XException& )
    {
        UT_ASSERT( false && "List iteration test threw an exception." );
    }

    try
    {
        list<int> ls;
        XListBox<int> l(ls, XListBoxTest_x_compare);

        UT_ASSERT(!l.GetIterator().IterValid());
        UT_ASSERT(!l.GetIterator(false).IterValid());

        for(XListBox<int>::ListBoxIter iter = l.GetIterator(false);
            iter.IterValid();
            iter.Iterate())
        {
            UT_ASSERT(false);
        }

        for(XListBox<int>::ListBoxIter iter = l.GetIterator();
            iter.IterValid();
            iter.Iterate())
        {
            UT_ASSERT(false);
        }
    }
    catch( XSDK::XException& )
    {
        UT_ASSERT( false && "List iteration test threw an exception." );
    }
}

void XListBoxTest::TestInternalIteration()
{
    try
    {
        list<int> ls;
        XListBox<int> l(ls);

        l.Append(1);
        l.Append(2);
        l.Append(3);
        l.Append(4);
        l.Append(5);

        int sum = 0;
        x_foreach_backward(l)
        {
            sum += *l.IterData();
        }

        UT_ASSERT(sum == 15);

        sum = 0;

        x_foreach(l)
        {
            sum += *l.IterData();
        }

        UT_ASSERT(sum == 15);
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "List iteration test threw an exception.");
    }

    try
    {
        list<int> ls;
        XListBox<int> l(ls, XListBoxTest_x_compare);

        l.InsertSorted(1);
        l.InsertSorted(2);
        l.InsertSorted(3);
        l.InsertSorted(4);
        l.InsertSorted(5);

        int sum = 0;
        x_foreach_backward(l)
        {
            sum += *l.IterData();
        }

        UT_ASSERT(sum == 15);

        sum = 0;

        x_foreach(l)
        {
            sum += *l.IterData();
        }

        UT_ASSERT(sum == 15);
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "List iteration test threw an exception.");
    }

    try
    {
        list<int> ls;
        XListBox<int> l(ls);

        x_foreach_backward(l)
        {
            UT_ASSERT(false);
        }

        x_foreach(l)
        {
            UT_ASSERT(false);
        }
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "List iteration test threw an exception.");
    }

    try
    {
        list<int> ls;
        XListBox<int> l(ls, XListBoxTest_x_compare);

        x_foreach_backward(l)
        {
            UT_ASSERT(false);
        }

        x_foreach(l)
        {
            UT_ASSERT(false);
        }
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "List iteration test threw an exception.");
    }
}

void XListBoxTest::TestNonPODAndOrdersExternal()
{
    try
    {
        list<foo> fooListS;
        XListBox<foo> fooList(fooListS);

        foo aFoo(10);
        foo bFoo(20);
        foo cFoo(30);
        foo dFoo(40);
        foo eFoo(50);

        fooList.Push( bFoo );
        UT_ASSERT( fooList.GetElemCount() == 1 );
        UT_ASSERT(!fooList.Empty());
        fooList.Append( cFoo );
        UT_ASSERT( fooList.GetElemCount() == 2 );
        UT_ASSERT(!fooList.Empty());
        fooList.Prepend( aFoo );
        UT_ASSERT( fooList.GetElemCount() == 3 );
        UT_ASSERT(!fooList.Empty());
        fooList.Append( dFoo );
        UT_ASSERT( fooList.GetElemCount() == 4 );
        UT_ASSERT(!fooList.Empty());
        fooList.Append( eFoo );
        UT_ASSERT( fooList.GetElemCount() == 5 );
        UT_ASSERT(!fooList.Empty());

        int expectedAnswers[5] = { 10, 20, 30, 40, 50 };

        // Iterate at make sure the values you get match our expected answers...
        {
            XListBox<foo>::ListBoxIter iter = fooList.GetIterator();
            for(int i = 0; i < 5; ++i, iter.Iterate())
            {
                foo& obj = *(iter.IterData());
                UT_ASSERT(obj.get() == expectedAnswers[i]);
            }
        }

        // Now, do it backward, making sure they all match...
        {
            XListBox<foo>::ListBoxIter iter = fooList.GetIterator(false);
            for(int i = 4; i >= 0; --i, iter.Iterate())
            {
                foo& obj = *(iter.IterData());
                UT_ASSERT(obj.get() == expectedAnswers[i]);
            }
        }

        //We'll let TestNonPODAndOrdersInternal test the ctor. There's
        //no reason to test that twice since it doesn't use the iterators.
    }
    catch( XSDK::XException& )
    {
        UT_ASSERT( false && "Non POD external test failed" );
    }
}

void XListBoxTest::TestNonPODAndOrdersInternal()
{
    try
    {
        list<foo> fooListS;
        XListBox<foo> fooList(fooListS);

        foo aFoo(10);
        foo bFoo(20);
        foo cFoo(30);
        foo dFoo(40);
        foo eFoo(50);

        fooList.Push( bFoo );
        UT_ASSERT( fooList.GetElemCount() == 1 );
        UT_ASSERT(!fooList.Empty());
        fooList.Append( cFoo );
        UT_ASSERT( fooList.GetElemCount() == 2 );
        UT_ASSERT(!fooList.Empty());
        fooList.Prepend( aFoo );
        UT_ASSERT( fooList.GetElemCount() == 3 );
        UT_ASSERT(!fooList.Empty());
        fooList.Append( dFoo );
        UT_ASSERT( fooList.GetElemCount() == 4 );
        UT_ASSERT(!fooList.Empty());
        fooList.Append( eFoo );
        UT_ASSERT( fooList.GetElemCount() == 5 );
        UT_ASSERT(!fooList.Empty());

        int expectedAnswers[5] = { 10, 20, 30, 40, 50 };

        // Iterate at make sure the values you get match our expected answers...
        fooList.IterStart();
        for( int i = 0; i < 5; i++ )
        {
            foo& obj = *(fooList.IterData());
            UT_ASSERT( obj.get() == expectedAnswers[i] );
            fooList.Iterate();
        }

        // Now, do it backward, making sure they all match...
        fooList.IterStart(false);
        for( int i = 4; i >= 0; i-- )
        {
            foo& obj = *(fooList.IterData());
            UT_ASSERT( obj.get() == expectedAnswers[i] );
            fooList.Iterate();
        }

        // test copy ctor...
        list<foo> anotherFooListS;
        XListBox<foo> anotherFooList(anotherFooListS);
        anotherFooList = fooList;

        foo f = anotherFooList.Pop();
        UT_ASSERT( anotherFooList.GetElemCount() == 4 );
        UT_ASSERT(!anotherFooList.Empty());
        UT_ASSERT( f.get() == 50 );

        anotherFooList.Remove( aFoo );
        UT_ASSERT( anotherFooList.GetElemCount() == 3 );
        UT_ASSERT(!anotherFooList.Empty());
        anotherFooList.Prepend( aFoo );
        UT_ASSERT( anotherFooList.GetElemCount() == 4 );
        UT_ASSERT(!anotherFooList.Empty());
        anotherFooList.Append( f );
        UT_ASSERT( anotherFooList.GetElemCount() == 5 );
        UT_ASSERT(!anotherFooList.Empty());
    }

    catch( XSDK::XException& )
    {
        UT_ASSERT( false && "Non POD internal test failed" );
    }
}

void XListBoxTest::TestAppendListAndSubList()
{
    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS);
        list<int> intListBS;
        XListBox<int> intListB(intListBS);

        int val = 1;
        intListA.Append( val );
        val = 2;
        intListA.Append( val );
        val = 3;
        intListA.Append( val );
        val = 4;
        intListA.Append( val );
        val = 5;
        intListA.Append( val );

        val = 6;
        intListB.Append( val );
        val = 7;
        intListB.Append( val );
        val = 8;
        intListB.Append( val );
        val = 9;
        intListB.Append( val );
        val = 10;
        intListB.Append( val );

        intListA.AppendList( intListB );

        UT_ASSERT( intListA.GetElemCount() == 10 );
        UT_ASSERT(!intListA.Empty());

        int sum = 0;

        x_foreach( intListA )
        {
            sum += *intListA.IterData();
        }

        UT_ASSERT( sum == 55 );

        intListA.SubList( 5, 5 );

        sum = 0;

        x_foreach_backward( intListA )
        {
            sum += *intListA.IterData();
        }

        UT_ASSERT( sum == 40 );
    }

    catch( XSDK::XException& )
    {
        UT_ASSERT( false && "List appending test failed." );
    }
}

void XListBoxTest::TestInsertBeforeIterAndClearExternal()
{
    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS);

        // This variant tests InsertBeforeIter() against the middle of the list...
        intListA.Append(1);
        intListA.Append(3);
        intListA.Append(4);
        intListA.Append(5);

        {
            XListBox<int>::ListBoxIter iter = intListA.GetIterator();
            iter.Iterate();
            intListA.InsertBeforeIter(2, iter);
        }

        int expectedAnswers[5] = { 1, 2, 3, 4, 5 };

        // Iterate at make sure the values you get match our expected answers...
        {
            XListBox<int>::ListBoxIter iter = intListA.GetIterator();
            for(int i = 0; i < 5; ++i, iter.Iterate())
            {
                int cv = *(iter.IterData());
                UT_ASSERT(cv == expectedAnswers[i]);
            }
        }


        intListA.Clear();
        UT_ASSERT(intListA.GetElemCount() == 0);
        UT_ASSERT(intListA.Empty());

        // This variant tests InsertBeforeIter() against the END of a list...
        intListA.Append(1);
        intListA.Append(2);
        intListA.Append(3);
        intListA.Append(5);

        {
            XListBox<int>::ListBoxIter iter = intListA.GetIterator();

            iter.Iterate();
            iter.Iterate();
            iter.Iterate();
            intListA.InsertBeforeIter(4, iter);
        }

        // Iterate at make sure the values you get match our expected answers...
        {
            XListBox<int>::ListBoxIter iter = intListA.GetIterator();
            for(int i = 0; i < 5; ++i, iter.Iterate())
            {
                int cv = *(iter.IterData());
                UT_ASSERT(cv == expectedAnswers[i]);
            }
        }


        intListA.Clear();
        UT_ASSERT(intListA.GetElemCount() == 0);
        UT_ASSERT(intListA.Empty());

        // This variant tests InsertBeforeIter() against the beginning of a list...
        intListA.Append(2);
        intListA.Append(3);
        intListA.Append(4);
        intListA.Append(5);

        {
            XListBox<int>::ListBoxIter iter = intListA.GetIterator();
            intListA.InsertBeforeIter(1, iter);
        }

        // Iterate at make sure the values you get match our expected answers...
        {
            XListBox<int>::ListBoxIter iter = intListA.GetIterator();
            for(int i = 0; i < 5; ++i, iter.Iterate())
            {
                int cv = *(iter.IterData());
                UT_ASSERT(cv == expectedAnswers[i]);
            }
        }
    }
    catch( XSDK::XException& )
    {
        UT_ASSERT( false && "List insert before iter test failed." );
    }
}

void XListBoxTest::TestInsertBeforeIterAndClearInternal()
{
    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS);

        // This variant tests InsertBeforeIter() against the middle of the list...
        int val = 1;
        intListA.Append( val );
        val = 3;
        intListA.Append( val );
        val = 4;
        intListA.Append( val );
        val = 5;
        intListA.Append( val );

        intListA.IterStart();
        intListA.Iterate();
        val = 2;
        intListA.InsertBeforeIter( val );

        int expectedAnswers[5] = { 1, 2, 3, 4, 5 };

        // Iterate at make sure the values you get match our expected answers...
        intListA.IterStart();
        for( int i = 0; i < 5; i++ )
        {
            int cv = *(intListA.IterData());
            UT_ASSERT( cv == expectedAnswers[i] );
            intListA.Iterate();
        }


        intListA.Clear();
        UT_ASSERT( intListA.GetElemCount() == 0 );
        UT_ASSERT(intListA.Empty());

        // This variant tests InsertBeforeIter() against the END of a list...
        val = 1;
        intListA.Append( val );
        val = 2;
        intListA.Append( val );
        val = 3;
        intListA.Append( val );
        val = 5;
        intListA.Append( val );

        intListA.IterStart();
        intListA.Iterate(); intListA.Iterate(); intListA.Iterate();
        val = 4;
        intListA.InsertBeforeIter( val );

        // Iterate at make sure the values you get match our expected answers...
        intListA.IterStart();
        for( int i = 0; i < 5; i++ )
        {
            int cv = *(intListA.IterData());
            UT_ASSERT( cv == expectedAnswers[i] );
            intListA.Iterate();
        }


        intListA.Clear();
        UT_ASSERT( intListA.GetElemCount() == 0 );
        UT_ASSERT(intListA.Empty());

        // This variant tests InsertBeforeIter() against the beginning of a list...
        val = 2;
        intListA.Append( val );
        val = 3;
        intListA.Append( val );
        val = 4;
        intListA.Append( val );
        val = 5;
        intListA.Append( val );

        intListA.IterStart();
        val = 1;
        intListA.InsertBeforeIter( val );

        // Iterate at make sure the values you get match our expected answers...
        intListA.IterStart();
        for( int i = 0; i < 5; i++ )
        {
            int cv = *(intListA.IterData());
            UT_ASSERT( cv == expectedAnswers[i] );
            intListA.Iterate();
        }
    }

    catch( XSDK::XException& )
    {
        UT_ASSERT( false && "List insert before iter test failed." );
    }
}

void XListBoxTest::TestInsertSortedAndGet()
{
    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS, XListBoxTest_x_compare);

        intListA.InsertSorted(1);
        intListA.InsertSorted(3);
        intListA.InsertSorted(2);
        intListA.InsertSorted(5);
        intListA.InsertSorted(4);

        const int expectedAnswers[5] = { 1, 2, 3, 4, 5 };

        // Iterate at make sure the values you get match our expected answers...
        for(int i = 0; i < 5; ++i)
        {
            int cv = *intListA.Get(i);
            UT_ASSERT(cv == expectedAnswers[i]);
        }
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "List insert sorted test failed.");
    }
}

void XListBoxTest::TestInsertListSortedEmptyList()
{
    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS, XListBoxTest_x_compare);
        list<int> intListBS;
        XListBox<int> intListB(intListBS, XListBoxTest_x_compare);

        intListB.InsertSorted(10);
        intListB.InsertSorted(2);
        intListB.InsertSorted(5);
        intListB.InsertSorted(6);
        intListB.InsertSorted(3);

        const int expectedAnswers[5] = {2, 3, 5, 6, 10};

        intListA.InsertListSorted(intListB);

        // Iterate at make sure the values you get match our expected answers...
        for(int i = 0; i < 5; ++i)
        {
            int cv = *intListA.Get(i);
            UT_ASSERT(cv == expectedAnswers[i]);
        }
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test Failed!!!");
    }

    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS, XListBoxTest_x_compare);
        list<int> intListBS;
        XListBox<int> intListB(intListBS);

        intListB.Append(10);
        intListB.Append(2);
        intListB.Append(5);
        intListB.Append(6);
        intListB.Append(3);

        const int expectedAnswers[5] = {2, 3, 5, 6, 10};

        intListA.InsertListSorted(intListB);

        // Iterate at make sure the values you get match our expected answers...
        for(int i = 0; i < 5; ++i)
        {
            int cv = *intListA.Get(i);
            UT_ASSERT(cv == expectedAnswers[i]);
        }
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test Failed!!!");
    }

    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS, XListBoxTest_x_compare);
        list<int> intListBS;
        XListBox<int> intListB(intListBS, XListBoxTest_x_compare);

        intListA.InsertSorted(1);
        intListA.InsertSorted(9);
        intListA.InsertSorted(2);
        intListA.InsertSorted(7);
        intListA.InsertSorted(4);

        const int expectedAnswers[5] = {1, 2, 4, 7, 9};

        intListA.InsertListSorted(intListB);

        // Iterate at make sure the values you get match our expected answers...
        for(int i = 0; i < 5; ++i)
        {
            int cv = *intListA.Get(i);
            UT_ASSERT(cv == expectedAnswers[i]);
        }
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test Failed!!!");
    }
}

void XListBoxTest::TestInsertListSortedWithSorted()
{
    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS, XListBoxTest_x_compare);
        list<int> intListBS;
        XListBox<int> intListB(intListBS, XListBoxTest_x_compare);

        intListA.InsertSorted(1);
        intListA.InsertSorted(9);
        intListA.InsertSorted(2);
        intListA.InsertSorted(7);
        intListA.InsertSorted(4);

        intListB.InsertSorted(10);
        intListB.InsertSorted(2);
        intListB.InsertSorted(5);
        intListB.InsertSorted(6);
        intListB.InsertSorted(3);

        const int expectedAnswers[10] = {1, 2, 2, 3, 4, 5, 6, 7, 9, 10};

        intListA.InsertListSorted(intListB);

        // Iterate at make sure the values you get match our expected answers...
        for(int i = 0; i < 10; ++i)
        {
            int cv = *intListA.Get(i);
            UT_ASSERT(cv == expectedAnswers[i]);
        }
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test Failed!!!");
    }
}

void XListBoxTest::TestInsertListSortedWithUnsorted()
{
    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS, XListBoxTest_x_compare);
        list<int> intListBS;
        XListBox<int> intListB(intListBS);

        intListA.InsertSorted(1);
        intListA.InsertSorted(9);
        intListA.InsertSorted(2);
        intListA.InsertSorted(7);
        intListA.InsertSorted(4);

        intListB.Append(10);
        intListB.Append(2);
        intListB.Append(5);
        intListB.Append(6);
        intListB.Append(3);

        const int expectedAnswers[10] = {1, 2, 2, 3, 4, 5, 6, 7, 9, 10};

        intListA.InsertListSorted(intListB);

        // Iterate at make sure the values you get match our expected answers...
        for(int i = 0; i < 10; ++i)
        {
            int cv = *intListA.Get(i);
            UT_ASSERT(cv == expectedAnswers[i]);
        }
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test Failed!!!");
    }
}

void XListBoxTest::TestPop()
{
    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS, XListBoxTest_x_compare);

        intListA.InsertSorted(1);
        intListA.InsertSorted(3);
        intListA.InsertSorted(2);
        intListA.InsertSorted(5);
        intListA.InsertSorted(4);

        const int expectedAnswers[5] = { 1, 2, 3, 4, 5 };

        // Iterate at make sure the values you get match our expected answers...
        for(int i = 4; i >= 0; --i)
        {
            const int cv = intListA.Pop();

            UT_ASSERT(cv == expectedAnswers[i]);
        }
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "List popping test failed.");
    }
}

void XListBoxTest::TestContains()
{
    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS, XListBoxTest_x_compare);

        intListA.InsertSorted(1);
        intListA.InsertSorted(3);
        intListA.InsertSorted(2);
        intListA.InsertSorted(5);
        intListA.InsertSorted(4);

        const int expectedAnswers[5] = { 1, 2, 3, 4, 5 };

        // Iterate at make sure the values you get match our expected answers...
        for(int i = 4; i >= 0; --i)
            UT_ASSERT(intListA.Contains(expectedAnswers[i]));
    }

    catch(...)
    {
        UT_ASSERT(false && "List contains test failed.");
    }
}

void XListBoxTest::TestRemove()
{
    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS, XListBoxTest_x_compare);

        intListA.InsertSorted(1);
        intListA.InsertSorted(3);
        intListA.InsertSorted(2);
        intListA.InsertSorted(5);
        intListA.InsertSorted(4);

        intListA.Remove(3);

        int sum = 0;

        x_foreach(intListA)
        {
            sum += *intListA.IterData();
        }

        UT_ASSERT(sum == 12);
    }

    catch(...)
    {
        UT_ASSERT(false && "List appending test failed.");
    }
}

void XListBoxTest::TestRemoveAtIterExternal()
{
    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS, XListBoxTest_x_compare);

        intListA.InsertSorted(1);
        intListA.InsertSorted(3);
        intListA.InsertSorted(2);
        intListA.InsertSorted(5);
        intListA.InsertSorted(4);
        intListA.InsertSorted(7);

        {
            XListBox<int>::ListBoxIter iter1 = intListA.GetIterator();
            iter1.Iterate();
            iter1.Iterate();

            XListBox<int>::ListBoxIter iter2 = iter1;
            iter2.Iterate();

            const XListBox<int>::ListBoxIter iter3 = intListA.RemoveAtIter(iter1);
            UT_ASSERT(iter3._currIter == iter2._currIter);
        }

        int sum = 0;

        for(XListBox<int>::ListBoxIter iter = intListA.GetIterator();
            iter.IterValid();
            iter.Iterate())
        {
            sum += *iter.IterData();
        }

        UT_ASSERT(sum == 19);
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "List remove at test failed.");
    }
}

void XListBoxTest::TestRemoveAtIterInternal()
{
    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS, XListBoxTest_x_compare);

        intListA.InsertSorted(1);
        intListA.InsertSorted(3);
        intListA.InsertSorted(2);
        intListA.InsertSorted(5);
        intListA.InsertSorted(4);
        intListA.InsertSorted(7);

        intListA.IterStart();
        intListA.Iterate(); intListA.Iterate();
        intListA.RemoveAtIter();

        int sum = 0;

        x_foreach(intListA)
        {
            sum += *intListA.IterData();
        }

        UT_ASSERT(sum == 19);
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "List remove at test failed.");
    }
}

void XListBoxTest::TestSameStorageAndBadIter()
{
    try
    {
        list<int> intListAS;
        XListBox<int> intListA1(intListAS);

        intListA1.Append(1);
        intListA1.Append(2);
        intListA1.Append(3);
        intListA1.Append(4);
        intListA1.Append(5);

        XListBox<int> intListA2(intListAS);

        UT_ASSERT(intListA1.GetElemCount() == intListA2.GetElemCount());

        unsigned int num = 0;
        for(XListBox<int>::ListBoxIter iter1 = intListA1.GetIterator(),
                                      iter2 = intListA2.GetIterator();
            iter1.IterValid() && iter2.IterValid();
            iter1.Iterate(), iter2.Iterate(), ++num)
        {
            UT_ASSERT(iter1.IterData() == iter2.IterData());
        }

        UT_ASSERT(num == intListA1.GetElemCount());

        {
            XListBox<int>::ListBoxIter iter = intListA2.GetIterator();
            iter.Iterate();
            UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", intListA1.InsertBeforeIter(42, iter), XSDK::XException);
            UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", intListA1.RemoveAtIter(iter), XSDK::XException);
        }
        {
            XListBox<int>::ListBoxIter iter = intListA2.GetIterator(false);
            iter.Iterate();
            UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", intListA1.InsertBeforeIter(42, iter), XSDK::XException);
            UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", intListA1.RemoveAtIter(iter), XSDK::XException);
        }
        {
            XListBox<int>::ListBoxIter iter = intListA2.GetIterator(false);

            while(iter.IterValid())
                iter.Iterate();

            UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", intListA1.InsertBeforeIter(42, iter), XSDK::XException);
            UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", intListA1.RemoveAtIter(iter), XSDK::XException);
        }
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "List same storage test failed.");
    }
}

void XListBoxTest::TestAllowedOperations()
{
    try
    {
        list<int> forUnsorted;
        XListBox<int> unsorted(forUnsorted);
        list<int> forOther;
        XListBox<int> other(forOther);

        other.Append(50);
        other.Append(20);
        other.Append(10);
        other.Append(40);
        other.Append(30);

        unsorted.AppendList(other);
        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", unsorted.InsertListSorted(other), XSDK::XException);
        unsorted.SubList(1, 3);
        unsorted.Prepend(7);
        unsorted.Push(12);
        unsorted.Pop();

        {
            XListBox<int>::ListBoxIter iter = unsorted.GetIterator();
            iter.Iterate();

            unsorted.InsertBeforeIter(15, iter);
        }

        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", unsorted.InsertSorted(22), XSDK::XException);

        unsorted.Remove(20);
        unsorted.RemoveAtIter(unsorted.GetIterator());
        UT_ASSERT(unsorted.GetElemCount() == 3);

        const int expectedAnswers[3] = { 15, 10, 40 };

        // Iterate at make sure the values you get match our expected answers...
        {
            int i = 0;
            for(XListBox<int>::ListBoxIter iter = unsorted.GetIterator(); iter.IterValid(); iter.Iterate(), ++i)
            {
                int val = *(iter.IterData());
                UT_ASSERT(val == expectedAnswers[i]);
            }
            UT_ASSERT(i == 3);
        }
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }

    try
    {
        list<int> forSorted;
        XListBox<int> sorted(forSorted, XListBoxTest_x_compare);
        list<int> forOther;
        XListBox<int> other(forOther);

        other.Append(50);
        other.Append(20);
        other.Append(10);
        other.Append(40);
        other.Append(30);

        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", sorted.AppendList(other), XSDK::XException);
        sorted.InsertListSorted(other);
        sorted.SubList(1, 3);
        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", sorted.Prepend(7), XSDK::XException);
        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", sorted.Push(12), XSDK::XException);
        sorted.Pop();

        {
            XListBox<int>::ListBoxIter iter = sorted.GetIterator();
            iter.Iterate();

            UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", sorted.InsertBeforeIter(15, iter), XSDK::XException);
        }

        sorted.InsertSorted(22);
        sorted.Remove(20);
        sorted.RemoveAtIter(sorted.GetIterator());

        UT_ASSERT(sorted.GetElemCount() == 1);
        UT_ASSERT(*sorted.Get(0) == 30);
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XListBoxTest::TestIsSorted()
{
    try
    {
        list<int> forSorted;
        const XListBox<int> sorted(forSorted, XListBoxTest_x_compare);
        list<int> forUnsorted;
        const XListBox<int> unsorted(forUnsorted);

        UT_ASSERT(sorted.IsSorted());
        UT_ASSERT(!unsorted.IsSorted());
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XListBoxTest::TestBackwardIterPermissionsExternal()
{
    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS);

        intListA.Append(1);

        intListA.InsertBeforeIter(12, intListA.GetIterator());
        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", intListA.InsertBeforeIter(42, intListA.GetIterator(false)), XSDK::XException);
        intListA.RemoveAtIter(intListA.GetIterator());
        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", intListA.RemoveAtIter(intListA.GetIterator(false)), XSDK::XException);

        UT_ASSERT(intListA.GetElemCount() == 1);
        UT_ASSERT(*intListA.Get(0) == 1);
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test Failed!");
    }
}

void XListBoxTest::TestBackwardIterPermissionsInternal()
{
    try
    {
        list<int> intListAS;
        XListBox<int> intListA(intListAS);

        intListA.Append(1);

        intListA.IterStart();
        intListA.InsertBeforeIter(12);
        intListA.IterStart(false);
        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", intListA.InsertBeforeIter(42), XSDK::XException);
        intListA.IterStart();
        intListA.RemoveAtIter();
        intListA.IterStart(false);
        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", intListA.RemoveAtIter(), XSDK::XException);

        UT_ASSERT(intListA.GetElemCount() == 1);
        UT_ASSERT(*intListA.Get(0) == 1);
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test Failed!");
    }
}

