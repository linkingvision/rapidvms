
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <typeinfo>
#include "XSDK/XObjCount.h"
#include "XSDK/XException.h"
#include "XSDK/XGuard.h"
#include "XSDK/XMutex.h"
#include "XSDK/XStackTrace.h"

using namespace std;
using namespace XSDK;

XObjCount XObjCount::_cInstance;


XObjCount::~XObjCount()
{
#ifdef COUNT_REFS
    if(!_objCount.empty())
    {
        printf("*****************************\n");
        string msg = "There's at least one static variable which needs to be moved into a function in order to ";
        msg += "ensure that it's not constructed before XObjCount:";
        printf("%s\n", msg.c_str());

        for(map<string, size_t>::iterator iter = _objCount.begin(),
                                          end  = _objCount.end();
            iter != end;
            ++iter)
        {
            printf("%lu of %s still exist when XObjCount is destroyed.\n", iter->second, iter->first.c_str());
        }
        printf("*****************************\n");
    }
#endif

#ifdef TRACK_STACK
    if(!_stackTracker.empty())
    {
        printf("*****************************\n");
        string msg = "There's at least one static variable which needs to be moved into a function in order to ";
        msg += "ensure that it's not constructed before XObjCount:";
        printf("%s\n", msg.c_str());

        for(map<const void*, string>::iterator iter = _stackTracker.begin(),
                                         end  = _stackTracker.end();
            iter != end;
            ++iter)
        {
            printf("%p of %s still exists when XObjCount is destroyed:\n%s\n",
                   iter->first, typeid(iter->first).name(), iter->second.c_str());
        }
        printf("*****************************\n");
    }
#endif
}


#ifdef COUNT_REFS
void XObjCount::AddObj(const string& typeName)
{
    XGuard lock(_cInstance._objLock);
    map<string, size_t>::iterator found = _cInstance._objCount.find(typeName);
    if(found == _cInstance._objCount.end())
        _cInstance._objCount.insert(make_pair(typeName, 1));
    else
        ++found->second;
}


void XObjCount::RemoveObj(const string& typeName)
{
    XGuard lock(_cInstance._objLock);
    map<string, size_t>::iterator found = _cInstance._objCount.find(typeName);

    if(found != _cInstance._objCount.end())
    {
        if(--found->second == 0)
            _cInstance._objCount.erase(found);
    }
    else
    {
        printf("Tried to remove [%s] when there were none in the hash of object types.\n", typeName.c_str());
        XLog::LogBacktrace();
        //X_ASSERT(false && "Ref counting with XObjCount is buggy.");
    }
}
#endif

map<string, size_t> XObjCount::GetObjCount()
{
    XGuard lock(_cInstance._objLock);
    return _cInstance._objCount;
}


#ifdef TRACK_STACK
void XObjCount::AddStack(const void* ptr)
{
    XGuard lock(_cInstance._objLock);
    _cInstance._stackTracker.insert(make_pair(ptr, XStackTrace::GetStack()));
}


void XObjCount::RemoveStack(const void* ptr)
{
    XGuard lock(_cInstance._objLock);
    map<const void*, string>::iterator found = _cInstance._stackTracker.find(ptr);

    if(found != _cInstance._stackTracker.end())
        _cInstance._stackTracker.erase(found);
    else
    {
        printf("Tried to remove [%p] of type [%s] when it's not being tracked.\n", ptr, typeid(ptr).name());
        XLog::LogBacktrace();
        //X_ASSERT(false && "Stack tracking with XObjCount is buggy.");
    }
}
#endif


std::map<const void*, std::string> XObjCount::GetStacks()
{
    XGuard lock(_cInstance._objLock);
    return _cInstance._stackTracker;
}
