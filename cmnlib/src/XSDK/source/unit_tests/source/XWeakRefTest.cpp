
#include "XWeakRefTest.h"
#include <string>
#include <iostream>
#include "XSDK/XRef.h"
#include "XSDK/XWeakRef.h"

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XWeakRefTest);

// Helper macro for detecting the correct exception was thrown
#define XSDK_ASSERT_ORPHANED_THROW(exp) \
    try {   \
        (exp);  \
        UT_ASSERT_MESSAGE("XWeakRef didn't throw even though it's been orphaned.", false);    \
    }   \
    catch(XException&)    \
    {} \
    catch(...) \
    { \
        UT_ASSERT_MESSAGE("XWeakRef threw the wrong exception type when accessing an orphaned object.", false);   \
    }

class TestBase
{
public:
    TestBase(){}
    virtual ~TestBase(){}
};

class TestBase2 : public TestBase
{
public:
    TestBase2() : TestBase(){}
    virtual ~TestBase2(){}
};

class TestBase3 : public TestBase2
{
public:
    TestBase3() : TestBase2(){}
    virtual ~TestBase3(){}
};

class IInc
{
public:
    IInc(){}
    virtual int Increment(int value)=0;
    virtual ~IInc(){}
};

class IDec
{
public:
    IDec(){}
    virtual int Decrement(int value)=0;
    virtual ~IDec(){}
};

class MultiBase : public IInc,
                  public IDec
{
public:
    MultiBase(bool& deleted) :
        IInc(),
        IDec(),
        _deleted(deleted)
    {
        _deleted = false;
    }

    virtual ~MultiBase()
    {
        _deleted = true;
    }

    virtual int Increment(int value){ return ++value; }
    virtual int Decrement(int value){ return --value; }

private:
    bool& _deleted;
};

class MultiDerived : public TestBase3,
                     public MultiBase
{
public:
    MultiDerived(bool& deleted) :
        TestBase3(),
        MultiBase(deleted)
    {
    }

    virtual ~MultiDerived(){}
};

void XWeakRefTest::setup()
{
}

void XWeakRefTest::teardown()
{
}

