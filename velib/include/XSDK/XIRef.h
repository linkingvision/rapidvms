
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK__X_I_REF_H__
#define _XSDK__X_I_REF_H__

//==============================================================================
// Include files
//==============================================================================
#include <assert.h>
#include <cstdlib>
#include "XSDK/XGuard.h"

template<class TPtr>
class XIWeakRef;

//==============================================================================
/// @brief This class acts as a "smart pointer" for reference counted objects.
///
/// The class acts as a "smart pointer" for reference counted objects. The
/// class will increment the reference count of any object assigned to it.
/// When an instance of this class goes out of scope, it will call the
/// Release() method on any pointer it has been assigned.
/// \n\n
/// For a discussion of "smart pointers" and their advantages and draw backs,
/// see the book "More Effective C++" by Scott Meyers. Specifically, see the
/// items 28-29.
/// \n\n
/// @b Common @b Problems
/// \n\n
/// There are a few problems that commonly occur when people first begin using
/// this object. Making a note of the following may save some time in the
/// future.
/// \n
/// @note Throughout the following "AutoRef" is used to refer to instances
/// of the current class type.
///
/// @li @b Know @b Which @b Operator @b to @b Use - You must understand the
/// difference between using the "." operator and the "->" operator (and the
/// "&" operator for that matter). Assuming you are using an instance of an
/// AutoRef object on the stack, the "." operator will refer to the AutoRef
/// object, not the pointer within the AutoRef object. When using the "."
/// operator you will be able to access the methods implemented by the instance
/// of the AutoRef object (i.e., .IsEmpty(), .Raw(), etc.). If you use the "->"
/// operator (or "&" operator), you will be working with the pointer contained
/// inside the AutoRef. This can cause headaches when the AutoRef object and
/// the object pointed to by the AutoRef both implement a method with the same
/// name. Lets suppose you have an object pointed to by an AutoRef and that
/// object has a Clear() method. Well, the AutoRef object has a Clear() method
/// as well. If you accidentally use the "autoRef.Clear()" method instead of
/// the "autoRef->Clear()" method, you will have just released your reference
/// count on the object within the AutoRef (possibly causing the object to be
/// deleted). If you meant to call the Clear() method on the pointer contained
/// within the AutoRef, you will likely run into problems. Consider the
/// following:
/// \n\n
/// @code
/// // Clear my object (oops, actually cleared
/// // the pointer internal to the AutoRef)
/// autoRef.Clear();
///
/// // Do something with my object (segmentation fault
/// // since we accidentally cleared the AutoRef object)
/// autoRef->DoSomething();
/// @endcode
/// \n
/// Someone writing this code would likely be expecting to call the Clear()
/// method on the pointed to object and then call the DoSomething() method
/// on that same object. The problem here is that the "." operator is being
/// used. Instead of clearing the pointed to object, the Clear() call will
/// clear the AutoRef object. When the second call is made, it will be made
/// with a NULL pointer (this will assert when in debug mode, but all bets
/// are off in release mode).
/// \n\n
/// @li @b Know @b How @b References @b are @b Handled - It is important
/// to understand how the AutoRef objects manage reference counts when they
/// are constructed or assigned to. When a (raw) pointer is supplied to an
/// AutoRef object, either through construction or assignment, the AutoRef
/// object increments the reference count of the pointer. Furthermore, the
/// AutoRef object will decrement the reference count on the pointer when the AutoRef object goes out of scope.
/// Basically, when supplying a raw pointer to an AutoRef, the AutoRef object
/// will @b always increment the reference count on that object.
/// \n
/// Given the way reference counts are handled, AutoRef objects can always
/// be used safely with STL containers:
/// \n\n
/// @code
/// // Get a container for our objects
/// typedef std::vector<XIRef<MyClass> > REFED_CLASSES;
/// REFED_CLASSES refedClasses;
///
/// // Get an object to put into our container
/// XIRef<MyClass> arMyClass(new MyClass());
///
/// // Put the object into our container (problem here)
/// refedClasses.push_back(arMyClass.Raw());
/// @endcode
/// \n
/// That's it... no reference count problems. When arMyClass goes out of scope
/// it will release it's reference on MyClass. However, the object will still
/// exist as long as it is in the refedClasses container. When the container is
/// destroyed, the AutoRef in the container will automatically release it's
/// reference to the MyClass object, which will cause the object to delete
/// itself.
/// \n\n
/// @li @b Understand @b the @b Pitfalls @b of @b the @b .Raw() @b Method -
/// It is important to understand how you can shoot yourself in the foot when
/// using the .Raw() method. When using the Raw() method, you have direct
/// access to the internal pointer contained within the AutoRef object (Raw()
/// returns a reference to the pointer data member within the object). If you
/// clear this pointer, the AutoRef will not be able to release the reference
/// it was given responsibility for. Likewise, if you overwrite this pointer
/// with a different pointer, the AutoRef object will not be able to release
/// the reference count it owned on the previously held pointer. Doing the
/// following, for example, will likely cause a memory leak:
/// \n\n
/// @code
/// // Put a pointer to a new object into the AutoRef object
/// XIRef<CMyRefedClass> arMyClass(new CMyRefedClass());
///
/// // Put a new pointer into the AutoRef (memory leak)
/// arMyClass.Raw() = new CMyRefedClass();
/// @endcode
/// \n
/// In this case, the AutoRef will release the reference on the second
/// allocated object, but the pointer to the first allocated object will be
/// lost and a memory leak will have occurred.
/// \n\n
/// An easy way to get in trouble with this is when references to objects
/// are being obtained within a loop. Consider the following:
/// \n\n
/// @code
/// // Get a AutoRef object to work with
/// XIRef<CMyRefedClass> arMyClass;
///
/// // Cycle until all problems have been solved
/// while (bMore)
/// {
///     // Get a new object reference and find out if we are to continue
///     bMore = GetAnObjectRef(arMyClass.Raw());
///     .
///     .
///     .
/// }
/// @endcode
/// \n
/// In this example, the GetAnObjectRef() routine expects a reference to a
/// pointer to a CMyRefedClass. Internally, the GetAnObjectRef() creates a
/// new object and stores a pointer to the new object within the pointer
/// reference it has been provided. The problem here is that each iteration
/// of the loop will overwrite the pointer within the AutoRef object. If you
/// are counting on that AutoRef to decrement a reference count from each of
/// the objects it holds while looping, you will be sorely disappointed. In
/// order to fix the problem, the AutoRef object's Clear() method should be
/// called. This will force the AutoRef to release a reference count on the
/// object it is pointing to. If the following were to be used within the
/// loop, the problem would be fixed:
/// \n\n
/// @code
/// // Cycle until all problems have been solved
/// while (bMore)
/// {
///     // Release any reference already held
///     arMyClass.Clear();
///
///     // Get a new object reference and find out if we are to continue
///     bMore = GetAnObjectRef(arMyClass.Raw());
///     .
///     .
///     .
/// }
/// @endcode
/// \n
/// The Clear() method supports chaining. If preferred, the code could be
/// shrunk down to the following:
/// \n\n
/// @code
/// // Cycle until all problems have been solved
/// while (bMore)
/// {
///     // Get a new object reference and find out if we are to continue
///     bMore = GetAnObjectRef(arMyClass.Clear().Raw());
///     .
///     .
///     .
/// }
/// @endcode
/// \n\n
/// @li @b Multiple @b Threads @b and @b Reference @b Counting - Understand
/// the issues with multiple threads and reference counting. This is not so
/// much an AutoRef object issue as much as it is a reference counting issue,
/// but this seemed like a good place to mention it. By default, reference
/// counting is not a thread protected operation. If you hold a reference
/// to an object within two or more threads, you should use your own lock
/// (internal to the object, or supplied through some other mechanism) to
/// protect the reference counting. The std::string class, for example, can
/// experience corruption when used in a multi-threaded environment as well
/// (that's why we generally make deep copies of such objects when passing them
/// between threads). If you encounter reference counted objects being deleted
/// more than once, keep in mind that this could be due to multiple threads
/// trying to control the object's reference count.
///
/// @param TPtr The class the autoref will contain. This type must implement
///              the IXRef interface.
//==============================================================================

