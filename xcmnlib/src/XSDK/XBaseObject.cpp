
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <typeinfo>
#include "XSDK/Errors.h"
#include "XSDK/Module.h"
#include "XSDK/XUuid.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XGuard.h"
#include "XSDK/XObjCount.h"

using namespace std;
using namespace XSDK;

//=============================================================================
// Constructor
//=============================================================================
XBaseObject::XBaseObject() :
    _refCount(0),
    _deleting(false)
{
}

//=============================================================================
// Constructor
//=============================================================================
XBaseObject::XBaseObject(const std::string& name) :
    _refCount(0),
    _deleting(false),
    _className(name)
{
}

//=============================================================================
// Destructor
//=============================================================================
XBaseObject::~XBaseObject()
{
    if (_pProxy.IsValid())
    {
        _pProxy->DetachObject();
        _pProxy.Clear();
    }

}

//=============================================================================
// AddRef
//=============================================================================
int XBaseObject::AddRef() const
{
    _refLock.Acquire();
    X_ASSERT(_refCount >= 0);
    const int ref = ++_refCount;

#if defined(COUNT_REFS) || defined(TRACK_STACK)
    if (ref == 1)
    {
#ifdef COUNT_REFS
        XObjCount::AddObj(typeid(*this).name());
#endif
#ifdef TRACK_STACK
        XObjCount::AddStack(this);
#endif
    }
#endif

    _refLock.Release();
    return (ref);
}

//=============================================================================
// Release
//=============================================================================
int XBaseObject::Release() const
{
    _refLock.Acquire();
    X_ASSERT(_refCount > 0);
    int ref = --_refCount;
    bool deleteObject = false;
    if (ref == 0)
    {
        deleteObject = !_deleting; // MPF-1195: Only delete object once
        _deleting = true;
#ifdef COUNT_REFS
        XObjCount::RemoveObj(typeid(*this).name());
#endif
#ifdef TRACK_STACK
        XObjCount::RemoveStack(this);
#endif
    }
    _refLock.Release();
    if (deleteObject)
    {
        // this call provides a way to do pre-destructor clean up prior to deleting
        //
        const_cast<XBaseObject*>(this)->CleanUp();
        delete this;
    }
    return ref;
}

//=============================================================================
// QueryInterface
//=============================================================================
bool XBaseObject::QueryInterface
(
 IN  const X_UUID& id,
 OUT void** ppInterface
)
const
{
    if (ppInterface == 0)
    {
        XSDK::XException ex("Null pointer passed to XBaseObject::QueryInterface");
        ex.SetThrowPoint(__LINE__, __FILE__);
        throw ex;
    }

    XSDK::XUuid uuid(id);

    if (uuid == IXUnknown::Uuid())
    {
        *ppInterface = (GetIXUnknown()).Raw();
        AddRef();
    }
    else if (uuid == IXWeakReference::Uuid())
    {
        *ppInterface = static_cast<IXWeakReference*>(const_cast<XBaseObject*>(this));
        AddRef();
    }
    else
        return false;
    return true;
}

//=============================================================================
// GetProxy
//=============================================================================
XIRef<XObjectProxy> XBaseObject::GetProxy()
{
    XSDK::XGuard lock(_coreLock);
    if (_pProxy.IsEmpty())
    {
        // Allocate the one and only proxy instance.
        _pProxy = new (std::nothrow) XObjectProxy(this);
        if (_pProxy.IsEmpty())
        {
            X_THROW(("Failed allocate a XProxyObject in XBaseObject."));
        }
    }
    return (_pProxy);
}

//=============================================================================
// GetIXUnknwown
//=============================================================================
XIRef<XSDK::IXUnknown> XBaseObject::GetIXUnknown() const
{
    return XIRef<XSDK::IXUnknown>(static_cast<IXUnknown*>(const_cast<XBaseObject*>(this)));
}

//=============================================================================
// GetReferenceCount
//=============================================================================
int XBaseObject::GetReferenceCount() const
{
    _refLock.Acquire();
    int refCount = _refCount;
    _refLock.Release();
    return (refCount);
}

//=============================================================================
// SetBaseObjectName
//=============================================================================
void XBaseObject::SetObjectName(IN const std::string& name)
{
    XSDK::XGuard lock(_coreLock);
    _className = name;
}

//=============================================================================
// GetBaseObjectName
//=============================================================================
const std::string& XBaseObject::GetObjectName()
{
    XSDK::XGuard lock(_coreLock);
    return _className;
}

//=============================================================================
// CleanUp
//=============================================================================
void XBaseObject::CleanUp()
{
}