void XWeakRefTest::TestDefaultConstructor()
{
    bool deleted = false;
    {
        XWeakRef<MultiDerived> wr;
        UT_ASSERT(wr.IsEmpty());
        UT_ASSERT(!wr.IsValid());
        UT_ASSERT_NO_THROW(wr.Clear());
        UT_ASSERT(wr.Get() == NULL);

        XRef<MultiDerived> a = new MultiDerived(deleted);
        UT_ASSERT_NO_THROW(wr = a);

        UT_ASSERT(a.GetReferenceCount() == 1);
        UT_ASSERT_NO_THROW(*wr);
        UT_ASSERT(wr->Increment(10) == 11);
        UT_ASSERT(wr == a);
        UT_ASSERT(!(wr != a));
        UT_ASSERT(wr.Get());
        UT_ASSERT(wr.IsOrphaned() == false);
        UT_ASSERT(wr.IsValid() == true);
        UT_ASSERT(wr.IsEmpty() == false);
        UT_ASSERT_NO_THROW(wr.Clear());

        UT_ASSERT(a.GetReferenceCount() == 1);

        UT_ASSERT(wr.IsEmpty());
        UT_ASSERT(!wr.IsValid());
        UT_ASSERT_NO_THROW(wr.Clear());
        UT_ASSERT(wr.Get() == NULL);
    }
    UT_ASSERT(deleted == true);

    {
        XWeakRef<MultiDerived> wr;
        UT_ASSERT(wr.IsEmpty());
        UT_ASSERT(!wr.IsValid());
        UT_ASSERT_NO_THROW(wr.Clear());
        UT_ASSERT(wr.Get() == NULL);

        XRef<MultiDerived> a = new MultiDerived(deleted);
        UT_ASSERT_NO_THROW(wr = a);

        UT_ASSERT(a.GetReferenceCount() == 1);
        UT_ASSERT_NO_THROW(*wr);
        UT_ASSERT(wr->Increment(10) == 11);
        UT_ASSERT(wr == a);
        UT_ASSERT(!(wr != a));
        UT_ASSERT(wr.Get());
        UT_ASSERT(wr.IsOrphaned() == false);
        UT_ASSERT(wr.IsValid() == true);
        UT_ASSERT(wr.IsEmpty() == false);
        UT_ASSERT_NO_THROW(wr.Clear());

        UT_ASSERT(a.GetReferenceCount() == 1);

        UT_ASSERT(wr.IsEmpty());
        UT_ASSERT(!wr.IsValid());
        UT_ASSERT_NO_THROW(wr.Clear());
        UT_ASSERT(wr.Get() == NULL);

        XWeakRef<MultiDerived> wr2;
        UT_ASSERT(wr2.IsEmpty());
        UT_ASSERT(!wr2.IsValid());
        UT_ASSERT_NO_THROW(wr2.Clear());
        UT_ASSERT(wr2.Get() == NULL);

        UT_ASSERT_NO_THROW(wr = a);
        UT_ASSERT_NO_THROW(wr2 = wr);

        UT_ASSERT(a.GetReferenceCount() == 1);
        UT_ASSERT_NO_THROW(*wr);
        UT_ASSERT_NO_THROW(*wr2);
        UT_ASSERT(wr->Increment(10) == 11);
        UT_ASSERT(wr2->Increment(10) == 11);
        UT_ASSERT(wr == a);
        UT_ASSERT(wr2 == a);
        UT_ASSERT(!(wr != a));
        UT_ASSERT(!(wr2 != a));
        UT_ASSERT(wr.Get());
        UT_ASSERT(wr2.Get());
        UT_ASSERT(wr.IsOrphaned() == false);
        UT_ASSERT(wr2.IsOrphaned() == false);
        UT_ASSERT(wr.IsValid() == true);
        UT_ASSERT(wr2.IsValid() == true);
        UT_ASSERT(wr.IsEmpty() == false);
        UT_ASSERT(wr2.IsEmpty() == false);
        UT_ASSERT_NO_THROW(wr.Clear());
        UT_ASSERT_NO_THROW(wr2.Clear());

        UT_ASSERT(a.GetReferenceCount() == 1);

        UT_ASSERT(wr.IsEmpty());
        UT_ASSERT(!wr.IsValid());
        UT_ASSERT_NO_THROW(wr.Clear());
        UT_ASSERT(wr.Get() == NULL);

        UT_ASSERT(wr2.IsEmpty());
        UT_ASSERT(!wr2.IsValid());
        UT_ASSERT_NO_THROW(wr2.Clear());
        UT_ASSERT(wr2.Get() == NULL);
    }
    UT_ASSERT(deleted == true);
}

