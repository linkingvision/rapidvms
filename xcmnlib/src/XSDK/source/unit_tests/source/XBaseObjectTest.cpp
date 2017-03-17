
#include "XSDK/XIRef.h"
#include "XSDK/XVariant.h"
#include "XSDK/XUuid.h"
#include "XSDK/XBaseObject.h"
#include "XBaseObjectTest.h"
#include "DerivedObject.h"

using namespace XSDK;
using namespace XSDK;

//=============================================================================
// This macro registers the test suite for execution by the test applciation.
//=============================================================================
REGISTER_TEST_FIXTURE(XBaseObjectTest);

//=============================================================================
// setup
//=============================================================================
void XBaseObjectTest::setup()
{
}

//=============================================================================
// teardown
//=============================================================================
void XBaseObjectTest::teardown()
{
}

//=========================================================================
// TestConstruction
//=========================================================================
void XBaseObjectTest::TestConstruction()
{
    bool deleted = false;
    XIRef<DerivedObject> pObj = new (std::nothrow) DerivedObject(&deleted);
    UT_ASSERT_MESSAGE("Failed to allocate DerivedObject.", pObj.IsValid());
    UT_ASSERT_MESSAGE("The object was prematurely deleted.", !deleted);

    // Delete the object.
    UT_ASSERT_NO_THROW(pObj.Clear());
    UT_ASSERT_MESSAGE("The destructor of DerivedObject was NOT called.", deleted);
}

//=========================================================================
// TestDestruction
//=========================================================================
void XBaseObjectTest::TestDestruction()
{
    bool deleted = false;
    bool cleanedup = false;
    XIRef<DerivedObject> pObj = new (std::nothrow) DerivedObject(&deleted, &cleanedup);
    UT_ASSERT_MESSAGE("Failed to allocate DerivedObject.", pObj.IsValid());
    UT_ASSERT_MESSAGE("The object was prematurely deleted.", !deleted);
    UT_ASSERT_MESSAGE("The derived object's CleanUp method was called prematurely.", !cleanedup);

    // Delete the object.
    UT_ASSERT_NO_THROW(pObj.Clear());
    UT_ASSERT_MESSAGE("The destructor of DerivedObject was NOT called.", deleted);
    UT_ASSERT_MESSAGE("The CleanUp method of DerivedObject was NOT called.", cleanedup);
}

//=========================================================================
// TestSetGetObjectName
//=========================================================================
void XBaseObjectTest::TestSetGetObjectName()
{
    bool deleted = false;
    std::string name = "XBaseObjectTest::MyName";
    XIRef<DerivedObject> pObj = new (std::nothrow) DerivedObject(&deleted, name);
    UT_ASSERT_MESSAGE("Failed to allocate DerivedObject.", pObj.IsValid());
    UT_ASSERT_MESSAGE("The object was prematurely deleted.", !deleted);

    UT_ASSERT_MESSAGE("The base object name wasn't correctly set.", pObj->GetObjectName() == name);

    // Change the name and make sure it sticks.
    name = "XBaseObjectTest::MyOtherName";
    UT_ASSERT_NO_THROW(pObj->SetObjectName(name));
    UT_ASSERT_MESSAGE("The base object name wasn't correctly set.", pObj->GetObjectName() == name);

    // Delete the object.
    UT_ASSERT_NO_THROW(pObj.Clear());
    UT_ASSERT_MESSAGE("The destructor of DerivedObject was NOT called.", deleted);
}

//=========================================================================
// Tests AddRef and Release.
//=========================================================================
void XBaseObjectTest::TestReferenceCounting()
{
    bool deleted = false;
    DerivedObject* pObj = new (std::nothrow) DerivedObject(&deleted);
    UT_ASSERT_MESSAGE("Failed to allocate DerivedObject.", pObj != 0);

    // The reference count should be 0 at this point.
    UT_ASSERT_MESSAGE("The reference count is incorrect.", pObj->GetReferenceCount() == 0);
    UT_ASSERT_MESSAGE("The object was prematurely deleted.", !deleted);

    // Verify AddRef causes the reference count to increment.
    int refCount = -1;
    UT_ASSERT_NO_THROW(refCount = pObj->AddRef());
    UT_ASSERT_MESSAGE("The reference count is incorrect.", refCount == 1);
    UT_ASSERT_MESSAGE("The object was prematurely deleted.", !deleted);
    UT_ASSERT_NO_THROW(refCount = pObj->AddRef());
    UT_ASSERT_MESSAGE("The reference count is incorrect.", refCount == 2);
    UT_ASSERT_MESSAGE("The object was prematurely deleted.", !deleted);

    // Verify Release causes the reference count to decrement.
    UT_ASSERT_NO_THROW(refCount = pObj->Release());
    UT_ASSERT_MESSAGE("The reference count is incorrect.", refCount == 1);
    UT_ASSERT_MESSAGE("The object was prematurely deleted.", !deleted);

    // Verify the object goes away when the reference count hits 0.
    UT_ASSERT_NO_THROW(refCount = pObj->Release());
    UT_ASSERT_MESSAGE("The reference count is incorrect.", refCount == 0);
    UT_ASSERT_MESSAGE("The destructor of DerivedObject was NOT called.", deleted);
}

