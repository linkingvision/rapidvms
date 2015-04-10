#include "XRefTest.h"
#include "XSDK/XWeakRef.h"

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XRefTest);

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
    virtual int Increment(int value)=0;
    virtual ~IInc(){}
};

class IDec
{
public:
    virtual int Decrement(int value)=0;
    virtual ~IDec(){}
};

class MultiBase : public IInc,
                  public IDec
{
public:
    MultiBase(bool& deleted) :
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

class MultiDerivedXBaseObject : public TestBase3,
                                public MultiBase,
                                public XBaseObject
{
public:
    MultiDerivedXBaseObject(bool& deleted) :
        TestBase3(),
        MultiBase(deleted),
        XBaseObject()
    {
    }

    virtual ~MultiDerivedXBaseObject(){}
};


void XRefTest::setup()
{
}

void XRefTest::teardown()
{
}

void XRefTest::TestDefaultConstructor()
{
    {
        XRef<MultiBase> a;
        UT_ASSERT(a.Get() == NULL);
        UT_ASSERT(a.IsEmpty());
        UT_ASSERT(!a.IsValid());
        UT_ASSERT(a.GetReferenceCount() == 0);
        UT_ASSERT_NO_THROW(a.Clear());
    }
}

// Test assigning a raw pointer to the XRef.
void XRefTest::TestPointerAssignment()
{
    bool deleted = false;

    // Test assigning via the constructor.
    {
        XRef<MultiDerived> ar(new MultiDerived(deleted));
        UT_ASSERT(deleted == false);
        UT_ASSERT(ar.GetReferenceCount() == 1);
    }
    UT_ASSERT(deleted == true);

    // Test assigning when a pointer has already been assigned.
    bool deleted_1 = false;
    bool deleted_2 = false;
    {
        XRef<MultiDerived> ar = new MultiDerived(deleted_1);
        UT_ASSERT(deleted_1 == false);
        ar = new MultiDerived(deleted_2);
        UT_ASSERT(deleted_1 == true);
        UT_ASSERT(deleted_2 == false);
        UT_ASSERT(ar.GetReferenceCount() == 1);
        UT_ASSERT(deleted_2 == false);
    }
    UT_ASSERT(deleted_2 == true);

    deleted_1 = false;
    deleted_2 = false;
    {
        MultiDerived * p1 = new MultiDerived(deleted_1);
        MultiDerived * p2 = new MultiDerived(deleted_2);
        UT_ASSERT(p1 != 0);
        UT_ASSERT(p2 != 0);
        XRef<MultiDerived> ar1;
        XRef<MultiDerived> ar2;
        UT_ASSERT(deleted_1 == false);
        UT_ASSERT(deleted_2 == false);
        UT_ASSERT_NO_THROW(ar1 = p1);
        UT_ASSERT_NO_THROW(ar2 = p2);
    }
    UT_ASSERT(deleted_2 == true);
    UT_ASSERT(deleted_2 == true);
}

// Test assigning one XRef to another.
void XRefTest::TestObjectAssignment()
{
    bool deleted = false;
    {
        XRef<MultiBase> ar1(new MultiBase(deleted));
        XRef<MultiBase> ar2 = ar1;
        UT_ASSERT(ar1.GetReferenceCount() == 2);
        UT_ASSERT(ar2.GetReferenceCount() == 2);

        ar1.Clear();
        UT_ASSERT(deleted == false);
        UT_ASSERT(ar2.GetReferenceCount() == 1);
    }
    UT_ASSERT(deleted == true);

    {
        XRef<MultiBase> ar1 = new MultiBase(deleted);
        ar1 = NULL;
        UT_ASSERT(ar1.GetReferenceCount() == 0);
        UT_ASSERT(!ar1.IsValid());
        UT_ASSERT(ar1.IsEmpty());
        UT_ASSERT_NO_THROW(ar1.Clear());
    }

    bool deleted_1 = false;
    bool deleted_2 = false;
    {
        XRef<MultiDerived> p1 = new MultiDerived(deleted_1);
        XRef<MultiDerived> p2 = new MultiDerived(deleted_2);
        UT_ASSERT(p1.IsValid());
        UT_ASSERT(p2.IsValid());
        UT_ASSERT(deleted_1 == false);
        UT_ASSERT(deleted_2 == false);
        UT_ASSERT(p1.GetReferenceCount() == 1);
        UT_ASSERT(p2.GetReferenceCount() == 1);
        UT_ASSERT_NO_THROW(p1 = p2);
        UT_ASSERT(p1.IsValid());
        UT_ASSERT(p2.IsValid());
        UT_ASSERT(deleted_1 == true);
        UT_ASSERT(deleted_2 == false);
        UT_ASSERT(p1.GetReferenceCount() == 2);
        UT_ASSERT(p2.GetReferenceCount() == 2);
        UT_ASSERT(p1->Increment(10) == 11);
        UT_ASSERT(p2->Decrement(10) == 9);

        UT_ASSERT_NO_THROW(*p1);
        UT_ASSERT_NO_THROW(*p2);
        UT_ASSERT(p1 == p2);
        UT_ASSERT(p2 == p1);
        UT_ASSERT(!(p1 != p2));
        UT_ASSERT(!(p2 != p1));
        UT_ASSERT(p1.Get());
        UT_ASSERT(p2.Get());
        UT_ASSERT(p1.IsValid() == true);
        UT_ASSERT(p2.IsValid() == true);
        UT_ASSERT(p1.IsEmpty() == false);
        UT_ASSERT(p2.IsEmpty() == false);
        UT_ASSERT_NO_THROW(p1.Clear());
        UT_ASSERT_NO_THROW(p2.Clear());

        UT_ASSERT_NO_THROW(p2 = p2);
        UT_ASSERT_NO_THROW(p2 = p1);
        UT_ASSERT_NO_THROW(p1.Clear());
        UT_ASSERT_NO_THROW(p2.Clear());
        UT_ASSERT_NO_THROW(p2 = p1);

        UT_ASSERT(p1 == p2);
        UT_ASSERT(p2 == p1);
        UT_ASSERT(!(p1 != p2));
        UT_ASSERT(!(p2 != p1));
        UT_ASSERT(p1.Get() == NULL);
        UT_ASSERT(p2.Get() == NULL);
        UT_ASSERT(p1.IsValid() == false);
        UT_ASSERT(p2.IsValid() == false);
        UT_ASSERT(p1.IsEmpty() == true);
        UT_ASSERT(p2.IsEmpty() == true);
        UT_ASSERT_NO_THROW(p1.Clear());
        UT_ASSERT_NO_THROW(p2.Clear());
    }
}

void XRefTest::TestRelatedTypeAssignment()
{
    bool deleted = false;
    {
        XRef<MultiBase> p1;
        {
            XRef<MultiDerived> p2 = new MultiDerived(deleted);

            UT_ASSERT_NO_THROW(p1 = p2);

            UT_ASSERT(p1.IsValid());
            UT_ASSERT(p2.IsValid());
            UT_ASSERT(deleted == false);
            UT_ASSERT(p1.GetReferenceCount() == 2);
            UT_ASSERT(p2.GetReferenceCount() == 2);
            UT_ASSERT(p1.IsValid());
            UT_ASSERT(p2.IsValid());
            UT_ASSERT(p1->Increment(10) == 11);
            UT_ASSERT(p2->Decrement(10) == 9);
        }

        UT_ASSERT(deleted == false);
        UT_ASSERT(p1.IsValid());
        UT_ASSERT(p1.GetReferenceCount() == 1);
        UT_ASSERT(p1.IsValid());
        UT_ASSERT(p1->Increment(10) == 11);
    }
    UT_ASSERT(deleted == true);
}

void XRefTest::TestWeakRefAssignment()
{
    bool deleted = false;
    {
        XRef<MultiDerived> p = new MultiDerived(deleted);
        XWeakRef<MultiDerived> ar1(p);
        UT_ASSERT(deleted == false);
        XRef<MultiDerived> ar2(ar1);
        UT_ASSERT(ar2.GetReferenceCount() == 2);
        UT_ASSERT(deleted == false);
    }
    UT_ASSERT(deleted == true);

    deleted = false;
    {
        XRef<MultiDerived> ar1(new MultiDerived(deleted));
        XWeakRef<MultiDerived> ar2(ar1);
        UT_ASSERT(ar1.GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
        UT_ASSERT(deleted == false);
    }
    UT_ASSERT(deleted == true);

    deleted = false;
    {
        XRef<MultiDerived> ar1(new MultiDerived(deleted));
        XWeakRef<MultiDerived> ar2 = ar1;
        UT_ASSERT(ar1.GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
        UT_ASSERT(deleted == false);
    }
    UT_ASSERT(deleted == true);

    deleted = false;
    {
        XRef<MultiDerived> ar1 = new MultiDerived(deleted);
        UT_ASSERT(ar1.GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
        XWeakRef<MultiDerived> ar2(ar1);
        UT_ASSERT(deleted == false);
        XRef<MultiDerived> ar3 = ar2;
        UT_ASSERT(ar3.GetReferenceCount() == 2);
    }
    UT_ASSERT(deleted == true);

    deleted = false;
    {
        XRef<MultiDerived> ar1 = new MultiDerived(deleted);
        UT_ASSERT(ar1.GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
        XWeakRef<MultiDerived> ar2(ar1);
        UT_ASSERT(deleted == false);
        XRef<MultiDerived> ar3 = ar2;
        UT_ASSERT(ar3.GetReferenceCount() == 2);
        XWeakRef<MultiDerived> ar4 = ar2;
        UT_ASSERT(ar3.GetReferenceCount() == 2);
    }
    UT_ASSERT(deleted == true);

    {
        XWeakRef<MultiDerived> wr;
        {
            XRef<MultiDerived> ar1(new MultiDerived(deleted));
            UT_ASSERT(ar1.IsValid());
            UT_ASSERT(ar1.GetReferenceCount() == 1);
            UT_ASSERT_NO_THROW(wr = ar1);
            UT_ASSERT(wr.IsValid());
            UT_ASSERT(wr.IsOrphaned() == false);
            UT_ASSERT(deleted == false);
        }
        UT_ASSERT(deleted == true);
        UT_ASSERT(wr.IsValid());
        UT_ASSERT(wr.IsOrphaned() == true);
        XRef<MultiDerived> ar1;
        UT_ASSERT_NO_THROW(ar1 = wr);
        UT_ASSERT(ar1.IsEmpty());
        UT_ASSERT(!ar1.Get());
        UT_ASSERT_NO_THROW(wr.Clear());
        UT_ASSERT(wr.IsEmpty());
        UT_ASSERT(wr.IsOrphaned() == false);
    }
}

// Test assigning an XRef to itself.
void XRefTest::TestAssignmentOfSelf()
{
    bool deleted = false;
    {
        MultiDerived* p = new MultiDerived(deleted);
        UT_ASSERT(p != 0);
        XRef<MultiDerived> ar(p);
        UT_ASSERT_NO_THROW(ar = ar);
        UT_ASSERT(ar.GetReferenceCount() == 1);
        UT_ASSERT(ar.Get());
        UT_ASSERT(ar.IsValid() == true);
        UT_ASSERT(ar.IsEmpty() == false);
    }
    UT_ASSERT(deleted == true);
}

// Test the comparison operators
void XRefTest::TestComparisons()
{
    bool deleted_1 = false;
    bool deleted_2 = false;
    {
        XRef<MultiDerived> ar1(new MultiDerived(deleted_1));
        XRef<MultiDerived> ar2(new MultiDerived(deleted_2));
        UT_ASSERT(!(ar1 == ar2));
        UT_ASSERT(  ar1 != ar2 );
        UT_ASSERT(ar1.GetReferenceCount() == 1);
        UT_ASSERT(ar2.GetReferenceCount() == 1);

        ar2 = ar1;
        UT_ASSERT(deleted_2 == true);
        UT_ASSERT(ar1.GetReferenceCount() == 2);
        UT_ASSERT(  ar1 == ar2 );
        UT_ASSERT(!(ar1 != ar2));
        UT_ASSERT(ar2.GetReferenceCount() == 2);
    }
    UT_ASSERT(deleted_1 == true);

    deleted_1 = false;

    {
        XRef<MultiDerived> ar1;
        XRef<MultiDerived> ar2;

        UT_ASSERT( ar1 == ar2 );
        UT_ASSERT( (ar1 != ar2) == false);

        MultiDerived* a = 0;

        ar1 = a;

        UT_ASSERT( ar1 == ar2 );
        UT_ASSERT( !(ar1 != ar2) );

        ar2 = new MultiDerived(deleted_1);

        UT_ASSERT( ar1 != ar2 );
        UT_ASSERT( !(ar1 == ar2) );

        UT_ASSERT_NO_THROW(ar1.Clear());
        UT_ASSERT_NO_THROW(ar2.Clear());

        UT_ASSERT( ar1 == ar2 );
        UT_ASSERT( !(ar1 != ar2) );
    }
}

// Test the -> operator.
void XRefTest::TestPointerOperator()
{
    bool deleted = false;
    {
        MultiBase* p = new MultiBase(deleted);
        UT_ASSERT(p != 0);
        XRef<MultiBase> ar1(p);
        UT_ASSERT(ar1.GetReferenceCount() == 1);
        UT_ASSERT(ar1->Increment(10) == 11);
        UT_ASSERT(deleted == false);
    }
    UT_ASSERT(deleted == true);
}


void XRefTest::TestCastOperatorNonInstrusive()
{
    bool destroyed;

    {
        XRef<MultiBase> a = new MultiBase(destroyed);
    }

    UT_ASSERT(destroyed == true);

    {
        XRef<MultiDerived> a = new MultiDerived(destroyed);
        {
            XRef<MultiBase> b = a;
        }
        UT_ASSERT(destroyed == false);
    }

    UT_ASSERT(destroyed == true);

    {
        XRef<MultiDerived> a = new MultiDerived(destroyed);
        {
            XRef<MultiBase> b = a;
            UT_ASSERT((void*)a.Get() != (void*)b.Get());
        }
        UT_ASSERT(destroyed == false);
    }

    {
        XRef<MultiBase> a = new MultiDerived(destroyed);
        UT_ASSERT(a->Increment(10) == 11);
        {
            XRef<MultiDerived> b = a;
            UT_ASSERT(a->Decrement(11) == 10);
            UT_ASSERT(a->Increment(10) == 11);
            UT_ASSERT((void*)a.Get() != (void*)b.Get());
        }
        UT_ASSERT(a->Increment(10) == 11);
        UT_ASSERT(destroyed == false);
    }

    UT_ASSERT(destroyed == true);
}

// Test IsEmpty() and IsValid()
void XRefTest::TestStateMethods()
{
    {
        bool deleted = false;
        MultiBase* p = new MultiBase(deleted);
        UT_ASSERT(p != 0);
        XRef<MultiBase> ar;
        UT_ASSERT(ar.IsEmpty());
        UT_ASSERT(!ar.IsValid());
        ar = p;
        UT_ASSERT(!ar.IsEmpty());
        UT_ASSERT(ar.IsValid());
    }

    {
        XRef<MultiBase> ar;
        UT_ASSERT(ar.IsEmpty());
        UT_ASSERT(ar.IsValid() == false);
        UT_ASSERT_NO_THROW(ar.Clear());
        UT_ASSERT(ar.IsEmpty());
        UT_ASSERT(ar.IsValid() == false);
    }
}

