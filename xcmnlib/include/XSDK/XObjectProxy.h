
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_X_OBJECT_PROXY_H_
#define _XSDK_X_OBJECT_PROXY_H_

#include "XSDK/IXUnknown.h"
#include "XSDK/XMutex.h"

namespace XSDK
{
    // Forward declarations
    class XSDKObject;

    //=========================================================================
    /// Class ID for XObjectProxy {8FC2BE4F-5A34-4bdb-A6BF-44264DA9994A}
    //=========================================================================
    static const X_UUID kClsIdXObjectProxy = {{0x8f,0xc2,0xbe,0x4f,0x5a,0x34,0x4b,0xdb,0xa6,0xbf,0x44,0x26,0x4d,0xa9,0x99,0x4a}};

    //=========================================================================
    /// Implements the object used by XIWeakRef to proxy access to ref-counted
    /// objects and prevent circular references.
    ///
    /// This object maintains a "weak" reference to an object that implements
    /// IXUnknown. The term "weak" reference means that the target object is
    /// NOT AddRef'd or Released. To prevent segfaults, the target object MUST
    /// notify the proxy when it goes away. This notification is done via the
    /// DetachObject() method. The target object should call this method when
    /// its reference count reaches 0 but before it deletes itself. This allows
    /// the proxy object to remain valid even though the target object has
    /// gone away. In this case, the proxy object is considered "orphaned" and
    /// will throw an exception if the target object pointer is accessed. The
    /// advantage of using the proxy is that the calling code won't segfault
    /// when attempting to access the target object after it has gone away.
    ///
    /// So, this begs the question, "Why did the target object go away? Can't
    /// we just AddRef it to keep it around?" This answer is "yes", in most
    /// cases, but there are exceptions where using a "weak" reference is
    /// required. The main use of "weak" references is to prevent circular
    /// references that prevents the object from even going away.
    ///
    /// For example, if object A holds a reference to object B, while B also
    /// holds a reference to object A, neither object will ever go away because
    /// neither of their reference counts can ever reach 0. To prevent this
    /// scenario, object A could hold a reference to B, while be only holds a
    /// "weak" reference to object A. The problem now is how does B know when
    /// A is no longer valid? This is where the proxy object comes in. It
    /// maintains communication with object A and gets notified when it goes
    /// away. Then, if object B attempts to call A through the proxy, an
    /// exception will be thrown instead of the application possibly
    /// segfaulting.
    //=========================================================================
    class XObjectProxy : public XSDK::IXUnknown
    {
    public:
        X_API XObjectProxy(IN void* pObject);
        X_API ~XObjectProxy() throw();

        //=====================================================================
        // IXUnknown implementation
        //=====================================================================
        virtual X_API int AddRef() const;
        virtual X_API int Release() const;
        virtual X_API bool  QueryInterface
        (
         IN  const X_UUID& id,
         OUT void** ppInterface
        )
        const;
        virtual X_API const X_UUID GetClassId() const
        {
            return (XSDK::kClsIdXObjectProxy);
        }

        //=====================================================================
        /// Gets the current reference count of the object.
        //=====================================================================
        virtual X_API int GetReferenceCount() const;

        //=====================================================================
        /// Notifies the proxy that the target object is being destroyed.
        //=====================================================================
        X_API void DetachObject();

        //=====================================================================
        /// Indicates if the target object has been destroyed.
        //=====================================================================
        X_API bool IsOrphaned() const;

        //=====================================================================
        /// Returns a pointer to the proxied object.
        ///
        /// @throw E_ORPHANED_OBJECT - The proxied object has been destroyed.
        //=====================================================================
        X_API void* GetObjectPtr() const;

    private:

        // The object is non-copyable.
        XObjectProxy(const XSDK::XObjectProxy&);
        XObjectProxy& operator=(const XSDK::XObjectProxy&);


        mutable XSDK::XMutex _coreLock;  ///< Serializes access to member variables.
        mutable int             _refCount;  ///< Reference count.
        void*                   _pObject;   ///< Pointer to the proxied object.

    }; // XObjectProxy

} // XSDK

#endif // _XSDK_X_OBJECT_PROXY_H_
