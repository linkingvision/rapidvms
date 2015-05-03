
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// cppkit - http://www.cppkit.org
/// Copyright (c) 2013, Tony Di Croce
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without modification, are permitted
/// provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and
///    the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
///    and the following disclaimer in the documentation and/or other materials provided with the
///    distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
/// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
/// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
/// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
/// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
/// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// The views and conclusions contained in the software and documentation are those of the authors and
/// should not be interpreted as representing official policies, either expressed or implied, of the cppkit
/// Project.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-

#ifndef cppkit_exception_h
#define cppkit_exception_h

#include "cppkit/os/ck_exports.h"
#include "cppkit/os/ck_platform.h"
#include "cppkit/ck_logger.h"

#include <string>
#include <exception>
#include <assert.h>

namespace cppkit
{

class ck_exception : public std::exception
{
public:
    CK_API ck_exception();

    CK_API ck_exception(const std::string& msg);

    CK_API ck_exception(const char* msg, ...);

    CK_API virtual ~ck_exception() throw();

    CK_API virtual const char* what() const throw();

    /// This is used by the throwing macros so that derived exceptions can have their names used in
    /// their messages rather than ck_exception.
    CK_API void set_type_name(const std::string& typeName);
    CK_API const char* get_type_name() const;

    CK_API const char* get_msg() const;
    CK_API void set_msg(std::string msg);

    CK_API void set_throw_point(int line, const char* file);
    CK_API int get_line_num() const { return _line_num; }
    CK_API const char* get_src_file() const { return _src_file.c_str(); }

    CK_API const char* get_stack() const { return _stack.c_str(); }

private:
    std::string _type_name;
    std::string _msg;
    int _line_num;
    std::string _src_file;
    std::string _stack;
    mutable std::string _what_msg;
};

class file_not_found_exception : public ck_exception
{
public:
    CK_API file_not_found_exception();

    CK_API virtual ~file_not_found_exception() throw();

    CK_API file_not_found_exception(const char* msg, ...);

    CK_API file_not_found_exception(const std::string& msg);
};

}

/// Throws an ck_exception and logs (at level WARNING) before throwing it.
#define CK_THROW(PARAMS) \
CK_MACRO_BEGIN \
    cppkit::ck_exception __exception PARAMS ; \
    __exception.set_throw_point(__LINE__, __FILE__); \
    CK_LOG_WARNING("Exception thrown. Msg: \"%s\", At: %s(%d)\n", \
            __exception.get_msg(), \
            __FILE__, \
            __LINE__); \
    cppkit::ck_log::log_backtrace(); \
    throw __exception; \
CK_MACRO_END

/// Throws an ck_exception without logging
#define CK_THROW_NO_LOG(PARAMS)                       \
CK_MACRO_BEGIN                                       \
    cppkit::ck_exception __exception PARAMS ;         \
    __exception.set_throw_point(__LINE__, __FILE__); \
    throw __exception;                               \
CK_MACRO_END

/// Throws an XExepction and logs (at level CRITICAL) before throwing it.
#define CK_THROW_CRITICAL(PARAMS)                                   \
CK_MACRO_BEGIN                                                     \
    cppkit::ck_exception __exception PARAMS ;                       \
    __exception.set_throw_point(__LINE__, __FILE__);               \
    CK_LOG_CRITICAL("Exception thrown. Msg: \"%s\", At: %s(%d)\n", \
                    __exception.get_msg(),                         \
                    __FILE__,                                      \
                    __LINE__);                                     \
    cppkit::ck_log::log_backtrace();                                \
    throw __exception;                                             \
CK_MACRO_END

/// Puts double quotes around its argument.
/// ENQUOTE_EXPAND is required if you want to pass a macro to ENQUOTE.
#define ENQUOTE_EXPAND(tok) #tok
#define ENQUOTE(tok) ENQUOTE_EXPAND(tok)

/// Throws the given exception type and logs (at level WARNING) before throwing it.
#define CK_STHROW(ETYPE, PARAMS)                            \
CK_MACRO_BEGIN                                             \
    ETYPE _exception PARAMS ;                              \
    _exception.set_throw_point(__LINE__, __FILE__);        \
    _exception.set_type_name(ENQUOTE(ETYPE));              \
    CK_LOG_WARNING("%s thrown. Msg: \"%s\", At: %s(%d)\n", \
            _exception.get_type_name(),                    \
            _exception.get_msg(),                          \
            __FILE__,                                      \
            __LINE__);                                     \
    cppkit::ck_log::log_backtrace();                        \
    throw _exception;                                      \
CK_MACRO_END

/// Throw the given exception type without logging
#define CK_STHROW_NO_LOG(ETYPE, PARAMS)              \
CK_MACRO_BEGIN                                      \
    ETYPE _exception PARAMS ;                       \
    _exception.set_throw_point(__LINE__, __FILE__); \
    _exception.set_type_name(ENQUOTE(ETYPE));       \
    throw _exception;                               \
CK_MACRO_END

#define CK_ASSERT_NO_LOG assert

#ifdef IS_DEBUG
  #define CK_VERIFY CK_ASSERT_NO_LOG
#else
  #define CK_VERIFY(exp) (exp)
#endif

/// A version of assert that logs when the assertion fails.
#ifndef NDEBUG
#define CK_ASSERT(x) \
CK_MACRO_BEGIN \
    const bool rx = x; \
    CK_LOG_ASSERT(rx); \
    assert(rx); \
CK_MACRO_END
#else
#define CK_ASSERT(x)
#endif

#endif
