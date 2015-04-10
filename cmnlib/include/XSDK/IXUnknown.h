
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_IX_UNKNOWN_H_
#define _XSDK_IX_UNKNOWN_H_

#include <typeinfo>
#include "XSDK/Types.h"
#include "XSDK/IXInterface.h"
#include "XSDK/XIRef.h"
#include "XSDK/Errors.h"

namespace XSDK
{

//==============================================================================
/// @brief Defines methods for managing references to objects.
///
/// IXUnknown is the parent interface from which all other externally-facing
/// interfaces <b>must</b> derive. By 'externally-facing', we mean all
/// interfaces that are passed between software modules written by different
/// groups. The purpose of IXUnknown is to ensure a minimal set of methods that
/// are guarateed to exist on any interface. These methods allow a module to
/// control the lifetime of an interface, and query the underlying object or
/// other interfaces it implements (via QueryInterface). In addition, IXUnknown
/// defines the GetClassId method which allows the caller to identify the
/// concrete object implementing the current interface.
///
/// <b>Reference Counting</b>
///
/// The lifetime of objects <i>that implement interfaces derived from IXUnknown
/// </i> are controlled by reference counting, similar to Microsoft's Component
/// Object Model (COM). A reference count is simply a counter kept by the object
/// that is incremented (by a call to AddRef) each time a new entity (such as
/// another object, module, thread, etc) wants to use an interface provided by
/// the object. When a reference is added to an object, the entity that added
/// it is said to 'own' a reference. Each reference that an entity 'owns' must
/// subsequently be released (via a call to Release) before the underlying
/// object can be destroyed. Calling Release, however, does not guarantee that
/// the object will immediately be destroyed, as other entities may also 'own'
/// references to the object. An object implementing IXUnknown will only be dictates
/// destroyed when <b>ALL</b> references to it have been released (i.e. it's
/// internal reference count reached 0). In this way, an entity can be
/// guaranteed that a reference counted object will always exist as long as it
/// 'owns' a reference to it.
///
/// <b>IXUnknown as the Root Interface</b>
///
/// As stated above, all externally-facing interfaces must derive from
/// IXUnknown in some way (like deriving from an interface that itself derives
/// from IXUnknown). The reason for this is to ensure that all interfaces have
/// the IXUnknown methods available to external modules. By deriving from
/// IXUnknown, an interface will inherit the AddRef, Release, and
/// QueryInterface methods. This is required for reference counting to work
/// properly.
///
/// <b>Implementing IXUnknown</b>
///
/// The very nature of reference counting requires that multiple entities can
/// affect the value of the internal reference count at any time. This means
/// that all calls to AddRef and Release must be thread-safe to prevent the
/// count from being set incorrectly. This can be achieved by using a lock to
/// protect the counter value.
///
/// <b>NOTE:</b> A default implementation of the IXUnknown methods is provided
/// by the XSDK package. Implementers should use this implementation unless
/// there is very good reason not to. See the documentation for XUnknown for
/// more information.
//==============================================================================

//==============================================================================
//Forward declaration of XException
//==============================================================================
class XException;

class IXUnknown
{
    //==========================================================================
    /// @brief Interface library identifier including Destructor.
    ///
    /// UUID value: 93cb9993-674e-4736-8f44-46714f4b89fd
    //==========================================================================
    DEFINE_INTERFACE_ID(IXUnknown, 0x93,0xcb,0x99,0x93,0x67,0x4e,0x47,0x36,0x8f,0x44,0x46,0x71,0x4f,0x4b,0x89,0xfd)

    //==========================================================================
    /// @brief  Increments the reference count.
    ///
    ///         This method adds a reference count to the implementation object.
    ///         Each reference count added represents one reference (i.e.,
    ///         pointer if you will) held on the implementing object. The object
    ///         must remain in existence for at least as long as the reference
    ///         count exists as part of the total reference counts added to the
    ///         object. It is the callers responsibility to call the interfaces'
    ///         Release() method to remove the reference count once access to
    ///         the object is no longer needed.
    ///
    ///         @note Please see the XIRef<> class for automatic handling of
    ///         references. The XIRef<> class is particularly helpful in
    ///         scenarios where exceptions can be thrown and references can
    ///         accidentally be lost track of (often times resulting in memory
    ///         leaks).
    ///
    /// @return This method returns the number of references held on the object
    ///         after the increment has been applied. This value is primarily
    ///         helpful when debugging reference count related issues.
    //==========================================================================
    virtual int AddRef() const = 0;

