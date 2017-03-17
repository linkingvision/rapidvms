#include "XIRefTest.h"
#include <string>
#include <iostream>
#include "TestRefdObject.h"
#include "DerivedObject.h"
#include "XSDK/XIRef.h"
#include "XSDK/XIWeakRef.h"

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XIRefTest);


void XIRefTest::setup()
{
}

void XIRefTest::teardown()
{
}

// This method makes sure the TestRefdObject is working correctly.
void XIRefTest::TestAddRefRelease()
{
    bool deleted = false;
    TestRefdObject* p = new (std::nothrow) TestRefdObject(&deleted);
    UT_ASSERT(p != 0);
    UT_ASSERT(p->AddRef() == 1);
    UT_ASSERT(deleted == false);
    UT_ASSERT(p->AddRef() == 2);
    UT_ASSERT(deleted == false);
    UT_ASSERT(p->Release() == 1);
    UT_ASSERT(p->GetRefCount() == 1);
    UT_ASSERT(deleted == false);
    UT_ASSERT(p->Release() == 0);
    UT_ASSERT(deleted == true);

    // Test with no 'deleted' argument to constructor
    p = new (std::nothrow) TestRefdObject(0);
    UT_ASSERT(p != 0);
    UT_ASSERT(p->AddRef() == 1);
    UT_ASSERT(p->Release() == 0);
}

// Test assigning a raw pointer to the AutoRef.
void XIRefTest::TestPointerAssignment()
{
    bool deleted = false;

    // Test assigning via the constructor.
    {
        TestRefdObject* p = new (std::nothrow) TestRefdObject(&deleted);
        UT_ASSERT(p != 0);
        XIRef<TestRefdObject> ar(p);
        UT_ASSERT(deleted == false);
        UT_ASSERT(p->GetRefCount() == 1);
    }
    UT_ASSERT(deleted == true);

    // Test assigning via the assignment operator.
    deleted = false;
    {
        TestRefdObject* p = new (std::nothrow) TestRefdObject(&deleted);
        UT_ASSERT(p != 0);
        XIRef<TestRefdObject> ar = p;
        UT_ASSERT(deleted == false);
        UT_ASSERT(p->GetRefCount() == 1);
    }
    UT_ASSERT(deleted == true);

    // Test assigning when a pointer has already been assigned.
    bool deleted_1 = false;
    bool deleted_2 = false;
    {
        TestRefdObject* p1 = new (std::nothrow) TestRefdObject(&deleted_1);
        TestRefdObject* p2 = new (std::nothrow) TestRefdObject(&deleted_2);
        UT_ASSERT(p1 != 0);
        UT_ASSERT(p2 != 0);
        XIRef<TestRefdObject> ar = p1;
        UT_ASSERT(deleted_1 == false);
        ar = p2;
        UT_ASSERT(deleted_1 == true);
        UT_ASSERT(deleted_2 == false);
        UT_ASSERT(p2->GetRefCount() == 1);
        UT_ASSERT(deleted_2 == false);
    }
    UT_ASSERT(deleted_2 == true);
}


// Test assigning one AutoRef to another.
void XIRefTest::TestObjectAssignment()
{
    bool deleted = false;
    {
        TestRefdObject* p = new (std::nothrow) TestRefdObject(&deleted);
        UT_ASSERT(p != 0);
        XIRef<TestRefdObject> ar1(p);
        XIRef<TestRefdObject> ar2 = ar1;
        UT_ASSERT(p->GetRefCount() == 2);

        ar1.Clear();
        UT_ASSERT(deleted == false);
        UT_ASSERT(p->GetRefCount() == 1);
    }
    UT_ASSERT(deleted == true);
}


// Test assigning an AutoRef to itself.
void XIRefTest::TestAssignmentOfSelf()
{
    bool deleted = false;
    {
        TestRefdObject* p = new (std::nothrow) TestRefdObject(&deleted);
        UT_ASSERT(p != 0);
        XIRef<TestRefdObject> ar(p);
        ar = ar;
        UT_ASSERT(p->GetRefCount() == 1);
    }
    UT_ASSERT(deleted == true);
}

// Test IsEmpty() and IsValid()
void XIRefTest::TestStateMethods()
{
    bool deleted = false;
    TestRefdObject* p = new (std::nothrow) TestRefdObject(&deleted);
    UT_ASSERT(p != 0);
    XIRef<TestRefdObject> ar;
    UT_ASSERT(ar.IsEmpty());
    UT_ASSERT(!ar.IsValid());
    ar = p;
    UT_ASSERT(!ar.IsEmpty());
    UT_ASSERT(ar.IsValid());
}

// Test the evil Raw() method
void XIRefTest::TestEvilMethod()
{
    bool deleted_1 = false;
    bool deleted_2 = false;
    {
        TestRefdObject* p1 = new (std::nothrow) TestRefdObject(&deleted_1);
        TestRefdObject* p2 = new (std::nothrow) TestRefdObject(&deleted_2);
        UT_ASSERT(p1 != 0);
        UT_ASSERT(p2 != 0);
        XIRef<TestRefdObject> ar(p1);
        UT_ASSERT(p1 == ar.Raw());
        UT_ASSERT(p1->GetRefCount() == 1);

        // Test the ability to overwrite the raw pointer without the AutoRef's knowledge.
        ar.Raw() = p2;
        UT_ASSERT(deleted_1 == false); // The AutoRef didn't know to release p1
        UT_ASSERT(p1->GetRefCount() == 1);
        p1->Release();
        UT_ASSERT(p2->AddRef() == 1);  // The AutoRef didn't know to addref p2

        UT_ASSERT(deleted_1 == true);
        UT_ASSERT(deleted_2 == false);
    }
    UT_ASSERT(deleted_1 == true);
    UT_ASSERT(deleted_2 == true);
}

