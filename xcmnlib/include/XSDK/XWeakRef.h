
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XWeakRef_h_
#define _XWeakRef_h_

#include <cassert>
#include "XSDK/Types.h"
#include "XSDK/XRef.h"
#include "XSDK/XException.h"

template<class Tptr>
class XWeakRef
{
    friend class XRef<Tptr>;
    template<class>
    friend class XWeakRef;
public:
    XWeakRef() :
        _ptr(NULL),
        _counter(NULL)
    {
    }

    XWeakRef(XRef<Tptr>& other) :
        _ptr(NULL),
        _counter(NULL)
    {
        operator=(other);
    }

    XWeakRef(const XWeakRef<Tptr>& other) :
        _ptr(NULL),
        _counter(NULL)
    {
        operator=(other);
    }

    ~XWeakRef() throw ()
    {
        RawPtrRelease();
    }

    XWeakRef<Tptr>& operator=(XRef<Tptr>& other)
    {
        RawPtrRelease();
        if(other._ptr && other._counter)
            RawPtrAssign(other._ptr,other._counter->GetProxy());
        return *this;
    }

    XWeakRef<Tptr>& operator=(const XWeakRef<Tptr> &other)
    {
        if (&other == this)
            return *this;

        RawPtrRelease();
        if(other._ptr && other._counter.IsValid())
            RawPtrAssign(other._ptr,other._counter);
        return *this;
    }

    Tptr& operator*() const
    {
        assert(IsValid());
        return *GetTargetPtr();
    }

    Tptr* operator->() const
    {
        assert(IsValid());
        return GetTargetPtr();
    }

    bool operator==(const Tptr* ptr) const
    {
        return GetTargetPtr() == ptr;
    }

    bool operator==(const XWeakRef<Tptr>& other) const
    {
        return GetTargetPtr() == other._ptr;
    }

    bool operator==(const XRef<Tptr>& other) const
    {
        return GetTargetPtr() == other._ptr;
    }

    bool operator!=(const Tptr* ptr) const
    {
        return !operator==(ptr);
    }

    bool operator!=(const XWeakRef<Tptr>& other) const
    {
        return !operator==(other);
    }

    bool operator!=(const XRef<Tptr>& other) const
    {
        return !operator==(other);
    }

    Tptr* Get() const
    {
        return GetTargetPtr();
    }

    bool IsOrphaned() const
    {
        if(IsEmpty())
            return false;

        return _counter->IsOrphaned();
    }

    bool IsValid() const
    {
        return _counter.IsValid() && _ptr;
    }

    bool IsEmpty() const
    {
        return !IsValid();
    }

    XWeakRef<Tptr>& Clear()
    {
        RawPtrRelease();
        return *this;
    }

private:
    Tptr* GetTargetPtr() const
    {
        if(IsEmpty())
            return NULL;

        if (_counter->IsOrphaned())
            X_THROW(("[XWeakRef::GetTargetPtr]: The object pointed to by the WeakRef has been destroyed"));

        return _ptr;
    }

    void RawPtrRelease()
    {
        _counter.Clear();
        _ptr = NULL;
    }

    void RawPtrAssign(Tptr* ptr,XIRef<XSDK::XObjectProxy> counter)
    {
        _ptr = ptr;
        _counter = counter;
    }

    Tptr* _ptr;
    XIRef<XSDK::XObjectProxy> _counter;
};

#endif
