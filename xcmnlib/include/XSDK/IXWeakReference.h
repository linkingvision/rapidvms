
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_IX_WEAK_REFERENCE_H_
#define _XSDK_IX_WEAK_REFERENCE_H_

#include "XSDK/Types.h"
#include "XSDK/XInterface.h"
#include "XSDK/IXUnknown.h"
#include "XSDK/XException.h"
#include "XSDK/XObjectProxy.h"

namespace XSDK
{
    //=========================================================================
    /// This interface is required by any object that will be put into a
    /// XIWeakRef.
    //=========================================================================
    class IXWeakReference : public XSDK::IXUnknown
    {
        //=====================================================================
        /// Interface library identifier.
        ///
        /// UUID value: 51616B31-2135-4505-9AB3-6358C8DD50A5
        //=====================================================================
        DEFINE_INTERFACE_ID(IXWeakReference,0x51,0x61,0x6b,0x31,0x21,0x35,
            0x45,0x05,0x9a,0xb3,0x63,0x58,0xc8,0xdd,0x50,0xa5);

        //=====================================================================
        /// Returns a pointer to an instance of the proxy object for the class.
        ///
        /// @return A XIRef-wrapped pointer to the proxy instance.
        //=====================================================================
        virtual XIRef<XSDK::XObjectProxy> GetProxy() = 0;
    };
}; // namespace XSDK

#endif // _XSDK_IX_WEAK_REFERENCE_H_

