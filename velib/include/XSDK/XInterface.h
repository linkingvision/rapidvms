
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_intERFACE_H_
#define _XSDK_intERFACE_H_

#include "XSDK/XSharedLibImp.h"
#include "XSDK/Types.h"
#include "XSDK/XSharedLibDef.h"
#include "XSDK/Module.h"
#include "XSDK/Errors.h"

namespace XSDK
{

//=============================================================================
/// @brief Utility routine used to query for an interface.
///
/// This routine is used to query an interface/object for an interface.
/// The function accepts a pointer to an object expected to implement a
/// QueryInterface() method (this method is also expected to conform to
/// the semantics of the IXUnknown::QueryInterface() method - adding a
/// reference to the returned interface, always returning the same value
/// for IXUnknown, etc.). Unlike the similar XGi() function, this
/// routine does not throw if the requested interface is not available.
///
/// @param  pIn This is the interface/object to be queried. The pointer
///     should never be NULL.
///
/// @return If the queried for interface (determined by IFACE_OUT::Uuid())
///     is defined and available, the method will return a IFACE_OUT type
///     pointer (a reference count will have been added and the caller is
///     responsible for releasing it when the interface is no longer needed).
///     If the interface is not available, the method will report a NULL
///     pointer value.
///
/// @exception XException (Thrown only if the underlying QueryInterface()
///     throws or an invalid pointer is passed in - does not throw if the
///     requested interface does not exist.)
//=============================================================================
template <class IFACE_OUT, class TYPE_IN>
inline IFACE_OUT* XQi
(
    IN TYPE_IN* pIn
)

{
    // Should always be given a non-NULL pointer
    X_ASSERT(pIn != NULL);

    // Report an exception if we are given an invalid pointer
    if (pIn == NULL)
    {
        X_THROW (("Invalid"
            " NULL pointer given for the type to get an interface from"));
    }

    // Attempt to get the interface being queried for
    IFACE_OUT* piResult = NULL;
    bool bResult = pIn->QueryInterface(IFACE_OUT::
        XUuid(), reinterpret_cast<void**>(&piResult));

    // If we obtained the queried for interface, return it -
    // return a NULL pointer if the interface was not available
    return ((bResult)? piResult : NULL);
}


//=============================================================================
/// @brief Utility routine used to query for an interface.
///
/// This function is used to obtain an interface from an interface/object.
/// The function accepts a pointer to an object expected to implement a
/// QueryInterface() method (this method is also expected to conform to
/// the semantics of the IXUnknown::QueryInterface() method - adding a
/// reference to the returned interface, always returning the same value
/// for IXUnknown, etc.). Unlike the similar XQi() function, this
/// function will throw an exception if the interface being queried for
/// is not available.
///
/// @param  pIn This is the interface/object to be queried. The pointer
///     should never be NULL.
///
/// @return If the queried for interface (determined by IFACE_OUT::Uuid())
///     is defined and available, the method will return a IFACE_OUT type
///     pointer (a reference count will have been added and the caller is
///     responsible for releasing it when the interface is no longer needed).
///     If the interface is not available, the method will throw.
///
/// @exception XException (Thrown if the underlying QueryInterface()
///     throws, a NULL pointer is given or if the queried for interface
///     is not available).
//=============================================================================
template <class IFACE_OUT, class TYPE_IN>
inline IFACE_OUT* XGi
(
    IN TYPE_IN* pIn
)

{
    // Should always be given a non-NULL pointer
    X_ASSERT(pIn != NULL);

    // Attempt to get the requested interface from the given interface/object
    IFACE_OUT* piResult = XQi<IFACE_OUT, TYPE_IN>(pIn);

    // If we have an interface to report, do so now
    if (piResult != NULL)
        return (piResult);

    // Throw if we get here - the interface is not available
    X_THROW (("An unsupported interface has been requested"));
}

} // XSDK

#endif // _XSDK_intERFACE_H_

