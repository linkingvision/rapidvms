
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
#include "XSDK/OS.h"

#include <errno.h>

using namespace XSDK;

XMutex::XMutex() :
    _lok()
{
    pthread_mutexattr_t mutexAttributes;
    int err = pthread_mutexattr_init( &mutexAttributes );
    if( err < 0 )
        X_THROW(( "Unable to allocate mutex attributes: %s", GetErrorMsg(err).c_str() ));

    err = pthread_mutexattr_settype( &mutexAttributes, PTHREAD_MUTEX_RECURSIVE );
    if( err < 0 )
        X_THROW(( "Unable to specify recursive mutex: %s", GetErrorMsg(err).c_str() ));

    err = pthread_mutex_init( &_lok, &mutexAttributes );

    if( err < 0 )
        X_THROW(( "Unable to allocate a mutex: %s", GetErrorMsg(err).c_str() ));
}

XMutex::~XMutex() throw()
{
    pthread_mutex_destroy( &_lok );
}

void XMutex::Acquire()
{
    int err = pthread_mutex_lock( &_lok );

    if( err != 0 )
        X_THROW(( "pthread_mutex_lock() failed: %s", GetErrorMsg(err).c_str() ));
}

void XMutex::Release()
{
    int err = pthread_mutex_unlock( &_lok );

    if( err != 0 )
        X_THROW(( "pthread_mutex_unlock() failed: %s", GetErrorMsg(err).c_str() ));
}

bool XMutex::TryAcquire()
{
    int err = pthread_mutex_trylock( &_lok );

    if( err == 0 )
        return true;

    if( err == EBUSY )
        return false;

    X_THROW(( "pthread_mutex_trylock() failed: %s", GetErrorMsg(err).c_str() ));
}