// Test assigning an XRef to a WeakRef.
void XWeakRefTest::TestXRefAssignment()
{
    bool deleted = false;
    XRef<MultiBase> p = new MultiBase(deleted);
    UT_ASSERT(p.IsValid());

    {
        UT_ASSERT(p.GetReferenceCount() == 1);
        UT_ASSERT_NO_THROW(XWeakRef<MultiBase> wr(p));
        UT_ASSERT(p.GetReferenceCount() == 1);
    }

    UT_ASSERT(deleted == false);
    {
        UT_ASSERT(p.GetReferenceCount() == 1);
        UT_ASSERT_NO_THROW(XWeakRef<MultiBase> wr = p);
        UT_ASSERT(p.GetReferenceCount() == 1);
    }
    p.Clear();

    bool deleted_1 = false;
    bool deleted_2 = false;
    XRef<MultiBase> p1 = new MultiBase(deleted_1);
    XRef<MultiBase> p2 = new MultiBase(deleted_2);
    UT_ASSERT(p1.IsValid());
    UT_ASSERT(p2.IsValid());
    {
        UT_ASSERT(p1.GetReferenceCount() == 1);
        XWeakRef<MultiBase> wr = p1;
        UT_ASSERT(wr.IsValid());
        UT_ASSERT(deleted_1 == false);
        UT_ASSERT(p1.GetReferenceCount() == 1);
        UT_ASSERT_NO_THROW(wr = p2);
        UT_ASSERT(wr.IsValid());
        UT_ASSERT(deleted_1 == false);
        UT_ASSERT(deleted_2 == false);
        UT_ASSERT(p1.GetReferenceCount() == 1);
        UT_ASSERT(p2.GetReferenceCount() == 1);
    }

    p1.Clear();
    p2.Clear();
    UT_ASSERT(deleted_1 == true);
    UT_ASSERT(deleted_2 == true);

    {
        XWeakRef<MultiBase> wr1(p1);
        XWeakRef<MultiBase> wr2 = p2;

        UT_ASSERT(wr1 == wr2);
        UT_ASSERT(wr2 == wr1);
        UT_ASSERT(!(wr1 != wr2));
        UT_ASSERT(!(wr2 != wr1));
        UT_ASSERT(wr1.Get() == NULL);
        UT_ASSERT(wr2.Get() == NULL);
        UT_ASSERT(wr1.IsOrphaned() == false);
        UT_ASSERT(wr2.IsOrphaned() == false);
        UT_ASSERT(wr1.IsValid() == false);
        UT_ASSERT(wr2.IsValid() == false);
        UT_ASSERT(wr1.IsEmpty() == true);
        UT_ASSERT(wr2.IsEmpty() == true);
        UT_ASSERT_NO_THROW(wr1.Clear());
        UT_ASSERT_NO_THROW(wr2.Clear());

        UT_ASSERT_NO_THROW(wr1 = p2);
        UT_ASSERT_NO_THROW(wr2 = p1);

        UT_ASSERT(wr1 == wr2);
        UT_ASSERT(wr2 == wr1);
        UT_ASSERT(!(wr1 != wr2));
        UT_ASSERT(!(wr2 != wr1));
        UT_ASSERT(wr1.Get() == NULL);
        UT_ASSERT(wr2.Get() == NULL);
        UT_ASSERT(wr1.IsOrphaned() == false);
        UT_ASSERT(wr2.IsOrphaned() == false);
        UT_ASSERT(wr1.IsValid() == false);
        UT_ASSERT(wr2.IsValid() == false);
        UT_ASSERT(wr1.IsEmpty() == true);
        UT_ASSERT(wr2.IsEmpty() == true);
        UT_ASSERT_NO_THROW(wr1.Clear());
        UT_ASSERT_NO_THROW(wr2.Clear());

        UT_ASSERT_NO_THROW(p1 = new MultiBase(deleted_1));
        UT_ASSERT_NO_THROW(p2 = new MultiBase(deleted_2));

        UT_ASSERT_NO_THROW(wr1 = p1);
        UT_ASSERT_NO_THROW(wr2 = p2);

        UT_ASSERT_NO_THROW(*wr1);
        UT_ASSERT_NO_THROW(*wr2);
        UT_ASSERT_NO_THROW(wr1->Increment(10));
        UT_ASSERT_NO_THROW(wr2->Increment(10));
        UT_ASSERT(!(wr1 == wr2));
        UT_ASSERT(!(wr2 == wr1));
        UT_ASSERT(wr1 != wr2);
        UT_ASSERT(wr2 != wr1);
        UT_ASSERT(wr1.Get());
        UT_ASSERT(wr2.Get());
        UT_ASSERT(wr1.IsOrphaned() == false);
        UT_ASSERT(wr2.IsOrphaned() == false);
        UT_ASSERT(wr1.IsValid() == true);
        UT_ASSERT(wr2.IsValid() == true);
        UT_ASSERT(wr1.IsEmpty() == false);
        UT_ASSERT(wr2.IsEmpty() == false);
        UT_ASSERT_NO_THROW(wr1.Clear());
        UT_ASSERT_NO_THROW(wr2.Clear());

        UT_ASSERT_NO_THROW(p1.Clear());
        UT_ASSERT_NO_THROW(p2.Clear());
    }

    UT_ASSERT(deleted_1 == true);
    UT_ASSERT(deleted_2 == true);
}

