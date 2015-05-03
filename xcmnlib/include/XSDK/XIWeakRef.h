
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_X_WEAK_INSTRUSIVE_PTR_H_
#define _XSDK_X_WEAK_INSTRUSIVE_PTR_H_

//==============================================================================
// Include files
//==============================================================================
#include <assert.h>
#include <cstdlib>
#include "XSDK/Types.h"
#include "XSDK/IXWeakReference.h"
#include "XSDK/XIRef.h"

//=============================================================================
/// The XIWeakRef class is used to hold a "weak" reference to a separate
/// reference counted object (i.e. one that implements IXUnknown). It is used
/// much the same way as XIRef, except it does @b NOT call AddRef or
/// Release on the object it contains. This is why the reference is called
/// "weak"... putting an object in a XIWeakRef doesn't guarantee the lifetime
/// of the contained object like using XIRef would. What this means is
/// that it's possible for the reference count of the object to reach 0 and
/// for that object to be deleted while the XIWeakRef is still valid. However,
/// XIWeakRef is designed to handle this by throwing an exception (as opposed
/// to segfaulting).
///
/// So, why is this useful? I'm glad you asked!
///
/// A common problem in reference counted systems are circular references.
/// Circular references are when Object A hold a reference to Object B, while
/// Object B holds a reference back to Object A. The problem is that neither
/// object's reference count can ever reach 0 because each object is waiting
/// for the other one to release it (think of it as a deadlock, but with
/// references). In this case, a memory leak occurs because neither object is
/// ever deleted. This example may seem straight-forward, but circular
/// references can be more complex and hard to track down. Consider the
/// following example:
///
/// -> Object A -> Object B -> Object C -> Object D -
/// |                                                |
///  ------------------------------------------------
///
/// In this case, the circular reference is not immediately obvious simply by
/// looking at Object A and B's code. However, the problem is still there.
///
/// Bad news:  XIWeakRef can't help you track down the problem.
/// Good news: XIWeakRef can help you fix the problem once you find it.
///
/// Let's go back to the original example:
///
/// Object A <--> Object B
///
/// Object A has a member variable XIRef<ObjectB>, while Object B has a
/// member variable XIRef<ObjectA>. The solution is to replace one of the
/// XIRefs with a XIWeakRef.
///
/// @note Determining which XIRef to replace is implementation specific.
/// If one object can function without the other (i.e. due to a one-to-many or
/// parent-child relationship between classes), then it usually makes sense to
/// have that one object hold a hard reference (XIRef) to the other object,
/// while the other object holds a weak reference back.
///
/// To solve the problem mentioned above, we change Object B to have a weak
/// reference back to Object A (i.e. Object B's member variable will change to
/// XIWeakRef<ObjectA>). Behind the scenes, XIWeakRef will ask Object A to
/// create a proxy for itself, through which XIWeakRef will access Object A.
///
///    ----- Object A Proxy <---
///    |                        |
/// Object A -------------> Object B
///
/// Object B's XIWeakRef actually holds a hard reference to the proxy (so it
/// is guaranteed not to prematurely go away), but all access to Object A is
/// through the proxy. Object A is now free to go away when it's reference
/// count reaches 0, as long as it notifies its proxy that the pointer to it
/// is no longer valid. Object A will release its reference to Object B, giving
/// it a chance to go away. If there are still other references to Object B
/// it can continue to try and access Object A, but the proxy (and thus the
/// XIWeakRef) will throw an exception instead of segfaulting.
///
/// So, what do I have to do to get my object to work with XIWeakRef?
///
/// The simple answer is have your object derive from XSDKObject and you get
/// all this for free.
///
/// If you don't derive from XSDKObject, then your object @b must implement
/// IXWeakReference. XIWeakRef will query for this interface and use the
/// GetProxy() method to get a pointer to your object's proxy. The proxy is
/// already implemented in XObjectProxy, so you simply need to create an
/// instance of it in GetProxy. Then, when your object goes away, make sure to
/// call XObjectProxy::DetachObject() on all the proxies you created.
///
/// @note It's easiest to only allow a single instance of your object's proxy.
///
/// A sample implementation is below:
///
/// @code
/// // MyClass.h
/// \n
/// class MyClass : public IXWeakReference
/// {
/// public:
///    // C++ class goodness goes here...
///
///    // IXWeakReference implementation
///    virtual XIRef<XSDK::XObjectProxy> GetProxy();
///
/// private:
///    XIRef<XSDK::XObjectProxy> _pProxy;
/// };
/// \n\n
/// // MyClass.cpp
/// \n
/// MyClass::MyClass() : _pProxy(0)
/// {
/// }
///
/// MyClass::~MyClass()
/// {
///     if (_pProxy.IsValid())
///         _pProxy->DetachObject();
/// }
///
/// XIRef<XObjectProxy> MyClass::GetProxy()
/// {
///     // Maybe grab a lock here...
///     if (_pProxy.IsEmpty())
///     {
///         // Allocate the one and only proxy instance.
///         _pProxy = new (std::nothrow) XObjectProxy(this);
///         if (_pProxy.IsEmpty())
///             // Throw an exception here...
///     }
///     return (_pProxy);
/// }
/// @endcode
//=============================================================================
template<class TpPtr>
class XIWeakRef
{
public:
    //==========================================================================
    /// @brief  Constructor.
    ///
    ///         This method is used to initialize the object. If a null pointer
    ///         is specified, the object will be initialized to its empty state.
    ///
    /// @param  pPtr If this value is non-NULL, the object pointed to will have
    ///         a weak reference added to it. If the pointer is NULL, the
    ///         current object will be initialized to its "empty" state.
    //==========================================================================
    XIWeakRef(IN TpPtr* pPtr = NULL) :
        _pProxy()
    {
        // Set the initial state of the object with the given raw pointer value
        operator=(pPtr);
    }