// Test the GetCounter method
void XIRefTest::TestGetCounterMethod()
{
    bool deleted_1 = false;
    {
        TestRefdObject* p1 = new (std::nothrow) TestRefdObject(&deleted_1);
        UT_ASSERT(p1 != 0);
        XIRef<TestRefdObject> ar(p1);
        UT_ASSERT(p1 == ar.GetCounter());
        UT_ASSERT(p1->GetRefCount() == 1);
    }
    UT_ASSERT(deleted_1 == true);
}


// Test the comparison operators
void XIRefTest::TestComparisons()
{
    bool deleted_1 = false;
    bool deleted_2 = false;
    {
        TestRefdObject* p1 = new (std::nothrow) TestRefdObject(&deleted_1);
        TestRefdObject* p2 = new (std::nothrow) TestRefdObject(&deleted_2);
        UT_ASSERT(p1 != 0);
        UT_ASSERT(p2 != 0);
        XIRef<TestRefdObject> ar1(p1);
        XIRef<TestRefdObject> ar2(p2);
        UT_ASSERT(  ar1 == p1  );
        UT_ASSERT(!(ar1 == p2) );
        UT_ASSERT(!(ar1 == ar2));
        UT_ASSERT(  ar1 != ar2 );
        UT_ASSERT(p1->GetRefCount() == 1);
        UT_ASSERT(p2->GetRefCount() == 1);

        ar2 = p1;
        UT_ASSERT(deleted_2 == true);
        UT_ASSERT(p1->GetRefCount() == 2);
        UT_ASSERT(  ar2 == p1  );
        UT_ASSERT(!(ar2 == p2) );
        UT_ASSERT(  ar1 == ar2 );
        UT_ASSERT(!(ar1 != ar2));
        UT_ASSERT(p1->GetRefCount() == 2);
    }
    UT_ASSERT(deleted_1 == true);
}

// Test the -> operator.
void XIRefTest::TestPointerOperator()
{
    bool deleted = false;
    {
        TestRefdObject* p = new (std::nothrow) TestRefdObject(&deleted);
        UT_ASSERT(p != 0);
        XIRef<TestRefdObject> ar1(p);
        UT_ASSERT(p->GetRefCount()   == 1);
        UT_ASSERT(ar1->GetRefCount() == 1);
        UT_ASSERT(deleted == false);
    }
    UT_ASSERT(deleted == true);
}

void XIRefTest::TestWeakRefAssignment()
{
    bool deleted = false;
    {
        DerivedObject* p = new (std::nothrow) DerivedObject(&deleted);
        UT_ASSERT(p != 0);
        XIWeakRef<DerivedObject> ar1(p);
        UT_ASSERT(p->GetReferenceCount()   == 0);
        UT_ASSERT(ar1->GetReferenceCount() == 0);
        UT_ASSERT(deleted == false);
        XIRef<DerivedObject> ar2(ar1);
        UT_ASSERT(p->GetReferenceCount()   == 1);
        UT_ASSERT(ar2->GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
    }
    UT_ASSERT(deleted == true);

    deleted = false;
    {
        XIWeakRef<DerivedObject> ar1(new(std::nothrow)DerivedObject(&deleted));
        UT_ASSERT(ar1->GetReferenceCount() == 0);
        UT_ASSERT(deleted == false);
        XIRef<DerivedObject> ar2(ar1);
        UT_ASSERT(ar2->GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
    }
    UT_ASSERT(deleted == true);

    deleted = false;
    {
        XIWeakRef<DerivedObject> ar1 = new(std::nothrow)DerivedObject(&deleted);
        UT_ASSERT(ar1->GetReferenceCount() == 0);
        UT_ASSERT(deleted == false);
        XIRef<DerivedObject> ar2(ar1);
        UT_ASSERT(ar2->GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
    }
    UT_ASSERT(deleted == true);

    deleted = false;
    {
        XIRef<DerivedObject> ar1 = new(std::nothrow)DerivedObject(&deleted);
        UT_ASSERT(ar1->GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
        XIWeakRef<DerivedObject> ar2(ar1);
        UT_ASSERT(ar2->GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
        XIRef<DerivedObject> ar3 = ar2;
        UT_ASSERT(ar3->GetReferenceCount() == 2);
    }
    UT_ASSERT(deleted == true);

    deleted = false;
    {
        XIRef<DerivedObject> ar1 = new(std::nothrow)DerivedObject(&deleted);
        UT_ASSERT(ar1->GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
        XIWeakRef<DerivedObject> ar2(ar1);
        UT_ASSERT(ar2->GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
        XIRef<DerivedObject> ar3 = ar2;
        UT_ASSERT(ar3->GetReferenceCount() == 2);
        XIWeakRef<DerivedObject> ar4 = ar2;
        UT_ASSERT(ar3->GetReferenceCount() == 2);
    }
    UT_ASSERT(deleted == true);
}
