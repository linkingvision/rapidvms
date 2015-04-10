
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XObjCount_h_
#define XSDK_XObjCount_h_

#include <map>
#include <string>
#include "XSDK/XMutex.h"

namespace XSDK
{
    class XObjCount
    {
    public:

#ifdef COUNT_REFS
        //Keeps track of then number of each object type which exist.
        X_API static void AddObj(const std::string& typeName);
        X_API static void RemoveObj(const std::string& typeName);
#endif

#ifdef TRACK_STACK
        //Keeps track of individual ptr's and the stack trace of where they were created.
        X_API static void AddStack(const void* ptr);
        X_API static void RemoveStack(const void* ptr);
#endif

        X_API static std::map<std::string, size_t> GetObjCount();

        // Expensive
        X_API static std::map<const void*, std::string> GetStacks();

    private:

        XObjCount() : _objLock(), _objCount(), _stackTracker() {}
        ~XObjCount();

        //Disallow
        XObjCount(XObjCount&);
        void operator=(XObjCount&);

        XSDK::XMutex _objLock;
        std::map<std::string, size_t> _objCount;

        //ptr and stack trace for that ptr's creation.
        std::map<const void*, std::string> _stackTracker;

        static XObjCount _cInstance;
    };
}

#endif