    //==========================================================================
    /// @brief  Decrements the reference count. When the count reaches 0, the
    //         underlying object deletes itself.
    ///
    ///         This method is used to release a reference count added to the
    ///         implementation object. The method must only be called if the
    ///         caller has added the reference count using the interface's
    ///         AddRef() method. Once the last reference count added by the
    ///         caller has been released, the caller must not attempt to access
    ///         the object again (i.e., the object may have been destroyed and
    ///         accessing it will likely crash the program).
    ///
    /// @return This method returns the number of references held on the object
    ///         after the decrement has been applied. This value is primarily
    ///         helpful when debugging reference count related issues.
    //==========================================================================
    virtual int Release() const = 0;


    virtual X_API int GetReferenceCount() const = 0;

    //==========================================================================
    /// @brief     Queries the object for a given interface.
    ///
    ///            This method queries the implementing object to see if it
    ///            implements the specified interface. If such an interface is
    ///            supported, the method reports the interface to the caller.
    ///
    /// @note      This method will call AddRef() on the interface before it
    ///            is returned to the caller. It is the caller's responsibility
    ///            to call Release() on the interface when it is no longer
    ///            needed.
    ///
    /// @param     id The UUID of the interface to query for.
    ///
    /// @param     ppInterface The address of the variable that will receive
    ///            a pointer to the requested interface. A reference will be
    ///            added to the object before it is returned.
    ///
    /// @return    true if the object supports the specified interface. false
    ///            otherwise. If false is returned, then ppInterface will point
    ///            to NULL.
    ///
    /// @exception Throws a XException object. This allows tearoff interfaces
    ///            to be used. Exceptions will only be thrown for conditions
    ///            such as not being able to allocate a child object, etc.
    ///            Otherwise, a simple boolean result is returned.
    //==========================================================================
    virtual bool QueryInterface(IN const X_UUID& id, OUT void** ppInterface) const = 0;

    //==========================================================================
    /// @brief  Returns the UUID that identifies the class type of the
    ///         underlying object.
    ///
    ///         All objects that implement this interface (or any interface
    ///         derived from this one) should identify itself using a unique
    ///         UUID value. Use the DEFINE_CLASS_ID macro to specify this ID.
    ///
    /// @return The UUID that identifies the class type.
    //==========================================================================
    virtual const X_UUID GetClassId() const = 0;

    //==========================================================================
    /// @brief  Returns an autoref containing the desired interface.
    ///
    ///         This method is intended to be used in situations where it is
    ///         known that the underlying object supports the interface. If you
    ///         aren't sure, then you should use QueryInterface.
    ///
    ///         USAGE: XIRef<IBar> pBar = pFoo->GetInterface<IBar>();
    ///
    /// @return A XIRef containing the desired interface.
    //==========================================================================
    template <class T> XIRef<T> GetInterface() const
    {
        XIRef<T> p = const_cast<T*>( dynamic_cast<const T*>( this ) );

        if( p.IsEmpty() )
        {
            X_THROW_CRITICAL( ( "IXUnknown::GetInterface couldn't cast to the specified interface, actual:(%s).",
                                typeid(this).name() ) );
        }

        return p;
    }

    //==========================================================================
    /// @brief  Returns true if *this has an "is a" relationship with T.
    /// @return bool
    //==========================================================================
    template <class T> bool IsA() const
    {
        return (dynamic_cast<const T*>( this )) ? true : false;
    }

    //==========================================================================
    /// @brief  Returns true if *this has an "is a" relationship with T.
    /// @return bool
    //==========================================================================
    template <class T> bool IsA( XIRef<T>& ref ) const
    {
        T* p = const_cast<T*>( dynamic_cast<const T*>( this ) );

        if( p )
            ref = p;

        return (p) ? true : false;
    }

    //==========================================================================
    /// @brief  Returns an autoref of the object casted to T.
    /// @return An XIRef containing the desired type
    //==========================================================================
    template <class T> XIRef<T> GetAs() const
    {
        XIRef<T> p = const_cast<T*>( dynamic_cast<const T*>( this ) );

        if( p.IsEmpty() )
        {
            X_THROW_CRITICAL( ("IXUnknown::GetAs couldn't cast to the specified interface, actual:(%s).",
                               typeid(this).name()) );
        }

        return p;
    }
};

}; // namespace XSDK



#endif // _XSDK_IX_UNKNOWN_H_