// Test assigning one XWeakRef to another.
void XWeakRefTest::TestWeakRefAssignment()
{
    bool deleted = false;
    XRef<MultiDerived> p = new MultiDerived(deleted);
    UT_ASSERT(p.IsValid());

    {
        UT_ASSERT(p.GetReferenceCount() == 1);
        XWeakRef<MultiDerived> wr1(p);
        UT_ASSERT(wr1.IsValid());
        UT_ASSERT_NO_THROW(XWeakRef<MultiDerived> wr2(wr1));
        UT_ASSERT(wr1.IsValid());
        UT_ASSERT(p.GetReferenceCount() == 1);
    }

    UT_ASSERT(deleted == false);
    {
        UT_ASSERT(p.GetReferenceCount() == 1);
        XWeakRef<MultiDerived> wr1(p);
        UT_ASSERT(wr1.IsValid());
        XWeakRef<MultiDerived> wr2;
        UT_ASSERT_NO_THROW(wr2 = wr1);
        UT_ASSERT(wr1.IsValid());
        UT_ASSERT(wr2.IsValid());
        UT_ASSERT(p.GetReferenceCount() == 1);
    }
    p.Clear();

    // Test assigning when a pointer has already been assigned.
    bool deleted_1 = false;
    bool deleted_2 = false;
    XRef<MultiDerived> p1 = new MultiDerived(deleted_1);
    XRef<MultiDerived> p2 = new MultiDerived(deleted_2);
    UT_ASSERT(p1.IsValid());
    UT_ASSERT(p2.IsValid());
    {
        UT_ASSERT(p1.GetReferenceCount() == 1);
        XWeakRef<MultiDerived> wr1 = p1;
        XWeakRef<MultiDerived> wr2 = wr1;
        UT_ASSERT(wr1.IsValid());
        UT_ASSERT(wr2.IsValid());
        UT_ASSERT(deleted_1 == false);
        UT_ASSERT(deleted_2 == false);
        UT_ASSERT(p1.GetReferenceCount() == 1);
        UT_ASSERT(p2.GetReferenceCount() == 1);
        UT_ASSERT_NO_THROW(wr1 = wr2);
        UT_ASSERT(wr1.IsValid());
        UT_ASSERT(wr2.IsValid());
        UT_ASSERT(deleted_1 == false);
        UT_ASSERT(deleted_2 == false);
        UT_ASSERT(p1.GetReferenceCount() == 1);
        UT_ASSERT(p2.GetReferenceCount() == 1);
        UT_ASSERT(wr1->Increment(10) == 11);
        UT_ASSERT(wr2->Decrement(10) == 9);

        UT_ASSERT_NO_THROW(*wr1);
        UT_ASSERT_NO_THROW(*wr2);
        UT_ASSERT(wr1 == wr2);
        UT_ASSERT(wr2 == wr1);
        UT_ASSERT(!(wr1 != wr2));
        UT_ASSERT(!(wr2 != wr1));
        UT_ASSERT(wr1.Get());
        UT_ASSERT(wr2.Get());
        UT_ASSERT(wr1.IsOrphaned() == false);
        UT_ASSERT(wr2.IsOrphaned() == false);
        UT_ASSERT(wr1.IsValid() == true);
        UT_ASSERT(wr2.IsValid() == true);
        UT_ASSERT(wr1.IsEmpty() == false);
        UT_ASSERT(wr2.IsEmpty() == false);
        UT_ASSERT_NO_THROW(wr1.Clear());
        UT_ASSERT_NO_THROW(wr2.Clear());

        UT_ASSERT_NO_THROW(wr2 = p2);
        UT_ASSERT_NO_THROW(wr2 = wr1);
        UT_ASSERT_NO_THROW(wr1.Clear());
        UT_ASSERT_NO_THROW(wr2.Clear());
        UT_ASSERT_NO_THROW(wr2 = wr1);

        UT_ASSERT(wr1 == wr2);
        UT_ASSERT(wr2 == wr1);
        UT_ASSERT(!(wr1 != wr2));
        UT_ASSERT(!(wr2 != wr1));
        UT_ASSERT(wr1.Get() == NULL);
        UT_ASSERT(wr2.Get() == NULL);
        UT_ASSERT(wr1.IsOrphaned() == false);
        UT_ASSERT(wr2.IsOrphaned() == false);
        UT_ASSERT(wr1.IsValid() == false);
        UT_ASSERT(wr2.IsValid() == false);
        UT_ASSERT(wr1.IsEmpty() == true);
        UT_ASSERT(wr2.IsEmpty() == true);
        UT_ASSERT_NO_THROW(wr1.Clear());
        UT_ASSERT_NO_THROW(wr2.Clear());
    }
}