    //==========================================================================
    /// @brief  Copy constructor.
    ///
    ///         This method copies the contents of the given object into the
    ///         current object. If the current object already contains a weak
    ///         reference to an object, that weak reference will be discarded
    ///         before the contents of the given object is copied.
    ///
    /// @param  rOther This is the object that will be copied from.
    //==========================================================================
    XIWeakRef(IN const XIWeakRef<TpPtr>& rOther) :
        _pProxy()
    {
        // Copy the contents of the given object
        operator=(rOther);
    }

    //==========================================================================
    /// @brief  Constructor.
    ///
    ///         This method copies the contents of a XIRef object into the
    ///         current object. If the current object already contains a weak
    ///         reference to an object, that weak reference will be discarded
    ///         before the contents of the given object is copied.
    ///
    /// @param  rOther This is the object that will be copied from.
    //==========================================================================
    XIWeakRef(IN const XIRef<TpPtr>& rOther) :
        _pProxy()
    {
        // Copy the contents of the given object
        operator=(rOther);
    }

    //==========================================================================
    /// @brief  Assignment operator
    ///
    ///         This method is used to assign a raw pointer to the object. If
    ///         the object currently owns a reference, it will release that
    ///         reference before storing the given pointer.
    ///
    /// @param  pPtr This value is used to initialize the raw pointer
    ///         maintained by the object. If the value is non-NULL, the object
    ///         will have it's reference count incremented.
    //==========================================================================
    XIWeakRef<TpPtr>& operator=(IN const TpPtr *pPtr)
    {
        // Assign the new pointer to this object and then report
        // a reference to the current object to the caller
        RawPtrAssign(pPtr);

        return *this;
    }

    //==========================================================================
    /// @brief  Destructor.
    ///
    ///         This method is used to release any resources held by the object
    ///         before the object goes out of scope and those resources can no
    ///         longer be recovered.
    //==========================================================================
    ~XIWeakRef() throw ()
    {
        // Release any raw pointer currently contained within the object
        RawPtrRelease();
    }