//=========================================================================
// TestDoubleDeleteMPF1195
//=========================================================================
void XBaseObjectTest::TestDoubleDeleteMPF1195()
{
    int deleteCount = 0;
    DerivedObject* pObj = new (std::nothrow) DerivedObject(&deleteCount);
    UT_ASSERT_MESSAGE("Failed to allocate DerivedObject.", pObj != 0);

    // The reference count should be 0 at this point.
    UT_ASSERT_MESSAGE("The reference count is incorrect.", pObj->GetReferenceCount() == 0);
    UT_ASSERT_MESSAGE("The object was prematurely deleted.", deleteCount == 0);

    int refCount = -1;
    UT_ASSERT_NO_THROW(refCount = pObj->AddRef());
    UT_ASSERT_MESSAGE("The reference count is incorrect.", refCount == 1);
    UT_ASSERT_MESSAGE("The object was prematurely deleted.", deleteCount == 0);

    // Do double delete.
    pObj->EnableDoubleDelete();
    UT_ASSERT_NO_THROW(refCount = pObj->Release());
    UT_ASSERT_MESSAGE("The reference count is incorrect.", refCount == 0);
    UT_ASSERT_MESSAGE("The object destructor was called too many times.", deleteCount == 1);
}

//=========================================================================
// Tests QueryInterface.
//=========================================================================
void XBaseObjectTest::TestQueryInterface()
{
    bool deleted = false;
    XIRef<DerivedObject> pObj = new (std::nothrow) DerivedObject(&deleted);
    UT_ASSERT_MESSAGE("Failed to allocate DerivedObject.", pObj.IsValid());

    // Query for IXUnknown.
    XIRef<IXUnknown> pIUnk;
    bool result = false;
    UT_ASSERT_NO_THROW(result = pObj->QueryInterface(IXUnknown::Uuid(), (void**)&pIUnk.Raw()));
    UT_ASSERT_MESSAGE("Failed to get the IXUnknown interface.", result == true);
    UT_ASSERT_MESSAGE("QueryInterface returned true, but the interface pointer is NULL.", pIUnk.IsValid());

    // Query for IBaseObjectTestInterface.
    result = false;
    XIRef<IBaseObjectTestInterface> pITest;
    UT_ASSERT_NO_THROW(result = pObj->QueryInterface(IBaseObjectTestInterface::Uuid(), (void**)&pITest.Raw()));
    UT_ASSERT_MESSAGE("Failed to get the IBaseObjectTestInterface interface.", result == true);
    UT_ASSERT_MESSAGE("QueryInterface returned true, but the interface pointer is NULL.", pITest.IsValid());

    // Call a method on the test interface.
    int value = 1;
    UT_ASSERT_NO_THROW(value = pITest->Increment(value));

    // Verify that the object gets deleted at the appropriate time.
    UT_ASSERT_MESSAGE("The object was prematurely deleted.", !deleted);
    pObj.Clear();
    UT_ASSERT_MESSAGE("The object was prematurely deleted.", !deleted);
    pIUnk.Clear();
    UT_ASSERT_MESSAGE("The object was prematurely deleted.", !deleted);
    pITest.Clear();
    UT_ASSERT_MESSAGE("The destructor of DerivedObject was NOT called.", deleted);
}

//=========================================================================
// Tests GetClassId.
//=========================================================================
void XBaseObjectTest::TestGetClassId()
{
    bool deleted = false;
    XIRef<DerivedObject> pObj = new (std::nothrow) DerivedObject(&deleted);
    UT_ASSERT_MESSAGE("Failed to allocate DerivedObject.", pObj.IsValid());

    XUuid classId;
    UT_ASSERT_NO_THROW(classId = pObj->GetClassId());
    UT_ASSERT_MESSAGE("The class Id returned is incorrect.", classId == XSDK::kClsIdDerivedObject);
}
