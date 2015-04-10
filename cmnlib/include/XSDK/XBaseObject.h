
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_XSDK_OBJECT_H_
#define _XSDK_XSDK_OBJECT_H_

#include <string>
#include "XSDK/XSharedLib.h"
#include "XSDK/IXUnknown.h"
#include "XSDK/Module.h"
#include "XSDK/XUuid.h"
#include "XSDK/Errors.h"
#include "XSDK/IXWeakReference.h"
#include "XSDK/XMutex.h"


//=============================================================================
/// @brief Implements AddRef, Release, and GetClassId for objects that derive
/// from XBaseObject and will be using the reference counting methods.
///
/// @param classId - The UUID that identifies the derived class.
//=============================================================================
#define XSDK_DECLARE_IXUNKNOWN(classId)                                                      \
    virtual X_API int AddRef() const { return XSDK::XBaseObject::AddRef(); }   \
    virtual X_API int Release() const { return XSDK::XBaseObject::Release(); } \
    virtual X_API int GetReferenceCount() const { return XSDK::XBaseObject::GetReferenceCount(); } \
    virtual const X_API X_UUID GetClassId() const { return classId; }

//=============================================================================
/// @brief Implements AddRef, Release, and GetClassId for objects that derive
/// from XBaseObject that will be allocated ON THE STACK.
///
/// Some objects need to support interfaces but don't need to be reference
/// counted. These are called static objects, and their lifetime is controlled
/// by some other means. AddRef and Release will simply return 1 in these
/// objects.
///
/// @param classId - The UUID that identifies the derived class.
//=============================================================================
#define XSDK_DECLARE_IXUNKNOWN_STATIC(classId)                          \
    virtual X_API int AddRef() const { return 1; }            \
    virtual X_API int Release() const { return 1; }           \
    virtual X_API int GetReferenceCount() const { return 1; } \
    virtual const X_API X_UUID GetClassId() const { return classId; }

//=============================================================================
/// @brief Begins the implementation of QueryInterface for objects that derive
/// from XBaseObject.
///
/// @param moduleId - The module ID used when throwing exceptions.
///
/// Must be used with MPF_QUERY_INTERFACE_END
//=============================================================================
#define XSDK_QUERY_INTERFACE_BEGIN()                             \
        virtual X_API bool QueryInterface                                  \
        (                                                                   \
            IN const X_UUID& id,                                           \
            OUT void** ppInterface                                          \
        ) const                                \
        {                                                                   \
            if (ppInterface == 0)                                           \
            {                                                               \
                XSDK::XException ex("Null pointer passed to XBaseObject::QueryInterface"); \
                ex.SetThrowPoint(__LINE__, __FILE__);                       \
                throw ex;                                                   \
            }                                                               \
            XSDK::XUuid uuid(id);                                       \
            if (uuid == IXUnknown::Uuid())                                 \
                return XSDK::XBaseObject::QueryInterface(id, ppInterface);


//=============================================================================
/// @breif Adds support for a specific interface to QueryInterface.
///
/// @param interface - The interface. Ex. 'MPF::IFactory'
//=============================================================================
#define XSDK_QUERY_INTERFACE_ENTRY(interface)                \
            else if (uuid == interface::Uuid())                 \
            {                                                   \
                *ppInterface = (interface*)(this);              \
                XSDK::XBaseObject::AddRef();                \
                return true;                                    \
            }

//=============================================================================
/// @brief Completes the definition of QueryInterface.
///
/// @param parentObject - The direct parent object of the current object. The
///                       parent must derive from XBaseObject or BE
///                       XBaseObject.
//=============================================================================
#define XSDK_QUERY_INTERFACE_END(parentObject)                       \
            else                                                        \
                return parentObject::QueryInterface(id, ppInterface);   \
        }


namespace XSDK
{
    //=========================================================================
    /// Class ID for XBaseObject {7c32d18d-ba80-421b-ab06-209faaafd81f}
    //=========================================================================
    static const X_UUID kClsIdXBaseObject = {{0x7c,0x32,0xd1,0x8d,0xba,0x80,0x42,0x1b,0xab,0x06,0x20,0x9f,0xaa,0xaf,0xd8,0x1f}};

    //=========================================================================
    /// Provides a common implementation of IXUnknown methods.
    //=========================================================================
    class XBaseObject : public XSDK::IXWeakReference
    {
    public:

        //=====================================================================
        /// Construstor
        //=====================================================================
        X_API XBaseObject();

        //=====================================================================
        /// Constructor
        ///
        /// @param name - The name assinged to the object. This is used for
        ///               reference count debugging and should be the name of
        ///               the most derived class.
        //=====================================================================
        X_API XBaseObject(const std::string& name);
        virtual X_API ~XBaseObject();

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
            return (XSDK::kClsIdXBaseObject);
        }

        //=====================================================================
        // IXIWeakReference implementation
        //=====================================================================
        virtual X_API XIRef<XSDK::XObjectProxy> GetProxy();

        //=====================================================================
        /// Provides a quick way to get the IXUnknown pointer.
        ///
        /// Objects derived from BaseObject that implement other interfaces
        /// have an ambiguous path back to IXUnknown... BaseObject derives
        /// from it and the interfaces derive from it. This prevents the
        /// compiler from knowing with vtable entry for IXUnknown you want.
        /// Since we always want to provide the exact same pointer address for
        /// all instances of IXUnknown for a given object, we provide this
        /// method.
        //=====================================================================
        X_API XIRef<XSDK::IXUnknown> GetIXUnknown() const;

        //=====================================================================
        /// Gets the current reference count of the object.
        //=====================================================================
        virtual X_API int GetReferenceCount() const;

        //=====================================================================
        /// Sets the name of the object for reference count debugging.
        ///
        /// @param name - The name assigned to the object.
        //=====================================================================
        X_API void SetObjectName
        (
            IN const std::string& name
        );

        //=====================================================================
        /// Gets the name assigned to the object.
        ///
        /// @return The name assigned to the object.
        //=====================================================================
        X_API const std::string& GetObjectName();

    protected:

        //=====================================================================
        /// This method is called before delete is called on the object
        /// Deriving classes should override this method to add if there are any
        /// pre-destructor clean ups
        ///
        //=====================================================================
        X_API virtual void CleanUp();

    private:

        // The object is non-copyable.
        XBaseObject(const XSDK::XBaseObject&);
        XBaseObject& operator=(const XSDK::XBaseObject&);

        //=====================================================================
        // Member variables
        //=====================================================================
        mutable XSDK::XMutex      _refLock;   ///< Serializes ref count access.
        mutable volatile int      _refCount;  ///< Reference count.
        mutable volatile bool     _deleting;  ///< Object is currently being deleted.
        XSDK::XMutex              _coreLock;  ///< Serializes access to member variables.
        std::string               _className; ///< The name of the object instance.
        XIRef<XSDK::XObjectProxy> _pProxy;    ///< Pointer to the proxy object for "weak" refs.

    }; // XBaseObject

}; // XSDK

#endif // _XSDK_XSDK_OBJECT_H_
