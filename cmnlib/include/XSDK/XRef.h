
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XRef_h_
#define _XRef_h_

#include <cassert>
#include <typeinfo>
#include "XSDK/XGuard.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XObjCount.h"

template<class Tptr>
class XWeakRef;

template<class Tptr>
class XRef
{
    friend class XWeakRef<Tptr>;
    template<class>
    friend class XRef;

public:
    XRef(Tptr* ptr = NULL) :
#if defined(COUNT_REFS) || defined(TRACK_STACK)
        _countLock(),
#endif
        _ptr(NULL),
        _counter(NULL)
    {
        operator=(ptr);
    }

    XRef(const XRef<Tptr>& other) :
#if defined(COUNT_REFS) || defined(TRACK_STACK)
        _countLock(),
#endif
        _ptr(NULL),
        _counter(NULL)
    {
        operator=(other);
    }

    template<class TRelatedPtr>
    XRef(const XRef<TRelatedPtr>& other) :
#if defined(COUNT_REFS) || defined(TRACK_STACK)
        _countLock(),
#endif
        _ptr(NULL),
        _counter(NULL)
    {
        operator=(other);
    }

    XRef(const XWeakRef<Tptr>& other) :
#if defined(COUNT_REFS) || defined(TRACK_STACK)
        _countLock(),
#endif
        _ptr(NULL),
        _counter(NULL)
    {
        operator=(other);
    }

    ~XRef() throw()
    {
        RawPtrRelease();
    }

    Tptr& operator*() const
    {
        assert(_ptr && _counter);
        return *_ptr;
    }

    Tptr* operator->() const
    {
        assert(_ptr && _counter);
        return _ptr;
    }

    XRef<Tptr>& operator=(Tptr* ptr)
    {
        RawPtrRelease();

        if(ptr)
            RawPtrAssign(ptr,new XSDK::XBaseObject);

        return *this;
    }

    XRef<Tptr>& operator=(const XRef<Tptr>& other)
    {
        if (&other == this)
            return *this;

        RawPtrRelease();
        if(other._ptr && other._counter)
            RawPtrAssign(other._ptr,other._counter);
        return *this;
    }

    XRef<Tptr>& operator=(const XWeakRef<Tptr>& other)
    {
        RawPtrRelease();
        if(other._ptr && other._counter.IsValid() && !other._counter->IsOrphaned())
            RawPtrAssign(other._ptr,(XSDK::XBaseObject*)other._counter->GetObjectPtr());
        return *this;
    }

    template<class TRelatedPtr>
    XRef<Tptr>& operator=(const XRef<TRelatedPtr>& other)
    {
        RawPtrRelease();
        if(other._ptr && other._counter)
            RawPtrAssign(Cast<Tptr>(other._ptr),other._counter);
        return *this;
    }

    bool operator==(const Tptr* ptr) const
    {
        return _ptr == ptr;
    }

    bool operator==(const XRef<Tptr>& other) const
    {
        return _ptr == other._ptr;
    }

    bool operator!=(const Tptr* ptr) const
    {
        return !operator==(ptr);
    }

    bool operator!=(const XRef<Tptr>& other) const
    {
        return !operator==(other);
    }

    Tptr* Get() const
    {
        return _ptr;
    }

    int GetReferenceCount()
    {
        return IsValid() ? _counter->GetReferenceCount() : 0;
    }

    bool IsValid() const
    {
        return _ptr && _counter;
    }

    bool IsEmpty() const
    {
        return !IsValid();
    }

    XRef<Tptr>& Clear()
    {
        RawPtrRelease();
        return *this;
    }

private:
    //List parameter type last so we don't need to explicitly state its type.
    template<class U, class T>
    U* Cast(T* ptr) const
    {
        //In general, those that don't explicitly cast the
        //pointer themselves normally expect the implicit cast
        //to do something like the behavior below.
        //Note: It's important to check if the type is related through inheritance
        //first as a c-style cast won't dynamic_cast first and other casts
        //could be "successful".
        //At a later date we need to remove the c-style case as it could
        //be dangerous.
        U* casted = dynamic_cast<U*>(ptr);
        return casted ? casted : (U*)ptr;
    }

    void RawPtrRelease()
    {
#if defined(COUNT_REFS) || defined(TRACK_STACK)
        XSDK::XGuard lock(_countLock);
#endif

        if(_counter && _counter->Release() <= 0)
        {
#ifdef COUNT_REFS
            XSDK::XObjCount::RemoveObj(typeid(*_ptr).name());
#endif
#ifdef TRACK_STACK
            XSDK::XObjCount::RemoveStack(_ptr);
#endif
            delete _ptr;
        }

        _ptr = 0;
        _counter = 0;
    }

    void RawPtrAssign(Tptr* ptr,XSDK::XBaseObject* counter)
    {
#if defined(COUNT_REFS) || defined(TRACK_STACK)
        XSDK::XGuard lock(_countLock);
#endif

        counter->AddRef();

#if defined(COUNT_REFS) || defined(TRACK_STACK)
        if(counter->GetReferenceCount() == 1)
        {
#ifdef COUNT_REFS
            XSDK::XObjCount::AddObj(typeid(*ptr).name());
#endif
#ifdef TRACK_STACK
            XSDK::XObjCount::AddStack(ptr);
#endif
        }
#endif

        _ptr = ptr;
        _counter = counter;
    }

private:

#if defined(COUNT_REFS) || defined(TRACK_STACK)
    XSDK::XMutex _countLock;
#endif

    Tptr* _ptr;
    XSDK::XBaseObject* _counter;
};

#endif