    //==========================================================================
    /// @brief  Reference operator.
    ///
    ///         This method is used to obtain a reference to the object pointed
    ///         to by the proxy maintained in the object. The method will
    ///         assert (in debug mode) if the current object has not been
    ///         assigned a raw pointer value before this method is called.
    //==========================================================================
    TpPtr& operator*()
    {
        // The object was never assigned a valid pointer.
        assert(_pProxy.IsValid());

        // Report a reference to the object pointed to by our proxy.
        return (*(GetTargetPtr()));
    }

    //==========================================================================
    /// @brief  Pointer operator.
    ///
    ///         This method is used to obtain the raw pointer maintained by the
    ///         object (via the proxy) when the caller is using pointer notation
    ///         to access the  raw pointer. The method will assert (in debug
    ///         mode) if the object has not been assigned a raw pointer value
    ///         before this method is called.
    //==========================================================================
    TpPtr* operator->()
    {
        // The object was never assigned a valid pointer.
        assert(_pProxy.IsValid());

        // Use the proxy to return the object's pointer.
        return GetTargetPtr();
    }

    //==========================================================================
    /// @brief  Pointer operator (constant).
    ///
    ///         This method is used to obtain the raw pointer maintained by the
    ///         object (via the proxy) when the caller is using pointer notation
    ///         to access the  raw pointer. The method will assert (in debug
    ///         mode) if the object has not been assigned a raw pointer value
    ///         before this method is called.
    //==========================================================================
    const TpPtr* operator->() const
    {
        // The object was never assigned a valid pointer.
        assert(_pProxy.IsValid());

        // Use the proxy to return the object's pointer.
        return GetTargetPtr();
    }

    //==========================================================================
    /// @brief  Assignment operator for same parameterized type.
    ///
    ///         This method is used to assign instances of the class when the
    ///         template parameters for the classes match. If the current object
    ///         already has a weak pointer reference, the reference will be
    ///         released before the assignment.
    ///
    /// @param  rOther This is the object to be copied from.
    //==========================================================================
    XIWeakRef<TpPtr>& operator=
    (IN const XIWeakRef<TpPtr> &rOther)
    {
        // If it is I, stop here
        if (&rOther == &(*this))
            return (*this);

        // Assign the new pointer to this object and then report
        // a reference to the current object to the caller.
        RawPtrAssign(rOther.Raw());

        return *this;
    }

    //==========================================================================
    /// @brief  Assignment operator for same parameterized type.
    ///
    ///         This method is used to assign the value of a XIRef to this
    ///         object when the template parameters for both classes match. If
    ///         the current object already has a weak pointer reference, the
    ///         reference will be released before the assignment.
    ///
    /// @param  rOther This is the object to be copied from.
    //==========================================================================
    XIWeakRef<TpPtr>& operator=
    (IN const XIRef<TpPtr> &rOther)
    {
        // Assign the new pointer to this object and then report
        // a reference to the current object to the caller.
        RawPtrAssign(rOther.Raw());

        return *this;
    }

    //==========================================================================
    /// @brief  Reports the value to the object's raw pointer.
    ///
    /// @return Returns the raw pointer maintained by the proxy.
    //==========================================================================
    TpPtr* Raw() const
    {
        // Use the proxy to return the object's pointer.
        return GetTargetPtr();
    }

    //==========================================================================
    /// @brief  Reports a constant pointer to the counter object
    ///
    ///         This method is used to obtain a constant pointer to the object's
    ///         counter.
    ///
    /// @return Reports a counter object.
    //==========================================================================
    TpPtr* GetCounter() const
    {
        return GetTargetPtr();
    }