//In this case we have determined that the type derives from IXUnknown
template<class TPtr>
class XIRef
{
public:
    //==========================================================================
    /// @brief  Constructor.
    ///
    ///         This method is used to initialize the object. If a null pointer
    ///         is specified, the object will be initialized to its empty state.
    ///
    /// @param  ptr If this value is non-NULL, the object pointed to will have
    ///         its reference count incremented. If the pointer is NULL,
    ///         the current object will be initialized to its "empty" state.
    //==========================================================================
    XIRef(IN TPtr* ptr = NULL)  :
        _ptr(NULL)
    {
        // Set the initial state of the object with the given raw pointer value
        operator=(ptr);
    }

    //==========================================================================
    /// @brief  Copy constructor.
    ///
    ///         This method copies the contents of the given object into the
    ///         current object. If the given object contains a raw pointer, that
    ///         pointer will have its reference count incremented before it is
    ///         stored by the current object. If the current object contains a
    ///         raw pointer when this method is called, the Release() method
    ///         will be called on that pointer before the assignment from the
    ///         other object takes place.
    ///
    /// @param  other This is the object that will be copied from.
    //==========================================================================
    XIRef(IN const XIRef<TPtr>& other) :
        _ptr(NULL)
    {
        // Copy the contents of the given object
        operator=(other);
    }

