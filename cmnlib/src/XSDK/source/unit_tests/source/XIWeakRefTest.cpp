#include "XIWeakRefTest.h"
#include <string>
#include <iostream>
#include "XSDK/XIRef.h"
#include "XSDK/XIWeakRef.h"
#include "DerivedObject.h"

using namespace std;
using namespace XSDK;

// Helper macro for detecting the correct exception was thrown
#define XSDK_ASSERT_ORPHANED_THROW(exp) \
    try {   \
        (exp);  \
        UT_ASSERT_MESSAGE("XIWeakRef didn't throw even though it's been orphaned.", false);    \
    }   \
        catch(XException&)    \
    {} \
        catch(...) \
    { \
        UT_ASSERT_MESSAGE("XIWeakRef threw the wrong exception type when accessing an orphaned object.", false);   \
    }

REGISTER_TEST_FIXTURE(XIWeakRefTest);

void XIWeakRefTest::setup()
{
}

void XIWeakRefTest::teardown()
{
}

// Test assigning a raw pointer to the XIWeakRef.
void XIWeakRefTest::TestPointerAssignment()
{
    bool deleted = false;
    XIRef<DerivedObject> p = new (std::nothrow) DerivedObject(&deleted);
    UT_ASSERT(p.IsValid());

    // Test assigning via the constructor (raw pointer).
    {
        UT_ASSERT(p->GetReferenceCount() == 1);
        XIWeakRef<DerivedObject> wr(p.Raw());
        UT_ASSERT(wr.IsValid());
        UT_ASSERT(p->GetReferenceCount() == 1);
    }

    // Test assigning via the assignment operator.
    UT_ASSERT(deleted == false);
    {
        UT_ASSERT(p->GetReferenceCount() == 1);
        XIWeakRef<DerivedObject> wr = p.Raw();
        UT_ASSERT(wr.IsValid());
        UT_ASSERT(p->GetReferenceCount() == 1);
    }
    p.Clear();

    // Test assigning when a pointer has already been assigned.
    bool deleted_1 = false;
    bool deleted_2 = false;
    XIRef<DerivedObject> p1 = new (std::nothrow) DerivedObject(&deleted_1);
    XIRef<DerivedObject> p2 = new (std::nothrow) DerivedObject(&deleted_2);
    UT_ASSERT(p1.IsValid());
    UT_ASSERT(p2.IsValid());
    {
        UT_ASSERT(p1->GetReferenceCount() == 1);
        XIWeakRef<DerivedObject> wr = p1.Raw();
        UT_ASSERT(wr.IsValid());
        UT_ASSERT(deleted_1 == false);
        UT_ASSERT(p1->GetReferenceCount() == 1);
        UT_ASSERT_NO_THROW(wr = p2.Raw());
        UT_ASSERT(wr.IsValid());
        UT_ASSERT(deleted_1 == false);
        UT_ASSERT(deleted_2 == false);
        UT_ASSERT(p1->GetReferenceCount() == 1);
        UT_ASSERT(p2->GetReferenceCount() == 1);
    }
}

