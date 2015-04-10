
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XGuard.h"

using namespace XSDK;

XGuard::XGuard( XMutex& lok ) :
    _lok( lok ),
    _lokCount( 0 )
{
    _lok.Acquire();
    _lokCount++;
}

XGuard::~XGuard() throw()
{
    if( _lokCount )
    {
        _lokCount--;
        _lok.Release();
    }
}

void XGuard::Acquire()
{
    if( !_lokCount )
    {
        _lok.Acquire();
        _lokCount++;
    }
}

void XGuard::Release()
{
    if( _lokCount )
    {
        _lokCount--;
        _lok.Release();
    }
}








