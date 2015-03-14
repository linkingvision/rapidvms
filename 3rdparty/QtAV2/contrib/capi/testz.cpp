/******************************************************************************
    An example to show how to use C API in C++ dynamically and no link
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

#include <QtCore/QLibrary>
// no need to include the C header if only functions declared there
extern "C" {
#include "zlib.h"
}
#ifndef Q_FUNC_INFO
#define Q_FUNC_INFO __FUNCTION__
#endif

/*
 * base ctor dll_helper("name")=>derived members in decl order(resolvers)=>derived ctor
 */
class dll_helper {
public:
    dll_helper(const QString& soname) {
        m_lib.setFileName(soname);
        if (m_lib.load())
            qDebug("%s loaded", m_lib.fileName().toUtf8().constData());
        else
            qDebug("can not load %s: %s", m_lib.fileName().toUtf8().constData(), m_lib.errorString().toUtf8().constData());
    }
    virtual ~dll_helper() { m_lib.unload();}
    bool isLoaded() const { return m_lib.isLoaded(); }
    void* resolve(const char *symbol) { return (void*)m_lib.resolve(symbol);}
private:
    QLibrary m_lib;
};

/*!
 * \brief The zlib_dll class
 * dll loader and symbol resolver.
 * It's designed to easily to use macros to simplify the code
 */
class zlib_dll : public dll_helper {
public:
    zlib_dll()
#ifdef Q_OS_WIN
        : dll_helper("zlib")
#else
        : dll_helper("z")
#endif
    { qDebug("dll symbols resolved...");}
    // can use 1 macro here for each zlib function
public:
    typedef const char* zlibVersion_t();
    zlibVersion_t* zlibVersion;
private:
    class zlibVersion_resolver_t {
    public:
        zlibVersion_resolver_t() {
            // nested class can not call non-static members outside the class, so hack the address here
            // need -Wno-invalid-offsetof
            const qptrdiff diff = qptrdiff(&((zlib_dll*)0)->zlibVersion_resolver) - qptrdiff(&((zlib_dll*)0)->zlibVersion);
            zlibVersion_t **p = (zlibVersion_t**)((quint8*)this - diff);
            zlib_dll* dll = (zlib_dll*)((quint8*)this - ((qptrdiff)(&((zlib_dll*)0)->zlibVersion_resolver)));
            if (!dll->isLoaded()) {
                qWarning("dll not loaded %s @%d", __FUNCTION__, __LINE__);
                *p = 0;
                return;
            }
            *p = (zlibVersion_t*)dll->resolve("zlibVersion");
            qDebug("zlib_dll::zlibVersion: %p", *p);
        }
    } zlibVersion_resolver;
public:
    typedef uLong zlibCompileFlags_t();
    zlibCompileFlags_t* zlibCompileFlags;
private:
    class zlibCompileFlags_resolver_t {
    public:
        zlibCompileFlags_resolver_t() {
            const qptrdiff diff = qptrdiff(&((zlib_dll*)0)->zlibCompileFlags_resolver) - qptrdiff(&((zlib_dll*)0)->zlibCompileFlags);
            zlibCompileFlags_t **p = (zlibCompileFlags_t**)((quint8*)this - diff);
            zlib_dll* dll = (zlib_dll*)((quint8*)this - ((qptrdiff)(&((zlib_dll*)0)->zlibCompileFlags_resolver)));
            if (!dll->isLoaded()) {
                qWarning("dll not loaded %s @%d", __FUNCTION__, __LINE__);
                *p = 0;
                return;
            }
            *p = (zlibCompileFlags_t*)dll->resolve("zlibCompileFlags");
            qDebug("zlib_dll::zlibCompileFlags: %p", *p);
        }
    } zlibCompileFlags_resolver;
};

/*!
 * \brief The zlib_api class
 * Any class calling zlib functions inherits this. The dynamically loaded symbols will be called
 */
class zlib_api {
public:
    zlib_api() : dll(new zlib_dll()) {}
    const char* zlibVersion() {
        qDebug("%s @%d", Q_FUNC_INFO, __LINE__);
        Q_ASSERT(dll);
        return dll->zlibVersion();
    }
    uLong zlibCompileFlags() {
        qDebug("%s @%d", Q_FUNC_INFO, __LINE__);
        Q_ASSERT(dll);
        return dll->zlibCompileFlags();
    }
private:
    zlib_dll *dll;
};

class test_zlib_api : public zlib_api {
public:
    void test_version() {
        qDebug("START %s", __FUNCTION__);
        qDebug("zlib version: %s", zlibVersion());
        qDebug("STOP %s", __FUNCTION__);
    }
    void test_compileFlags() {
        qDebug("START %s", __FUNCTION__);
        qDebug("zlib compile flags=%lu", zlibCompileFlags());
        qDebug("STOP %s", __FUNCTION__);
    }
};

int main(int argc, char *argv[])
{
    test_zlib_api tt;
    tt.test_version();
    tt.test_compileFlags();
    return 0;
}

