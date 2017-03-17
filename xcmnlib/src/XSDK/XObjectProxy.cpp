
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/MemDebugBegin.h"
#include "XSDK/Assert.h"
#include "XSDK/Module.h"
#include "XSDK/Errors.h"
#include "XSDK/XUuid.h"
#include "XSDK/XObjectProxy.h"
#include "XSDK/MemDebugEnd.h"
#include "XSDK/XGuard.h"

using namespace XSDK;

//=============================================================================
// Constructor
//=============================================================================
XObjectProxy::XObjectProxy(IN void* pObject) :
    _refCount(0),
    _pObject(pObject)
{
}

//=============================================================================
// Destructor
//=============================================================================
XObjectProxy::~XObjectProxy() throw()
{
    _pObject = 0;
}

//=============================================================================
// AddRef
//=============================================================================
int XObjectProxy::AddRef() const
{
    _coreLock.Acquire();
    X_ASSERT(_refCount >= 0);
    int ref = ++_refCount;
    _coreLock.Release();
    return (ref);
}

//=============================================================================
// Release
//=============================================================================
int XObjectProxy::Release() const
{
    _coreLock.Acquire();
    X_ASSERT(_refCount > 0);
    int ref = --_refCount;
    _coreLock.Release();
    if (ref == 0)
    {
        // Commit suicide
        delete this;
    }
    return (ref);
}

//=============================================================================
// GetReferenceCount
//=============================================================================
int XObjectProxy::GetReferenceCount() const
{
    XGuard lock(_coreLock);
    return _refCount;
}

//=============================================================================
// QueryInterface
//=============================================================================
bool XObjectProxy::QueryInterface
(
 IN  const X_UUID& id,
 OUT void** ppInterface
)
const
{
    if (ppInterface == 0)
        X_THROW(("Null pointer passed to XSDKObject::QueryInterface"));

    XSDK::XUuid uuid(id);

    if (uuid == IXUnknown::Uuid())
    {
        *ppInterface = static_cast<XSDK::IXUnknown*>(const_cast<XObjectProxy*>(this));
        AddRef();
        return true;
    }
    return false;
}

//=============================================================================
// DetachObject
//=============================================================================
void XObjectProxy::DetachObject()
{
    XSDK::XGuard lock(_coreLock);
    _pObject = 0;
}

//=============================================================================
// IsOrphaned
//=============================================================================
bool XObjectProxy::IsOrphaned() const
{
    XSDK::XGuard lock(_coreLock);
    return _pObject == 0;
}

//=============================================================================
// GetObjectPtr
//=============================================================================
void* XObjectProxy::GetObjectPtr() const
{
    XSDK::XGuard lock(_coreLock);
    if (_pObject == 0)
    {
        X_THROW(("The object pointed to by the proxy object has been destroyed."));
    }
    return (_pObject);
}

