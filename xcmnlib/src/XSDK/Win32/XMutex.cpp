
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XMutex.h"
#include "XSDK/Errors.h"

using namespace XSDK;

XMutex::XMutex()
: _lokCount(0)
{
    InitializeCriticalSection( &_lok );
}

XMutex::~XMutex() throw()
{
    DeleteCriticalSection( &_lok );
}

void XMutex::Acquire()
{
    EnterCriticalSection( &_lok );
    _lokCount++;
}

void XMutex::Release()
{
    if( (_lokCount - 1) < 0 )
        X_THROW(( "Attempted to Leave already left critical section." ));
    _lokCount--;
    LeaveCriticalSection( &_lok );
}

bool XMutex::TryAcquire()
{
    BOOL entered = TryEnterCriticalSection( &_lok );

    if( entered )
    {
        _lokCount++;
        return true;
    }

    return false;
}