// Test assigning one XIWeakRef to another.
void XIWeakRefTest::TestWeakRefAssignment()
{
    bool deleted = false;
    XIRef<DerivedObject> p = new (std::nothrow) DerivedObject(&deleted);
    UT_ASSERT(p.IsValid());

    // Test assigning via the constructor.
    {
        UT_ASSERT(p->GetReferenceCount() == 1);
        XIWeakRef<DerivedObject> wr1(p.Raw());
        UT_ASSERT(wr1.IsValid());
        UT_ASSERT_NO_THROW(XIWeakRef<DerivedObject> wr2(wr1));
        UT_ASSERT(wr1.IsValid());
        UT_ASSERT(p->GetReferenceCount() == 1);
    }

    // Test assigning via the assignment operator.
    UT_ASSERT(deleted == false);
    {
        UT_ASSERT(p->GetReferenceCount() == 1);
        XIWeakRef<DerivedObject> wr1(p.Raw());
        UT_ASSERT(wr1.IsValid());
        XIWeakRef<DerivedObject> wr2;
        UT_ASSERT_NO_THROW(wr2 = wr1);
        UT_ASSERT(wr1.IsValid());
        UT_ASSERT(wr2.IsValid());
        UT_ASSERT(p->GetReferenceCount() == 1);
    }
    p.Clear();

    // Test assigning when a pointer has already been assigned.
    bool deleted_1 = false;
    bool deleted_2 = false;
    XIRef<DerivedObject> p1 = new (std::nothrow) DerivedObject(&deleted_1);
    XIRef<DerivedObject> p2 = new (std::nothrow) DerivedObject(&deleted_2);
    UT_ASSERT(p1.IsValid());
    UT_ASSERT(p2.IsValid());
    {
        UT_ASSERT(p1->GetReferenceCount() == 1);
        XIWeakRef<DerivedObject> wr1 = p1.Raw();
        XIWeakRef<DerivedObject> wr2 = p2.Raw();
        UT_ASSERT(wr1.IsValid());
        UT_ASSERT(wr2.IsValid());
        UT_ASSERT(deleted_1 == false);
        UT_ASSERT(deleted_2 == false);
        UT_ASSERT(p1->GetReferenceCount() == 1);
        UT_ASSERT(p2->GetReferenceCount() == 1);
        UT_ASSERT_NO_THROW(wr1 = wr2);
        UT_ASSERT(wr1.IsValid());
        UT_ASSERT(wr2.IsValid());
        UT_ASSERT(deleted_1 == false);
        UT_ASSERT(deleted_2 == false);
        UT_ASSERT(p1->GetReferenceCount() == 1);
        UT_ASSERT(p2->GetReferenceCount() == 1);
    }
}

// Test assigning an AutoRef to a WeakRef.
void XIWeakRefTest::TestAutoRefAssignment()
{
    bool deleted = false;
    XIRef<DerivedObject> p = new (std::nothrow) DerivedObject(&deleted);
    UT_ASSERT(p.IsValid());

    // Test assigning via the constructor (raw pointer).
    {
        UT_ASSERT(p->GetReferenceCount() == 1);
        UT_ASSERT_NO_THROW(XIWeakRef<DerivedObject> wr(p));
        UT_ASSERT(p->GetReferenceCount() == 1);
    }

    // Test assigning via the assignment operator.
    UT_ASSERT(deleted == false);
    {
        UT_ASSERT(p->GetReferenceCount() == 1);
        UT_ASSERT_NO_THROW(XIWeakRef<DerivedObject> wr = p);
        UT_ASSERT(p->GetReferenceCount() == 1);
    }
    p.Clear();

    // Test assigning when a pointer has already been assigned.
    bool deleted_1 = false;
    bool deleted_2 = false;
    XIRef<DerivedObject> p1 = new (std::nothrow) DerivedObject(&deleted_1);
    XIRef<DerivedObject> p2 = new (std::nothrow) DerivedObject(&deleted_2);
    UT_ASSERT(p1.IsValid());
    UT_ASSERT(p2.IsValid());
    {
        UT_ASSERT(p1->GetReferenceCount() == 1);
        XIWeakRef<DerivedObject> wr = p1;
        UT_ASSERT(wr.IsValid());
        UT_ASSERT(deleted_1 == false);
        UT_ASSERT(p1->GetReferenceCount() == 1);
        UT_ASSERT_NO_THROW(wr = p2);
        UT_ASSERT(wr.IsValid());
        UT_ASSERT(deleted_1 == false);
        UT_ASSERT(deleted_2 == false);
        UT_ASSERT(p1->GetReferenceCount() == 1);
        UT_ASSERT(p2->GetReferenceCount() == 1);
    }
}

