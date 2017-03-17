#ifndef _TEST_REFD_OBJECT_H_
#define _TEST_REFD_OBJECT_H_

#include "XSDK/IXUnknown.h"
#include "XSDK/Object.h"

class TestRefdObject : public XSDK::IXUnknown
{
public:
    TestRefdObject(bool* pDeleted) :
      _refCount(0),
      _pDeleted(pDeleted)
    {
        if (_pDeleted)
            *_pDeleted = false;
    }
    ~TestRefdObject() {}

    int GetRefCount() { return (_refCount); }

    DEFINE_CLASS_ID(0x8e,0x91,0x1a,0xa4,0xe8,0x33,0x41,0xd9,0xbd,0xff,0x40,0x31,0xd,0xfa,0xa4,0xce);

    // IXUnknown implmentation.
    virtual int AddRef() const { return (++_refCount); }
    virtual int Release() const
    {
        int ref = --_refCount;
        if (ref == 0)
        {
            if (_pDeleted)
                *_pDeleted = true;
            _pDeleted = 0;
            delete this;
        }
        return (ref);
    }

    virtual X_API int GetReferenceCount() const { return _refCount; }

    virtual bool QueryInterface(IN  const X_UUID& id,
        OUT void** ppInterface) const { return false; }
    virtual const X_UUID GetClassId() const { return ClassId(); }

private:
    mutable int   _refCount;
    mutable bool* _pDeleted; // Indicates if the object has been deleted.
};

#endif // _TEST_REFD_OBJECT_H_

