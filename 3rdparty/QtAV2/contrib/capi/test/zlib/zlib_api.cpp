/******************************************************************************
    An example to show how to use CAPI
    Copyright (C) 2014 Wang Bin <wbsecg1@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
******************************************************************************/

#define DEBUG
#include "zlib_api.h"
#include "capi.h"
#include <QtCore/QLibrary>

namespace zlib {
static const char* zlib[] = {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
    "zlib",
#else
    "z",
#endif
    NULL
};
static const int versions[] = { capi::NoVersion, 1, 0, capi::EndVersion };
CAPI_BEGIN_DLL_VER(zlib, versions, QLibrary)
CAPI_DEFINE_RESOLVER(const char*, zlibVersion, CAPI_ARG0())
CAPI_DEFINE_RESOLVER(const char*, zError, CAPI_ARG1(int))
CAPI_END_DLL()
CAPI_DEFINE(const char*, zlibVersion, CAPI_ARG0())
CAPI_DEFINE(const char*, zError, CAPI_ARG1(int))

api::api() : dll(new api_dll()) {}
api::~api() { delete dll;}
bool api::loaded() const { return dll->isLoaded();}
} //namespace zlib
