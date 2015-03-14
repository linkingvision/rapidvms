/******************************************************************************
    Use C API in C++ dynamically and no link. Header only.
    Use it with a code generation tool: https://github.com/wang-bin/mkapi
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
// no class based implemention: https://github.com/wang-bin/dllapi . limitation: can not reload library
#ifndef CAPI_H
#define CAPI_H

#include <cstddef> //ptrdiff_t
#include <cstdio>
#include <cassert>

namespace capi {
namespace version {
    enum {
        Major = 0,
        Minor = 0,
        Patch = 2,
        Value = ((Major&0xff)<<16) | ((Minor&0xff)<<8) | (Patch&0xff)
    };
    static const char name[] = { Major + '0', '.', Minor + '0', '.', Patch + '0', 0 };
    // DO NOT use macro here because it's included outside, the "build" will change
    //CAPI_EXPORT const char* build(); //header only because of template, so build() is useless
} //namespace version
// set lib name with version
enum {
    NoVersion = -1,
    EndVersion = -2
};
} //namespace capi
/********************************** The following code is only used in .cpp **************************************************/
/*!
  * compiler may not support init list {a, b, c}
  * static const char* zlib[] = {
  * #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
  *   "zlib",
  * #else
  *   "z",
  * #endif
  *   NULL};
  * CAPI_BEGIN_DLL(zlib, QLibrary) // the 2nd parameter 'QLibrary' is a library loading class. \sa dll_helper class
  * ...
  */