    template<class TpRelatedPtr>
    XIRef(IN const XIRef<TpRelatedPtr>& other) :
        _ptr(NULL)
    {
        // Copy the contents of the given object
        operator=(other);
    }

    //==========================================================================
    /// @brief  Copy constructor.
    ///
    ///         This method copies the contents of the given object into the
    ///         current object. If the given object contains a raw pointer, that
    ///         pointer will have its reference count incremented before it is
    ///         stored by the current object. If the current object contains a
    ///         raw pointer when this method is called, the Release() method
    ///         will be called on that pointer before the assignment from the
    ///         other object takes place.
    ///
    /// @param  other This is the object that will be copied from.
    //==========================================================================
    XIRef(IN const XIWeakRef<TPtr>& other) :
        _ptr(NULL)
    {
        operator=(other);
    }

    //==========================================================================
    /// @brief  Assignment operator
    ///
    ///         This method is used to assign a raw pointer to the object. If
    ///         the object currently owns a reference, it will release that
    ///         reference before storing the given pointer.
    ///
    /// @param  ptr This value is used to initialize the raw pointer
    ///         maintained by the object. If the value is non-NULL, the object
    ///         will have it's reference count incremented.
    //==========================================================================
    XIRef<TPtr>& operator=(IN TPtr *ptr)
    {
        // Assign the new pointer to this object and then report
        // a reference to the current object to the caller
        RawPtrAssign(ptr);

        return *this;
    }

    //==========================================================================
    /// @brief  Destructor.
    ///
    ///         This method is used to release any resources held by the object
    ///         before the object goes out of scope and those resources can no
    ///         longer be recovered. Specifically, the method calls the
    ///         Release() method on any non-null raw pointer it is maintaining.
    //==========================================================================
    ~XIRef()
    {
        // Release any raw pointer currently contained within the object
        RawPtrRelease();
    }

    //==========================================================================
    /// @brief  Reference operator.
    ///
    ///         This method is used to obtain a reference to the object pointed
    ///         to by the raw pointer maintained in the object. The method will
    ///         assert (in debug mode) if the current object has not been
    ///         assigned a raw pointer value before this method is called.
    //==========================================================================
    TPtr& operator*()
    {
        // The object does not currently contain a raw pointer
        assert(_ptr != NULL);

        // Report a reference to the object pointed to by our raw pointer
        return (*_ptr);
    }

    //==========================================================================
    /// @brief  Pointer operator.
    ///
    ///         This method is used to obtain the raw pointer maintained by the
    ///         object when the caller is using pointer notation to access the
    ///         raw pointer. The method will assert (in debug mode) if the
    ///         object has not been assigned a raw pointer value before this
    ///         method is called.
    //==========================================================================
    TPtr* operator->()
    {
        // The object does not currently contain a raw pointer
        assert(_ptr != NULL);

        // Report the object's raw pointer value
        return (_ptr);
    }

