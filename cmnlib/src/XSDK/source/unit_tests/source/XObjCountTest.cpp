
#include "XObjCountTest.h"

#include "XSDK/XBaseObject.h"
#include "XSDK/XIRef.h"
#include "XSDK/XObjCount.h"
#include "XSDK/XRef.h"

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XObjCountTest);

void XObjCountTest::TestCountRefs_AddAndRemove()
{
#ifdef COUNT_REFS

    {
        map<string, size_t> before = XObjCount::GetObjCount();
        UT_ASSERT(before.find("xyzzy") == before.end());
    }

    XObjCount::AddObj("xyzzy");
    XObjCount::AddObj("xyzzy");
    XObjCount::AddObj("xyzzy");

    {
        map<string, size_t> mid = XObjCount::GetObjCount();
        map<string, size_t>::iterator found = mid.find("xyzzy");
        UT_ASSERT(found != mid.end());
        UT_ASSERT(found->second == 3);
    }

    XObjCount::RemoveObj("xyzzy");
    XObjCount::RemoveObj("xyzzy");
    XObjCount::RemoveObj("xyzzy");

    map<string, size_t> after = XObjCount::GetObjCount();
    UT_ASSERT(after.find("xyzzy") == after.end());
#else
    printf("XObjCountTest::TestCountRefs_AddAndRemove() DISABLED\n");
    fflush(stdout);
#endif
}


struct XYZZY
{
};

void XObjCountTest::TestCountRefs_XRef()
{
#ifdef COUNT_REFS

    XYZZY x;
    string xName = typeid(x).name();

    {
        map<string, size_t> before = XObjCount::GetObjCount();
        UT_ASSERT(before.find(xName) == before.end());
    }

    {
        XRef<XYZZY> a = new XYZZY;
        XRef<XYZZY> b = new XYZZY;
        XRef<XYZZY> c = new XYZZY;
        XRef<XYZZY> d = new XYZZY;

        {
            map<string, size_t> mid = XObjCount::GetObjCount();
            map<string, size_t>::iterator found = mid.find(xName);
            UT_ASSERT(found != mid.end());
            UT_ASSERT(found->second == 4);
        }
    }

    map<string, size_t> after = XObjCount::GetObjCount();
    UT_ASSERT(after.find(xName) == after.end());
#else
    printf("XObjCountTest::TestCountRefs_XRef() DISABLED\n");
    fflush(stdout);
#endif
}


struct Balderdash : public XBaseObject
{
};

void XObjCountTest::TestCountRefs_XBaseObject()
{
#ifdef COUNT_REFS

    Balderdash b;
    string bName = typeid(b).name();

    {
        map<string, size_t> before = XObjCount::GetObjCount();
        UT_ASSERT(before.find(bName) == before.end());
    }

    {
        XIRef<Balderdash> a = new Balderdash;
        XIRef<Balderdash> b = new Balderdash;
        XIRef<Balderdash> c = new Balderdash;
        XIRef<Balderdash> d = new Balderdash;

        {
            map<string, size_t> mid = XObjCount::GetObjCount();
            map<string, size_t>::iterator found = mid.find(bName);
            UT_ASSERT(found != mid.end());
            UT_ASSERT(found->second == 4);
        }
    }

    map<string, size_t> after = XObjCount::GetObjCount();
    UT_ASSERT(after.find(bName) == after.end());
#else
    printf("XObjCountTest::TestCountRefs_XBaseObject() DISABLED\n");
    fflush(stdout);
#endif
}


void XObjCountTest::TestTrackStack_AddAndRemove()
{
#ifdef TRACK_STACK

    XObjCount::AddStack((void*)12);
    XObjCount::AddStack((void*)13);
    XObjCount::AddStack((void*)14);

    {
        map<const void*, string> mid = XObjCount::GetStacks();
        UT_ASSERT(mid.find((void*)12) != mid.end());
        UT_ASSERT(mid.find((void*)13) != mid.end());
        UT_ASSERT(mid.find((void*)14) != mid.end());
    }

    XObjCount::RemoveStack((void*)12);
    XObjCount::RemoveStack((void*)13);
    XObjCount::RemoveStack((void*)14);

    map<const void*, string> after = XObjCount::GetStacks();
    UT_ASSERT(after.find((void*)12) == after.end());
    UT_ASSERT(after.find((void*)13) == after.end());
    UT_ASSERT(after.find((void*)14) == after.end());
#else
    printf("XObjCountTest::TestTrackStack_AddAndRemove() DISABLED\n");
    fflush(stdout);
#endif
}


void XObjCountTest::TestTrackStack_XRef()
{
#ifdef TRACK_STACK
    const void* ptrA = 0;
    const void* ptrB = 0;
    const void* ptrC = 0;
    const void* ptrD = 0;

    {
        XRef<XYZZY> a = new XYZZY;
        XRef<XYZZY> b = new XYZZY;
        XRef<XYZZY> c = new XYZZY;
        XRef<XYZZY> d = new XYZZY;
        ptrA = a.Get();
        ptrB = b.Get();
        ptrC = c.Get();
        ptrD = d.Get();

        {
            map<const void*, string> mid = XObjCount::GetStacks();
            UT_ASSERT(mid.find(ptrA) != mid.end());
            UT_ASSERT(mid.find(ptrB) != mid.end());
            UT_ASSERT(mid.find(ptrC) != mid.end());
            UT_ASSERT(mid.find(ptrD) != mid.end());
        }
    }

    map<const void*, string> after = XObjCount::GetStacks();
    UT_ASSERT(after.find(ptrA) == after.end());
    UT_ASSERT(after.find(ptrB) == after.end());
    UT_ASSERT(after.find(ptrC) == after.end());
    UT_ASSERT(after.find(ptrD) == after.end());
#else
    printf("XObjCountTest::TestTrackStack_XRef() DISABLED\n");
    fflush(stdout);
#endif
}


void XObjCountTest::TestTrackStack_XBaseObject()
{
#ifdef TRACK_STACK

    const void* ptrA = 0;
    const void* ptrB = 0;
    const void* ptrC = 0;
    const void* ptrD = 0;

    {
        XIRef<Balderdash> a = new Balderdash;
        XIRef<Balderdash> b = new Balderdash;
        XIRef<Balderdash> c = new Balderdash;
        XIRef<Balderdash> d = new Balderdash;
        ptrA = a.Get();
        ptrB = b.Get();
        ptrC = c.Get();
        ptrD = d.Get();

        {
            map<const void*, string> mid = XObjCount::GetStacks();
            UT_ASSERT(mid.find(ptrA) != mid.end());
            UT_ASSERT(mid.find(ptrB) != mid.end());
            UT_ASSERT(mid.find(ptrC) != mid.end());
            UT_ASSERT(mid.find(ptrD) != mid.end());
        }
    }

    map<const void*, string> after = XObjCount::GetStacks();
    UT_ASSERT(after.find(ptrA) == after.end());
    UT_ASSERT(after.find(ptrB) == after.end());
    UT_ASSERT(after.find(ptrC) == after.end());
    UT_ASSERT(after.find(ptrD) == after.end());
#else
    printf("XObjCountTest::TestTrackStack_XIRef() DISABLED\n");
    fflush(stdout);
#endif
}