/// DLL_CLASS is a library loading and symbols resolving class like Qt's QLibrary. Must implement api:
/// void setFileName(const char*); setFileNameAndVersion(const char* name, int ver);
/// bool load(); bool unload(); bool isLoaded() const;
/// void* resolve(const char* symbol);
#define CAPI_BEGIN_DLL(names, DLL_CLASS) \
    class api_dll : public capi::internal::dll_helper<DLL_CLASS> { \
    public: api_dll() : capi::internal::dll_helper<DLL_CLASS>(names) { CAPI_DBG_RESOLVE("capi resolved dll symbols...");}
/*!
  also defines possible library versions to be use. capi::NoVersion means no version suffix is used,
  e.g. libz.so. Versions array MUST be end with capi::EndVersion;
  below is an example to open libz.so, libz.so.1, libz.so.0 on unix
  static const int ver[] = { capi::NoVersion, 1, 0, capi::EndVersion };
  CAPI_BEGIN_DLL_VER(zlib, ver)
  ...
 */
#define CAPI_BEGIN_DLL_VER(names, versions, DLL_CLASS) \
    class api_dll : public capi::internal::dll_helper<DLL_CLASS> { \
    public: api_dll() : capi::internal::dll_helper<DLL_CLASS>(names, versions) { CAPI_DBG_RESOLVE("capi resolved dll symbols...");}

#define CAPI_END_DLL() };

/*!
 * N: number of arguments
 * R: return type
 * name: api name
 * ...: api arguments with only types, wrapped by CAPI_ARGn, n=0,1,2,...13
 * The symbol of the api is "name". Otherwise, use CAPI_DEFINE instead.
 * example:
 * 1. const char* zlibVersion()
 *    CAPI_DEFINE(const char* zlibVersion, CAPI_ARG0())
 * 2. const char* zError(int) // get error string from zlib error code
 *    CAPI_DEFINE(const char*, zError, CAPI_ARG1(int))
 */
#define CAPI_DEFINE(R, name, ...) EXPAND(CAPI_DEFINE_X(R, name, __VA_ARGS__)) /* not ##__VA_ARGS__ !*/
#define CAPI_DEFINE_RESOLVER(R, name, ...) EXPAND(CAPI_DEFINE_RESOLVER_X(R, name, name, __VA_ARGS__))
#define CAPI_DEFINE_M_RESOLVER(R, M, name, ...) EXPAND(CAPI_DEFINE_M_RESOLVER_X(R, M, name, name, __VA_ARGS__))
//EXPAND(CAPI_DEFINE##N(R, name, #name, __VA_ARGS__))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************The followings are used internally**********/
#define CAPI_DEFINE_T_V(R, name, ARG_T, ARG_T_V, ARG_V) \
    R api::name ARG_T_V { \
        CAPI_DBG_CALL(" "); \
        assert(dll && dll->isLoaded()); \
        return dll->name ARG_V; \
    }
// nested class can not call non-static members outside the class, so hack the address here
// need -Wno-invalid-offsetof
#define CAPI_DEFINE_M_RESOLVER_T_V(R, M, name, sym, ARG_T, ARG_T_V, ARG_V) \
    public: \
        typedef R (M *name##_t) ARG_T; \
        name##_t name; \
    private: \
        struct name##_resolver_t { \
            name##_resolver_t() { \
                const ptrdiff_t diff = ptrdiff_t(&((api_dll*)0)->name##_resolver) - ptrdiff_t(&((api_dll*)0)->name); \
                name##_t *p = (name##_t*)((char*)this - diff); \
                api_dll* dll = (api_dll*)((char*)this - ((ptrdiff_t)(&((api_dll*)0)->name##_resolver))); \
                if (!dll->isLoaded()) { \
                    CAPI_WARN_LOAD("dll not loaded"); \
                    *p = 0; \
                    return; \
                } \
                *p = (name##_t)dll->resolve(#sym); \
                if (*p) { CAPI_DBG_RESOLVE("dll::" #name ": @%p", *p); } \
                else { CAPI_WARN_RESOLVE("capi resolve error '" #name "'"); } \
            } \
        } name##_resolver;

#if defined(__GNUC__)
#  define CAPI_FUNC_INFO __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#  define CAPI_FUNC_INFO __FUNCSIG__
#else
#  define CAPI_FUNC_INFO __FUNCTION__
#endif
#ifdef DEBUG
#define DEBUG_LOAD
#define DEBUG_RESOLVE
#define DEBUG_CALL
#endif //DEBUG
#if defined(DEBUG) || defined(DEBUG_LOAD) || defined(DEBUG_RESOLVE) || defined(DEBUG_CALL)
#define CAPI_LOG(STDWHERE, fmt, ...) do {fprintf(STDWHERE, "[%s] %s@%d: " fmt "\n", __FILE__, CAPI_FUNC_INFO, __LINE__, ##__VA_ARGS__); fflush(0);} while(0);
#else
#define CAPI_LOG(...)
#endif //DEBUG
#ifdef DEBUG_LOAD
#define CAPI_DBG_LOAD(...) EXPAND(CAPI_LOG(stdout, ##__VA_ARGS__))
#define CAPI_WARN_LOAD(...) EXPAND(CAPI_LOG(stderr, ##__VA_ARGS__))
#else
#define CAPI_DBG_LOAD(...)
#define CAPI_WARN_LOAD(...)
#endif //DEBUG_LOAD
#ifdef DEBUG_RESOLVE
#define CAPI_DBG_RESOLVE(...) EXPAND(CAPI_LOG(stdout, ##__VA_ARGS__))
#define CAPI_WARN_RESOLVE(...) EXPAND(CAPI_LOG(stderr, ##__VA_ARGS__))
#else
#define CAPI_DBG_RESOLVE(...)
#define CAPI_WARN_RESOLVE(...)
#endif //DEBUG_RESOLVE
#ifdef DEBUG_CALL
#define CAPI_DBG_CALL(...) EXPAND(CAPI_LOG(stdout, ##__VA_ARGS__))
#define CAPI_WARN_CALL(...) EXPAND(CAPI_LOG(stderr, ##__VA_ARGS__))
#else
#define CAPI_DBG_CALL(...)
#define CAPI_WARN_CALL(...)
#endif //DEBUG_CALL
//fully expand. used by VC. VC will not expand __VA_ARGS__ but treats it as 1 parameter
#define EXPAND(expr) expr //TODO: rename CAPI_EXPAND
namespace capi {
namespace internal {
// base ctor dll_helper("name")=>derived members in decl order(resolvers)=>derived ctor
static const int kDefaultVersions[] = {capi::NoVersion, capi::EndVersion};
template <class DLL> class dll_helper { //no CAPI_EXPORT required
    DLL m_lib;
public:
    dll_helper(const char* names[], const int versions[] = kDefaultVersions) {
        static bool is_1st = true;
        if (is_1st) {
            is_1st = false;
            printf("capi::version: %s\n", capi::version::name); fflush(0);
        }
        for (int i = 0; names[i]; ++i) {
            for (int j = 0; versions[j] != capi::EndVersion; ++j) {
                if (versions[j] == capi::NoVersion)
                    m_lib.setFileName(names[i]);
                else
                    m_lib.setFileNameAndVersion(names[i], versions[j]);
                if (m_lib.load()) {
                    CAPI_DBG_LOAD("capi loaded {library name: %s, version: %d}", names[i], versions[j]);
                    return;
                }
                CAPI_WARN_LOAD("capi can not load {library name: %s, version %d}", names[i], versions[j]);
            }
        }
    }
    virtual ~dll_helper() { m_lib.unload();}
    bool isLoaded() const { return m_lib.isLoaded(); }
    void* resolve(const char *symbol) { return (void*)m_lib.resolve(symbol);}
};
} //namespace internal
} //namespace capi

#if defined(_MSC_VER)
#pragma warning(disable:4098) //vc return void
#endif //_MSC_VER
#ifdef __GNUC__
//gcc: ((T*)0)->member
//no #pragma GCC diagnostic push/pop around because code is defined as a macro
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif
/*!
 * used by .cpp to define the api
 *  e.g. CAPI_DEFINE(cl_int, clGetPlatformIDs, "clGetPlatformIDs", CAPI_ARG3(cl_uint, cl_platform_id*, cl_uint*))
 * sym: symbol of the api in library.
 */
#define CAPI_DEFINE_X(R, name, ARG_T, ARG_T_V, ARG_V) CAPI_DEFINE_T_V(R, name, ARG_T, ARG_T_V, ARG_V)
/* declare and define the symbol resolvers*/
#define EMPTY_LINKAGE
#define CAPI_DEFINE_RESOLVER_X(R, name, sym, ARG_T, ARG_T_V, ARG_V) CAPI_DEFINE_M_RESOLVER_T_V(R, EMPTY_LINKAGE, name, sym, ARG_T, ARG_T_V, ARG_V)
// api with linkage modifier
#define CAPI_DEFINE_M_RESOLVER_X(R, M, name, sym, ARG_T, ARG_T_V, ARG_V) CAPI_DEFINE_M_RESOLVER_T_V(R, M, name, sym, ARG_T, ARG_T_V, ARG_V)
#define CAPI_ARG0() (), (), ()
#define CAPI_ARG1(P1) (P1), (P1 p1), (p1)
#define CAPI_ARG2(P1, P2) (P1, P2), (P1 p1, P2 p2), (p1, p2)
#define CAPI_ARG3(P1, P2, P3) (P1, P2, P3), (P1 p1, P2 p2, P3 p3), (p1, p2, p3)
#define CAPI_ARG4(P1, P2, P3, P4) (P1, P2, P3, P4), (P1 p1, P2 p2, P3 p3, P4 p4), (p1, p2, p3, p4)
#define CAPI_ARG5(P1, P2, P3, P4, P5) (P1, P2, P3, P4, P5), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5), (p1, p2, p3, p4, p5)
#define CAPI_ARG6(P1, P2, P3, P4, P5, P6) (P1, P2, P3, P4, P5, P6), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6), (p1, p2, p3, p4, p5, p6)
#define CAPI_ARG7(P1, P2, P3, P4, P5, P6, P7) (P1, P2, P3, P4, P5, P6, P7), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7), (p1, p2, p3, p4, p5, p6, p7)
#define CAPI_ARG8(P1, P2, P3, P4, P5, P6, P7, P8) (P1, P2, P3, P4, P5, P6, P7, P8), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8), (p1, p2, p3, p4, p5, p6, p7, p8)
#define CAPI_ARG9(P1, P2, P3, P4, P5, P6, P7, P8, P9) (P1, P2, P3, P4, P5, P6, P7, P8, P9), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9), (p1, p2, p3, p4, p5, p6, p7, p8, p9)
#define CAPI_ARG10(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) (P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10), (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define CAPI_ARG11(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11) (P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11), (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11)
#define CAPI_ARG12(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12) (P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12), (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12)
#define CAPI_ARG13(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13) (P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12, P13 p13), (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13)

#endif // CAPI_H