    //==========================================================================
    /// @brief  Reports whether or not the object is "empty".
    ///
    ///         When the object does not contain a pointer, it is considered
    ///         "empty". This method is used to report the "empty" status of the
    ///         object.
    ///
    /// @return The method returns true if there is no pointer (and therefore
    ///         no reference) currently being maintained by the object. If the
    ///         object does contain a pointer, the method returns false.
    //==========================================================================
    bool IsEmpty() const
    {
        // Reports whether or not the object is empty
        return (_pProxy.IsEmpty());
    }

    //==========================================================================
    /// @brief  Reports whether or not the object is "valid".
    ///
    ///         When the object contains a pointer, it is considered "valid".
    ///         This method is used to report the "valid" status of the object.
    ///
    /// @return The method returns true if there is a pointer currently being
    ///         maintained by the object. If the object does not contain a
    ///         pointer, the method returns false.
    //==========================================================================
    bool IsValid() const
    {
        // Reports whether or not the object is empty
        return (_pProxy.IsValid());
    }

    //==========================================================================
    /// @brief  Reports whether or not the object has been deleted.
    ///
    ///         When the XIWeakRef contains an object, it is considered orphaned
    ///         when that object has been deleted. This method allows the caller
    ///         to determine if the contained object has been deleted without
    ///         having to wait for an exception to be thrown.
    ///
    /// @return The method returns true if the object pointed to has been
    ///         deleted. It returns false if either (a) the object is still
    ///         valid, or (b) if the XIWeakRef is currently empty.
    //==========================================================================
    bool IsOrphaned() const
    {
        // We can't be orphaned if nothing has been set yet.
        if (_pProxy.IsEmpty())
            return false;

        // Delegate the task.
        return (_pProxy->IsOrphaned());
    }

    //==========================================================================
    /// @brief  Clears any reference held by the object.
    ///
    ///         This method is used to clear any reference held by the object.
    ///         This method is particularly helpful when working with the
    ///         object's Raw() method. Often times, reference pointers are
    ///         received by passing a pointer-pointer into a function through
    ///         its parameter list. The object's Raw() method can be used to get
    ///         the raw pointer ("&obj.Raw()"), but if the object already owns a
    ///         reference in this case, that reference will be overwritten. This
    ///         will likely cause an extra reference to be held on the previous
    ///         raw pointer which will likely also translate to a memory leak.
    ///         To prevent this problem, this method should be called before
    ///         calling the Raw() method. In order to make this convenient, the
    ///         method has been implemented such that it can be chained (i.e.,
    ///         "obj.Clear().Raw()")
    //==========================================================================
    XIWeakRef<TpPtr>& Clear()
    {
        // Clear any raw pointer the object contains and
        // then report a reference to the current object
        RawPtrRelease();

        return *this;
    }

    //==========================================================================
    /// @brief  Equal comparison operator.
    ///
    ///         This method is used to compare the pointer value stored within
    ///         the current object with another pointer.
    ///
    /// @param  pPtr The pointer to compare the internal pointer to.
    ///
    /// @return true if the pointers are equal. false otherwise.
    //==========================================================================
    bool operator==(IN const TpPtr* pPtr) const
    {
        return (GetTargetPtr() == pPtr ? true : false);
    }

    //==========================================================================
    /// @brief  Equal comparison operator (XIWeakRef).
    ///
    ///         This method is used to compare the pointer value stored within
    ///         the current object with the pointer value stored in another
    ///         XIWeakRef object.
    ///
    /// @param  rOther A reference to the XIWeakRef object to compare against.
    ///
    /// @return true if the stored pointers are equal. false otherwise.
    //==========================================================================
    bool operator==(IN const XIWeakRef<TpPtr>& rOther) const
    {
        return (GetTargetPtr() == rOther.Raw() ? true : false);
    }