    //==========================================================================
    /// @brief  Pointer operator (constant).
    ///
    ///         This method is used to obtain the raw pointer maintained by the
    ///         object when the caller is using pointer notation to access the
    ///         raw pointer. The method will assert (in debug mode) if the
    ///         object has not been assigned a raw pointer value before this
    ///         method is called.
    //==========================================================================
    const TPtr* operator->() const
    {
        // The object does not currently contain a raw pointer
        assert(_ptr != NULL);

        // Report the object's raw pointer value
        return (_ptr);
    }

    //==========================================================================
    /// @brief  Assignment operator for same parameterized type.
    ///
    ///         This method is used to assign instances of the class when the
    ///         template parameters for the classes match. If the current object
    ///         already has a raw pointer reference, the reference will be
    ///         released before the assignment. If the given object has a
    ///         non-NULL reference, the method will call the raw pointer's
    ///         AddRef() method to reflect the new raw pointer being stored by
    ///         the current object.
    ///
    /// @param  other This is the object to be copied from.
    //==========================================================================
    XIRef<TPtr>& operator=
    (IN const XIRef<TPtr> &other)
    {
        // If it is I, stop here
        if (&other == &(*this))
            return (*this);

        // Get the raw constant pointer from the given object
        const TPtr *pConstPtr = other.Raw();

        // Assign the new pointer to this object - adding a reference
        // in the assignment (We take the "const" off of the raw
        // pointer since we are not "logically" modifying the object)
        RawPtrAssign(const_cast<TPtr *>(pConstPtr));

        return *this;
    }

    //==========================================================================
    /// @brief  Assignment operator for Weak Refs
    ///
    ///         If the current object already has a raw pointer reference,
    ///         the reference will be released before the assignment. If
    ///         the given object has a non-NULL a reference, the method will
    ///         call the raw pointer's AddRef() method to reflect the new
    ///         raw pointer being stored by the current object.
    ///
    /// @param  other This is the object to be copied from.
    //==========================================================================
    XIRef<TPtr>& operator=
    (IN const XIWeakRef<TPtr> &other)
    {
        // Get the raw constant pointer from the given object
        const TPtr *pConstPtr = other.Raw();

        // Assign the new pointer to this object - adding a reference
        // in the assignment (We take the "const" off of the raw
        // pointer since we are not "logically" modifying the object)
        RawPtrAssign(const_cast<TPtr *>(pConstPtr));

        return *this;
    }

    //==========================================================================
    /// @brief  Assignment operator for any differing parameterized type.
    ///
    ///         This method is used to assign instances of the class when the
    ///         template parameters for the classes differ. If the current
    ///         object already has a raw pointer reference, the reference will
    ///         be released before the assignment. If the given object has a
    ///         non-NULL a reference, the method will call the raw pointer's
    ///         AddRef() method to reflect the new raw pointer being stored by
    ///         the current object.
    ///
    /// @param  other This is the object to be copied from.
    //==========================================================================
    template <class TpRelatedPtr>
    XIRef<TPtr> &operator = ( IN const XIRef<TpRelatedPtr> &other )

    {
        // Get the raw pointer from the given object
        const TPtr *pConstPtr = Cast<const TPtr>(other.Raw());

        // Assign the new pointer to this object - adding a reference
        // in the assignment (We take the "const" off of the raw
        // pointer since we are not "logically" modifying the object)
        RawPtrAssign(const_cast<TPtr *>(pConstPtr));

        return *this;
    }

    //==========================================================================
    /// @brief  Reports a reference to the object's raw pointer.
    ///
    ///         This method reports a reference to the object's raw pointer. The
    ///         caller must be careful when doing this. Setting the pointer to
    ///         NULL or to some other value may result in memory leaks or worse.
    ///         \n\n
    ///         @note See the object's Clear() method.
    ///
    /// @return Returns a reference to the pointer maintained by the object.
    //==========================================================================
    TPtr*& Raw()
    {
        // Report a reference to the object's raw pointer
        return (_ptr);
    }

