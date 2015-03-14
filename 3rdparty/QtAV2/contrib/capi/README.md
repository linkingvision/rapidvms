# capi

This header only library helps you use C api in a shared library by dynamically loading instead of linking to it with minimal efforts.

Here is a simple zlib example, if you want use zlib functions, inherits class zlib::api.

>define a class with zlib functions in zlib_api.h

    extern "C" {
    #include "zlib.h" // we need some types define there. otherwise we can remove this
    }
    namespace zlib { //need a unique namespace
    class api_dll; //must use this name
    class api //must use this name
    {
        api_dll *dll;
    public:
        api();
        virtual ~api();
        const char* zlibVersion();
        const char* zError(int);
        // and more functions
    };
    } //namespace zlib

`zlib_api.h` is the header you will use

>zlib_api.cpp (some code can be generated from  tools/mkapi)

    #include "zlib_api.h"
    #include "capi.h"
    #include <QLibrary> // need a library loader/resolver class whose function names like QLibrary
    namespace zlib {
    static const char* zlib[] = { "zlib", "z", NULL}; //zlib.dll, libz.so, libz.dylib
    CAPI_BEGIN_DLL(zlib, QLibrary)
    CAPI_DEFINE_RESOLVER(const char*, zlibVersion, CAPI_ARG0())
    CAPI_DEFINE_RESOLVER(const char*, zError, CAPI_ARG1(int))
    // and more functions
    CAPI_END_DLL()
    CAPI_DEFINE(const char*, zlibVersion, CAPI_ARG0())
    CAPI_DEFINE(uLong, zError, CAPI_ARG1(int))
    api::api() : dll(new api_dll()) {}
    api::~api() { delete dll;}
    // and more functions
    } //namespace zlib

>test.cpp (dynamically loaded symbols, not link to zlib):

    #include "zlib_api.h"
    class test_zlib_api : public zlib::api {
    public:
        void test_version() {
            printf("zlib version: %s\n", zlibVersion());
        }
        void test_error(int errorCode) {
            printf("zlib error: %d => %s\n", errorCode, zError(errorCode));
        }
    };
    int main(int, char **)
    {
        test_zlib_api tt;
        tt.test_version();
        tt.test_error(1);
        return 0;
    }

### Auto Code Generation

There is a tool to help you generate header and source: https://github.com/wang-bin/mkapi

The tool is based on clang 3.4.

Run `make` to build the tool then run `./mkapi.sh -name zlib zlib.h` to generate zlib_api.h and zlib_api.cpp.
