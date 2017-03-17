
#include "XInternalListTest.h"
#include "XSDK/XInternalList.h"

#include "XSDK/XException.h"

REGISTER_TEST_FIXTURE(XInternalListTest);

using namespace std;
using namespace XSDK;

void XInternalListTest::setup()
{
}

void XInternalListTest::teardown()
{
}

int XInternalListTest_x_compare(const int& a, const int& b, void* context )
{
    if( a < b )
        return -1;
    else if( a > b )
        return 1;
    return 0;
}

void XInternalListTest::TestIterationExternal()
{
    try
    {
        XInternalList<int> l;

        l.Append(1);
        l.Append(2);
        l.Append(3);
        l.Append(4);
        l.Append(5);

        UT_ASSERT(l.GetIterator().IterValid());
        UT_ASSERT(l.GetIterator(false).IterValid());

        int sum = 0;

        for(XInternalList<int>::InternalListIter iter = l.GetIterator(false);
            iter.IterValid();
            iter.Iterate())
        {
            sum += *iter.IterData();
        }

        UT_ASSERT(sum == 15);

        sum = 0;

        for(XInternalList<int>::InternalListIter iter = l.GetIterator();
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
        XInternalList<int> l(XInternalListTest_x_compare);

        l.InsertSorted(1);
        l.InsertSorted(2);
        l.InsertSorted(3);
        l.InsertSorted(4);
        l.InsertSorted(5);

        UT_ASSERT(l.GetIterator().IterValid());
        UT_ASSERT(l.GetIterator(false).IterValid());

        int sum = 0;

        for(XInternalList<int>::InternalListIter iter = l.GetIterator(false);
            iter.IterValid();
            iter.Iterate())
        {
            sum += *iter.IterData();
        }

        UT_ASSERT(sum == 15);

        sum = 0;

        for(XInternalList<int>::InternalListIter iter = l.GetIterator();
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
        XInternalList<int> l;

        UT_ASSERT(!l.GetIterator().IterValid());
        UT_ASSERT(!l.GetIterator(false).IterValid());

        for(XInternalList<int>::InternalListIter iter = l.GetIterator(false);
            iter.IterValid();
            iter.Iterate())
        {
            UT_ASSERT(false);
        }

        for(XInternalList<int>::InternalListIter iter = l.GetIterator();
            iter.IterValid();
            iter.Iterate())
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
        XInternalList<int> l(XInternalListTest_x_compare);

        UT_ASSERT(!l.GetIterator().IterValid());
        UT_ASSERT(!l.GetIterator(false).IterValid());

        for(XInternalList<int>::InternalListIter iter = l.GetIterator(false);
            iter.IterValid();
            iter.Iterate())
        {
            UT_ASSERT(false);
        }

        for(XInternalList<int>::InternalListIter iter = l.GetIterator();
            iter.IterValid();
            iter.Iterate())
        {
            UT_ASSERT(false);
        }
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "List iteration test threw an exception.");
    }
}

void XInternalListTest::TestIterationInternal()
{
    try
    {
        XInternalList<int> l;

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
        XInternalList<int> l(XInternalListTest_x_compare);

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
        XInternalList<int> l;

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
        XInternalList<int> l(XInternalListTest_x_compare);

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

void XInternalListTest::TestNonPODAndMethodOrdersExternal()
{
    try
    {
        XInternalList<foo> fooList;

        const foo aFoo(10);
        const foo bFoo(20);
        const foo cFoo(30);
        const foo dFoo(40);
        const foo eFoo(50);

        fooList.Push(bFoo);
        UT_ASSERT(fooList.GetElemCount() == 1);
        UT_ASSERT(!fooList.Empty());
        fooList.Append(cFoo);
        UT_ASSERT(fooList.GetElemCount() == 2);
        UT_ASSERT(!fooList.Empty());
        fooList.Prepend(aFoo);
        UT_ASSERT(fooList.GetElemCount() == 3);
        UT_ASSERT(!fooList.Empty());
        fooList.Append(dFoo);
        UT_ASSERT(fooList.GetElemCount() == 4);
        UT_ASSERT(!fooList.Empty());
        fooList.Append(eFoo);
        UT_ASSERT(fooList.GetElemCount() == 5);
        UT_ASSERT(!fooList.Empty());

        const int expectedAnswers[5] = { 10, 20, 30, 40, 50 };

        // Iterate at make sure the values you get match our expected answers...
        {
            XInternalList<foo>::InternalListIter iter = fooList.GetIterator();
            for(int i = 0; i < 5; ++i, iter.Iterate())
            {
                foo& obj = *(iter.IterData());
                UT_ASSERT(obj.get() == expectedAnswers[i]);
            }
        }

        // Now, do it backward, making sure they all match...
        {
            XInternalList<foo>::InternalListIter iter = fooList.GetIterator(false);
            for(int i = 4; i >= 0; --i, iter.Iterate())
            {
                foo& obj = *(iter.IterData());
                UT_ASSERT(obj.get() == expectedAnswers[i]);
            }
        }

        //We'll let TestNonPODAndOrdersInternal test the ctor. There's
        //no reason to test that twice since it doesn't use the iterators.
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Non POD test failed");
    }
}

void XInternalListTest::TestNonPODAndMethodOrdersInternal()
{
    try
    {
        XInternalList<foo> fooList;

        const foo aFoo(10);
        const foo bFoo(20);
        const foo cFoo(30);
        const foo dFoo(40);
        const foo eFoo(50);

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

        const int expectedAnswers[5] = { 10, 20, 30, 40, 50 };

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
            const foo& obj = *(fooList.IterData());
            UT_ASSERT( obj.get() == expectedAnswers[i] );
            fooList.Iterate();
        }

        // test copy ctor...
        XInternalList<foo> anotherFooList = fooList;

        const foo f = anotherFooList.Pop();
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
        UT_ASSERT( false && "Non POD test failed" );
    }
}

void XInternalListTest::TestAppendAndGet()
{
    try
    {
        XInternalList<int> intListA;

        intListA.Append(1);
        intListA.Append(3);
        intListA.Append(2);
        intListA.Append(5);
        intListA.Append(4);

        const int expectedAnswers[5] = { 1, 3, 2, 5, 4 };

        // Iterate at make sure the values you get match our expected answers...
        for(int i = 0; i < 5; ++i)
        {
            int cv = *intListA.Get(i);
            UT_ASSERT(cv == expectedAnswers[i]);
        }
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "List appending test failed.");
    }
}

void XInternalListTest::TestPrependAndGet()
{
    try
    {
        XInternalList<int> intListA;

        intListA.Prepend(1);
        intListA.Prepend(3);
        intListA.Prepend(2);
        intListA.Prepend(5);
        intListA.Prepend(4);

        const int expectedAnswers[5] = { 4, 5, 2, 3, 1 };

        // Iterate at make sure the values you get match our expected answers...
        for(int i = 0; i < 5; ++i)
        {
            int cv = *intListA.Get(i);
            UT_ASSERT(cv == expectedAnswers[i]);
        }
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "List appending test failed.");
    }
}