    //==========================================================================
    /// @brief  Reports a constant version of the object's raw pointer value.
    ///
    ///         This method is used to obtain a constant pointer to the object's
    ///         raw pointer.
    ///
    /// @return Reports a constant raw pointer value.
    //==========================================================================
    const TPtr* Raw() const
    {
        // Reports a reference to the current object
        return (_ptr);
    }

    //==========================================================================
    /// @brief  Returns a copy of the internal pointer
    ///
    /// @return Returns a copy of the internal pointer
    //==========================================================================
    TPtr* Get() const
    {
        // Report a reference to the object's raw pointer
        return _ptr;
    }

    //==========================================================================
    /// @brief  Reports a reference to a pointer to the counter object
    ///
    ///         This method is used to obtain a reference to the pointer to the object's
    ///         counter.
    ///
    /// @return Reports a referenece to a pointer to the counter object.
    //==========================================================================
    TPtr* GetCounter() const
    {
        return _ptr;
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
        return _ptr == NULL;
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
        return _ptr != NULL;
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
    XIRef<TPtr>& Clear()
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
    ///         the AutoRef object with another pointer.
    ///
    /// @param  ptr The pointer to compare the internal pointer to.
    ///
    /// @return true if the pointers are equal. false otherwise.
    //==========================================================================
    bool operator==(IN const TPtr* ptr) const
    {
        return _ptr == ptr;
    }

    //==========================================================================
    /// @brief  Equal comparison operator.
    ///
    ///         This method is used to compare the pointer value stored within
    ///         the AutoRef object with the pointer value stored in another
    ///         AutoRef object.
    ///
    /// @param  other A reference to the AutoRef object to compare against.
    ///
    /// @return true if the stored pointers are equal. false otherwise.
    //==========================================================================
    bool operator==(IN const XIRef<TPtr>& other) const
    {
        return _ptr == other.Raw();
    }

    //==========================================================================
    /// @brief  Not-equal comparison operator.
    ///
    ///         This method is used to compare the pointer value stored within
    ///         the AutoRef object with another pointer.
    ///
    /// @param  ptr The pointer to compare the internal pointer to.
    ///
    /// @return true if the pointers are @b not equal. false otherwise.
    //==========================================================================
    bool operator!=(IN const TPtr* ptr) const
    {
        return _ptr != ptr;
    }

    //==========================================================================
    /// @brief  Not-equal comparison operator.
    ///
    ///         This method is used to compare the pointer value stored within
    ///         the AutoRef object with the pointer value stored in another
    ///         AutoRef object.
    ///
    /// @param  other A reference to the AutoRef object to compare against.
    ///
    /// @return true if the stored pointers are @b not equal. false otherwise.
    //==========================================================================
    bool operator!=(IN const XIRef<TPtr>& other) const
    {
        return _ptr != other.Raw();
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

    //==========================================================================
    /// @brief  Releases the raw pointer held by the object.
    ///
    ///         This method is used to release any raw pointer currently held
    ///         by the object.
    //==========================================================================
    void RawPtrRelease()
    {
        // If the object does not contain a raw pointer, stop here
        if (_ptr == NULL)
            return;

        // Release the reference held on the raw pointer
        _ptr->Release();

        // Clear our raw pointer
        _ptr = NULL;
    }

    //==========================================================================
    /// @brief  Assigns a new raw pointer to the object.
    ///
    ///         This method is used to set the raw pointer for the object. If
    ///         the object already contains a raw pointer when this method is
    ///         called, the method will release the raw pointer before assigning
    ///         the new one. The raw pointer will have its reference count
    ///         incremented to reflect the pointer that will be stored within
    ///         the current object.
    ///
    /// @param  ptr This is the raw pointer that will be stored by the object.
    //==========================================================================
    void RawPtrAssign(IN TPtr *ptr)
    {
        // Release any raw pointer we already have
        RawPtrRelease();

        // If the given object is not empty, increment the reference count.
        if (ptr != NULL)
            ptr->AddRef();

        //  Assign the new raw pointer to our data member
        _ptr = ptr;
    }

    //==========================================================================
    /// This is the raw pointer the object is maintaining. The Release() method
    /// will be called through this pointer any time the object relinquished
    /// control of the pointer.
    //==========================================================================
    TPtr* _ptr;
};

#endif