// Test assigning a WeakRef to itself.
void XWeakRefTest::TestAssignmentOfSelf()
{
    bool deleted = false;
    XRef<MultiBase> p = new MultiBase(deleted);
    UT_ASSERT(p.IsValid());
    {
        XWeakRef<MultiBase> wr(p);
        UT_ASSERT_NO_THROW(p = p);
        UT_ASSERT(p.GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
    }
}

// Test the -> operator.
void XWeakRefTest::TestPointerOperator()
{
    bool deleted = false;
    XRef<MultiDerived> p = new MultiDerived(deleted);
    UT_ASSERT(p.IsValid());
    XWeakRef<MultiDerived> wr = p;
    UT_ASSERT(p.GetReferenceCount() == 1);
    int value = 1;
    UT_ASSERT_NO_THROW(value = wr->Increment(value)); // the test
    UT_ASSERT(value == 2);
    UT_ASSERT(deleted == false);
}

// Test the * operator.
void XWeakRefTest::TestIndirectionOperator()
{
    bool deleted = false;
    XRef<MultiDerived> p = new MultiDerived(deleted);
    UT_ASSERT(p.IsValid());
    XWeakRef<MultiDerived> wr = p;
    UT_ASSERT(p.GetReferenceCount()  == 1);
    int value = 1;
    UT_ASSERT_NO_THROW(value = (*wr).Increment(value)); // the test
    UT_ASSERT(value == 2);
    UT_ASSERT(&(*wr) == p.Get()); // the test
    UT_ASSERT(deleted == false);
}

// Test the comparison operators
void XWeakRefTest::TestComparisons()
{
    bool deleted_1 = false;
    bool deleted_2 = false;
    XRef<MultiBase> p1 = new MultiBase(deleted_1);
    XRef<MultiBase> p2 = new MultiBase(deleted_2);
    UT_ASSERT(p1.IsValid());
    UT_ASSERT(p2.IsValid());
    {
        XWeakRef<MultiBase> wr1(p1);
        XWeakRef<MultiBase> wr2(p1);
        XWeakRef<MultiBase> wr3(p2);
        UT_ASSERT(  wr1 == p1 );  // raw pointer
        UT_ASSERT(  wr1 == wr2 );      // WeakRef
        UT_ASSERT(  wr1 == p1  );      // XRef
        UT_ASSERT(!(wr1 == p2)); // raw pointer
        UT_ASSERT(!(wr1 == wr3));      // WeakRef
        UT_ASSERT(!(wr1 == p2) );      // XRef
        UT_ASSERT(  wr1 != p2 );  // raw pointer
        UT_ASSERT(  wr1 != wr3 );      // WeakRef
        UT_ASSERT(  wr1 != p2  );      // XRef
        UT_ASSERT(!(wr1 != p1)); // raw pointer
        UT_ASSERT(!(wr1 != wr2));      // WeakRef
        UT_ASSERT(!(wr1 != p1) );      // XRef
        UT_ASSERT(p1.GetReferenceCount() == 1);
        UT_ASSERT(p2.GetReferenceCount() == 1);
        UT_ASSERT(deleted_1 == false);
        UT_ASSERT(deleted_2 == false);
    }
}

// Test IsEmpty() and IsValid()
void XWeakRefTest::TestStateMethods()
{
    bool deleted = false;
    XWeakRef<MultiBase> wr;
    {
        XRef<MultiBase> p = new MultiBase(deleted);
        UT_ASSERT(p.IsValid());
        UT_ASSERT(wr.IsEmpty());
        UT_ASSERT(wr.IsValid() == false);
        UT_ASSERT(wr.IsOrphaned() == false);
        UT_ASSERT_NO_THROW(wr = p);
        UT_ASSERT(wr.IsValid());
        UT_ASSERT(wr.IsValid() == true);
        UT_ASSERT(p.GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
        UT_ASSERT(wr.IsOrphaned() == false);
    }
    UT_ASSERT(wr.IsOrphaned() == true);
}

// Test the functionality when the referenced object goes away.
void XWeakRefTest::TestObjectDeletion()
{
    bool deleted = false;
    XRef<MultiDerived> p = new MultiDerived(deleted);
    XRef<MultiDerived> p10;
    UT_ASSERT(p.IsValid());
    XWeakRef<MultiDerived> wr = p;
    UT_ASSERT(p.GetReferenceCount()  == 1);

    // Verify the object is really there first.
    int value = 1;
    UT_ASSERT_NO_THROW(value = wr->Increment(value));
    UT_ASSERT(value == 2);

    // Clear all "hard" references to the object and verify XRef throws the correct value.
    p.Clear();
    UT_ASSERT(deleted == true);

    // Test -> operator
    XSDK_ASSERT_ORPHANED_THROW((value = wr->Increment(value)));

    // Now assert that the XRef is Valid but orphaned.
    UT_ASSERT(wr.IsValid());
    UT_ASSERT(wr.IsOrphaned());

    // Test * operator
    XSDK_ASSERT_ORPHANED_THROW((value = (*wr).Increment(value)));

    // Test the Get() method
    MultiDerived* pRaw = 0;
    XSDK_ASSERT_ORPHANED_THROW((pRaw = wr.Get()));

    // Test operator==
    XRef<MultiDerived> p2 = new MultiDerived(deleted);
    UT_ASSERT(p2.IsValid());
    XWeakRef<MultiDerived> wr2(p2);
    bool result = true;
    XSDK_ASSERT_ORPHANED_THROW((result = ((wr == wr2) ? true : false)));

    // Verify that we can still assign to an orphaned object.
    UT_ASSERT_NO_THROW(wr = wr2);
    UT_ASSERT(wr.IsValid());
    UT_ASSERT(wr.IsValid() == true);
    UT_ASSERT_NO_THROW(value = wr->Increment(value));

    // Now, just make sure everything can still go away cleanly.
    UT_ASSERT(deleted == false);
    p2.Clear();
    UT_ASSERT(deleted == true);
}

// Test Clear().
void XWeakRefTest::TestClear()
{
    bool deleted = false;
    XRef<MultiBase> p = new MultiBase(deleted);
    UT_ASSERT(p.IsValid());
    {
        XWeakRef<MultiBase> wr(p);
        UT_ASSERT(wr.IsValid());
        UT_ASSERT_NO_THROW(wr.Clear());
        UT_ASSERT(wr.IsEmpty());
        UT_ASSERT(p.GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
    }
}