void XInternalListTest::TestAppendListAndSubList()
{
    try
    {
        XInternalList<int> intListA;
        XInternalList<int> intListB;

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

void XInternalListTest::TestListInsertion()
{
    try
    {
        XInternalList<int> sorted(XInternalListTest_x_compare);
        XInternalList<int> unsorted;
        XInternalList<int> other;

        sorted.InsertSorted(0);
        sorted.InsertSorted(2);
        sorted.InsertSorted(9);
        sorted.InsertSorted(12);
        sorted.InsertSorted(7);

        other.Append(1);
        other.Append(3);
        other.Append(2);
        other.Append(11);
        other.Append(4);

        unsorted.AppendList(other);
        unsorted.AppendList(sorted);
        sorted.InsertListSorted(other);

        {
            const int expectedAnswers[10] = {0, 1, 2, 2, 3, 4, 7, 9, 11, 12};

            // Iterate at make sure the values you get match our expected answers...
            for(int i = 0; i < 10; ++i)
            {
                int cv = *sorted.Get(i);
                UT_ASSERT(cv == expectedAnswers[i]);
            }
        }
        {
            const int expectedAnswers[10] = {1, 3, 2, 11, 4, 0, 2, 7, 9, 12};

            // Iterate at make sure the values you get match our expected answers...
            for(int i = 0; i < 10; ++i)
            {
                int cv = *unsorted.Get(i);
                UT_ASSERT(cv == expectedAnswers[i]);
            }
        }
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test Failed!");
    }
}

void XInternalListTest::TestInsertBeforeIterExternal()
{
    try
    {
        XInternalList<int> intListA;

        // This variant tests InsertBeforeIter() against the middle of the list...
        intListA.Append(1);
        intListA.Append(3);
        intListA.Append(4);
        intListA.Append(5);

        {
            XInternalList<int>::InternalListIter iter = intListA.GetIterator();
            iter.Iterate();
            intListA.InsertBeforeIter(2, iter);
        }

        int expectedAnswers[5] = { 1, 2, 3, 4, 5 };

        // Iterate at make sure the values you get match our expected answers...
        {
            XInternalList<int>::InternalListIter iter = intListA.GetIterator();
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
            XInternalList<int>::InternalListIter iter = intListA.GetIterator();

            iter.Iterate();
            iter.Iterate();
            iter.Iterate();
            intListA.InsertBeforeIter(4, iter);
        }

        // Iterate at make sure the values you get match our expected answers...
        {
            XInternalList<int>::InternalListIter iter = intListA.GetIterator();
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
            XInternalList<int>::InternalListIter iter = intListA.GetIterator();
            intListA.InsertBeforeIter(1, iter);
        }

        // Iterate at make sure the values you get match our expected answers...
        {
            XInternalList<int>::InternalListIter iter = intListA.GetIterator();
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

void XInternalListTest::TestInsertBeforeIterInternal()
{
    try
    {
        XInternalList<int> intListA;

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

        const int expectedAnswers[5] = { 1, 2, 3, 4, 5 };

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
        UT_ASSERT( false && "List appending test failed." );
    }
}

void XInternalListTest::TestInsertSortedAndGet()
{
    try
    {
        XInternalList<int> intListA(XInternalListTest_x_compare);

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

void XInternalListTest::TestPop()
{
    try
    {
        XInternalList<int> intListA(XInternalListTest_x_compare);

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

void XInternalListTest::TestContains()
{
    try
    {
        XInternalList<int> intListA(XInternalListTest_x_compare);

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

void XInternalListTest::TestRemove()
{
    try
    {
        XInternalList<int> intListA(XInternalListTest_x_compare);

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

void XInternalListTest::TestRemoveAtIterExternal()
{
    try
    {
        XInternalList<int> intListA(XInternalListTest_x_compare);

        intListA.InsertSorted(1);
        intListA.InsertSorted(3);
        intListA.InsertSorted(2);
        intListA.InsertSorted(5);
        intListA.InsertSorted(4);
        intListA.InsertSorted(7);

        {
            XInternalList<int>::InternalListIter iter1 = intListA.GetIterator();
            iter1.Iterate();
            iter1.Iterate();

            XInternalList<int>::InternalListIter iter2 = iter1;
            iter2.Iterate();

            const XInternalList<int>::InternalListIter iter3 = intListA.RemoveAtIter(iter1);
            UT_ASSERT(iter3._current == iter2._current);
        }

        int sum = 0;

        for(XInternalList<int>::InternalListIter iter = intListA.GetIterator();
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

void XInternalListTest::TestRemoveAtIterInternal()
{
    try
    {
        XInternalList<int> intListA(XInternalListTest_x_compare);

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

void XInternalListTest::TestCtorForIterAndBadIter()
{
    try
    {
        XInternalList<int> intListA1;

        intListA1.Append(1);
        intListA1.Append(2);
        intListA1.Append(3);
        intListA1.Append(4);
        intListA1.Append(5);

        XInternalList<int> intListA2(intListA1);

        UT_ASSERT(intListA1.GetElemCount() == intListA2.GetElemCount());

        unsigned int num = 0;
        for(XInternalList<int>::InternalListIter iter1 = intListA1.GetIterator(),
                                      iter2 = intListA2.GetIterator();
            iter1.IterValid() && iter2.IterValid();
            iter1.Iterate(), iter2.Iterate(), ++num)
        {
            UT_ASSERT(iter1.IterData() != iter2.IterData());
            UT_ASSERT(*iter1.IterData() == *iter2.IterData());
        }

        UT_ASSERT(num == intListA1.GetElemCount());

        {
            XInternalList<int>::InternalListIter iter = intListA2.GetIterator();
            iter.Iterate();
            UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", intListA1.InsertBeforeIter(42, iter), XSDK::XException);
            UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", intListA1.RemoveAtIter(iter), XSDK::XException);
        }
        {
            XInternalList<int>::InternalListIter iter = intListA2.GetIterator(false);
            iter.Iterate();
            UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", intListA1.InsertBeforeIter(42, iter), XSDK::XException);
            UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", intListA1.RemoveAtIter(iter), XSDK::XException);
        }
        {
            XInternalList<int>::InternalListIter iter = intListA2.GetIterator(false);

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

void XInternalListTest::TestAllowedOperations()
{
    try
    {
        XInternalList<int> unsorted;
        XInternalList<int> other;

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
            XInternalList<int>::InternalListIter iter = unsorted.GetIterator();
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
            for(XInternalList<int>::InternalListIter iter = unsorted.GetIterator(); iter.IterValid(); iter.Iterate(), ++i)
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
        XInternalList<int> sorted(XInternalListTest_x_compare);
        XInternalList<int> other;

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
            XInternalList<int>::InternalListIter iter = sorted.GetIterator();
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

void XInternalListTest::TestIsSorted()
{
    try
    {
        const XInternalList<int> sorted(XInternalListTest_x_compare);
        const XInternalList<int> unsorted;

        UT_ASSERT(sorted.IsSorted());
        UT_ASSERT(!unsorted.IsSorted());
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XInternalListTest::TestBackwardIterPermissionsExternal()
{
    try
    {
        XInternalList<int> intListA;

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

void XInternalListTest::TestBackwardIterPermissionsInternal()
{
    try
    {
        XInternalList<int> intListA;

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

