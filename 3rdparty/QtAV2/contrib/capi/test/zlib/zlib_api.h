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

#ifndef ZLIB_API_H
#define ZLIB_API_H

// no need to include the C header if only functions declared there
extern "C" {
#include "zlib.h"
}

namespace zlib {
class api_dll;
class api
{
public:
    api();
    virtual ~api();
    virtual bool loaded() const;
    const char* zlibVersion();
    const char* zError(int);
private:
    api_dll *dll;
};
} //namespace zlib


#endif // ZLIB_API_H