    //==========================================================================
    /// @brief  Equal comparison operator (XIRef).
    ///
    ///         This method is used to compare the pointer value stored within
    ///         the current object with the pointer value stored in another
    ///         XIRef object.
    ///
    /// @param  rOther A reference to the XIRef object to compare against.
    ///
    /// @return true if the stored pointers are equal. false otherwise.
    //==========================================================================
    bool operator==(IN const XIRef<TpPtr>& rOther) const
    {
        return (GetTargetPtr() == rOther.Raw() ? true : false);
    }

    //==========================================================================
    /// @brief  Not-equal comparison operator.
    ///
    ///         This method is used to compare the pointer value stored within
    ///         the current object with another pointer.
    ///
    /// @param  pPtr The pointer to compare the internal pointer to.
    ///
    /// @return true if the pointers are @b not equal. false otherwise.
    //==========================================================================
    bool operator!=(IN const TpPtr* pPtr) const
    {
        return (GetTargetPtr() != pPtr ? true : false);
    }

    //==========================================================================
    /// @brief  Not-equal comparison operator (XIWeakRef).
    ///
    ///         This method is used to compare the pointer value stored within
    ///         the current object with the pointer value stored in another
    ///         XIWeakRef object.
    ///
    /// @param  rOther A reference to the XIWeakRef object to compare against.
    ///
    /// @return true if the stored pointers are @b not equal. false otherwise.
    //==========================================================================
    bool operator!=(IN const XIWeakRef<TpPtr>& rOther) const
    {
        return (GetTargetPtr() != rOther.Raw() ? true : false);
    }

    //==========================================================================
    /// @brief  Not-equal comparison operator (XIRef).
    ///
    ///         This method is used to compare the pointer value stored within
    ///         the current object with the pointer value stored in another
    ///         AutoRef object.
    ///
    /// @param  rOther A reference to the AutoRef object to compare against.
    ///
    /// @return true if the stored pointers are @b not equal. false otherwise.
    //==========================================================================
    bool operator!=(IN const XIRef<TpPtr>& rOther) const
    {
        return (GetTargetPtr() != rOther.Raw() ? true : false);
    }

private:

    //==========================================================================
    /// @brief Returns the pointer to the object pointed to by the proxy.
    //==========================================================================
    TpPtr* GetTargetPtr() const
    {
        if (_pProxy.IsEmpty())
            return 0;

        // Use the proxy to return the object's pointer. We know that the
        // object implements IXUnknown if we've gotten this far.
        return (dynamic_cast<TpPtr*>(static_cast<XSDK::IXUnknown*>(_pProxy->GetObjectPtr())));
    }

    //==========================================================================
    /// @brief  Releases the pointer to the object proxy.
    //==========================================================================
    void RawPtrRelease()
    {
        _pProxy.Clear();
    }

    //==========================================================================
    /// @brief  Assigns a new raw pointer to the object.
    ///
    /// @param  pPtr This is the raw pointer that will be stored by the object.
    //==========================================================================
    void RawPtrAssign(IN const TpPtr *pPtr)
    {
        // Release any raw pointer we already have
        RawPtrRelease();

        // If the given object is not empty, get it's proxy object
        if (pPtr != NULL)
        {
            // We can only store objects that implement IXWeakReference.
            //
            // NOTE: Do not call QueryInterface() here because it will addref the
            // target object. This causes huge problems when XIWeakRef is used
            // in an object's constructor. The object will still have a refcount
            // of 0, so the addref/release cycle will cause the object to be
            // deleted from it's own constructor! <tdistler>
            XSDK::IXWeakReference* pIWeakRef = dynamic_cast<XSDK::IXWeakReference*>(const_cast<TpPtr*>(pPtr));
            X_ASSERT(pIWeakRef != 0);

            _pProxy = pIWeakRef->GetProxy();
        }
    }

    //==========================================================================
    /// This is the pointer to the proxy object which is used as a mediator for
    /// all calls to the target object.
    //==========================================================================
    XIRef<XSDK::XObjectProxy> _pProxy;
};

#endif
