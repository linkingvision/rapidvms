
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/OS/Rand.h"
#include "XSDK/XGuard.h"
#include <cstdlib>

using namespace std;
using namespace XSDK;

static XMutex& GetMutex()
{
    static XMutex mutex;
    return mutex;
}

static bool& GetRandSeeded()
{
    static bool seeded;
    return seeded;
}

int XSDK::x_rand()
{
    XGuard lock(GetMutex());
    return rand();
}

void XSDK::x_srand(unsigned int seed)
{
    XGuard lock(GetMutex());
    srand(seed);
    GetRandSeeded() = true;
}

void XSDK::x_srand_if(unsigned int seed)
{
    XGuard lock(GetMutex());
    if(!GetRandSeeded())
        x_srand(seed);
}