// Test assigning a WeakRef to itself.
void XIWeakRefTest::TestAssignmentOfSelf()
{
    bool deleted = false;
    XIRef<DerivedObject> p = new (std::nothrow) DerivedObject(&deleted);
    UT_ASSERT(p.IsValid());
    {
        XIWeakRef<DerivedObject> wr(p);
        UT_ASSERT_NO_THROW(p = p);
        UT_ASSERT(p->GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
    }
}

// Test IsEmpty() and IsValid()
void XIWeakRefTest::TestStateMethods()
{
    bool deleted = false;
    XIRef<DerivedObject> p = new (std::nothrow) DerivedObject(&deleted);
    UT_ASSERT(p.IsValid());
    XIWeakRef<DerivedObject> wr;
    UT_ASSERT(wr.IsEmpty());
    UT_ASSERT(wr.IsValid() == false);
    UT_ASSERT(wr.IsOrphaned() == false);
    UT_ASSERT_NO_THROW(wr = p);
    UT_ASSERT(wr.IsValid());
    UT_ASSERT(wr.IsValid() == true);
    UT_ASSERT(wr.IsOrphaned() == false);
    UT_ASSERT(p->GetReferenceCount() == 1);
    UT_ASSERT(deleted == false);
}

// Test Clear().
void XIWeakRefTest::TestClear()
{
    bool deleted = false;
    XIRef<DerivedObject> p = new (std::nothrow) DerivedObject(&deleted);
    UT_ASSERT(p.IsValid());
    {
        XIWeakRef<DerivedObject> wr(p);
        UT_ASSERT(wr.IsValid());
        UT_ASSERT_NO_THROW(wr.Clear());
        UT_ASSERT(wr.IsEmpty());
        UT_ASSERT(p->GetReferenceCount() == 1);
        UT_ASSERT(deleted == false);
    }
}

// Test the evil Raw() method
void XIWeakRefTest::TestEvilMethod()
{
    bool deleted = false;
    XIRef<DerivedObject> p = new (std::nothrow) DerivedObject(&deleted);
    UT_ASSERT(p.IsValid());
    XIWeakRef<DerivedObject> wr = p;
    UT_ASSERT(wr.Raw() == p.Raw());

    // test both versions
    DerivedObject* pPtr = 0;
    const DerivedObject* pcPtr = 0;
    UT_ASSERT_NO_THROW(pPtr  = wr.Raw());
    UT_ASSERT_NO_THROW(pcPtr = wr.Raw());
    UT_ASSERT(pPtr  == p.Raw());
    UT_ASSERT(pcPtr == p.Raw());
}

// Test the comparison operators
void XIWeakRefTest::TestComparisons()
{
    bool deleted_1 = false;
    bool deleted_2 = false;
    XIRef<DerivedObject> p1 = new (std::nothrow) DerivedObject(&deleted_1);
    XIRef<DerivedObject> p2 = new (std::nothrow) DerivedObject(&deleted_2);
    UT_ASSERT(p1.IsValid());
    UT_ASSERT(p2.IsValid());
    {
        XIWeakRef<DerivedObject> wr1(p1);
        XIWeakRef<DerivedObject> wr2(p1);
        XIWeakRef<DerivedObject> wr3(p2);
        UT_ASSERT(  wr1 == p1.Raw());  // raw pointer
        UT_ASSERT(  wr1 == wr2 );      // WeakRef
        UT_ASSERT(  wr1 == p1  );      // AutoRef
        UT_ASSERT(!(wr1 == p2.Raw())); // raw pointer
        UT_ASSERT(!(wr1 == wr3));      // WeakRef
        UT_ASSERT(!(wr1 == p2) );      // AutoRef
        UT_ASSERT(  wr1 != p2.Raw());  // raw pointer
        UT_ASSERT(  wr1 != wr3 );      // WeakRef
        UT_ASSERT(  wr1 != p2  );      // AutoRef
        UT_ASSERT(!(wr1 != p1.Raw())); // raw pointer
        UT_ASSERT(!(wr1 != wr2));      // WeakRef
        UT_ASSERT(!(wr1 != p1) );      // AutoRef
        UT_ASSERT(p1->GetReferenceCount() == 1);
        UT_ASSERT(p2->GetReferenceCount() == 1);
        UT_ASSERT(deleted_1 == false);
        UT_ASSERT(deleted_2 == false);
    }
}

// Test the -> operator.
void XIWeakRefTest::TestPointerOperator()
{
    bool deleted = false;
    XIRef<DerivedObject> p = new (std::nothrow) DerivedObject(&deleted);
    UT_ASSERT(p.IsValid());
    XIWeakRef<DerivedObject> wr = p;
    UT_ASSERT(p->GetReferenceCount()  == 1);
    int value = 1;
    UT_ASSERT_NO_THROW(value = wr->Increment(value)); // the test
    UT_ASSERT(value == 2);
    UT_ASSERT(deleted == false);
}

// Test the * operator.
void XIWeakRefTest::TestIndirectionOperator()
{
    bool deleted = false;
    XIRef<DerivedObject> p = new (std::nothrow) DerivedObject(&deleted);
    UT_ASSERT(p.IsValid());
    XIWeakRef<DerivedObject> wr = p;
    UT_ASSERT(p->GetReferenceCount()  == 1);
    int value = 1;
    UT_ASSERT_NO_THROW(value = (*wr).Increment(value)); // the test
    UT_ASSERT(value == 2);
    UT_ASSERT(&(*wr) == p.Raw()); // the test
    UT_ASSERT(deleted == false);
}

// Test the functionality when the referenced object goes away.
void XIWeakRefTest::TestObjectDeletion()
{
    bool deleted = false;
    XIRef<DerivedObject> p = new (std::nothrow) DerivedObject(&deleted);
    UT_ASSERT(p.IsValid());
    XIWeakRef<DerivedObject> wr = p;
    UT_ASSERT(p->GetReferenceCount()  == 1);

    // Verify the object is really there first.
    int value = 1;
    UT_ASSERT_NO_THROW(value = wr->Increment(value));
    UT_ASSERT(value == 2);

    // Clear all "hard" references to the object and verify XIWeakRef throws the correct value.
    p.Clear();
    UT_ASSERT(deleted == true);

    // Test -> operator
    XSDK_ASSERT_ORPHANED_THROW((value = wr->Increment(value)));

    // Now assert that the XIWeakRef is Valid but orphaned.
    UT_ASSERT(wr.IsValid());
    UT_ASSERT(wr.IsValid());
    UT_ASSERT(wr.IsOrphaned());

    // Test * operator
    XSDK_ASSERT_ORPHANED_THROW((value = (*wr).Increment(value)));

    // Test the Raw() method
    DerivedObject* pRaw = 0;
    XSDK_ASSERT_ORPHANED_THROW((pRaw = wr.Raw()));

    // Test operator==
    XIRef<DerivedObject> p2 = new (std::nothrow) DerivedObject(&deleted);
    UT_ASSERT(p2.IsValid());
    XIWeakRef<DerivedObject> wr2(p2);
    bool result = true;
    XSDK_ASSERT_ORPHANED_THROW((result = ((wr == wr2) ? true : false)));

    // Verify that we can still assign to an orphaned object.
    UT_ASSERT_NO_THROW(wr = wr2);
    UT_ASSERT(wr.IsValid());
    UT_ASSERT(wr.IsValid() == true);
    UT_ASSERT(wr.IsOrphaned() == false);
    UT_ASSERT_NO_THROW(value = wr->Increment(value));

    // Now, just make sure everything can still go away cleanly.
    UT_ASSERT(deleted == false);
    p2.Clear();
    UT_ASSERT(deleted == true);
}
