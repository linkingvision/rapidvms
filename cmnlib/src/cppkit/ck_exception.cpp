
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

#include "cppkit/ck_exception.h"
#include "cppkit/ck_string.h"
#include "cppkit/ck_stack_trace.h"

using namespace std;
using namespace cppkit;

ck_exception::ck_exception()
    : exception(),
      _type_name("cppkit::ck_exception"),
      _msg(),
      _line_num(0),
      _src_file(),
      _stack(ck_stack_trace::get_stack()),
      _what_msg()
{
}

ck_exception::ck_exception(const string& msg)
    : exception(),
      _type_name("cppkit::ck_exception"),
      _msg(msg),
      _line_num(0),
      _src_file(),
      _stack(ck_stack_trace::get_stack()),
      _what_msg()
{
}

ck_exception::ck_exception(const char* msg, ...)
    : exception(),
      _type_name("XSDK::ck_exception"),
      _msg(),
      _line_num(0),
      _src_file(),
      _stack(ck_stack_trace::get_stack()),
      _what_msg()
{
    va_list args;
    va_start(args, msg);
    _msg = ck_string::format(msg, args);
    va_end(args);
}

ck_exception::~ck_exception() throw()
{
}

void ck_exception::set_type_name(const std::string& typeName)
{
    _type_name = typeName;
}

const char* ck_exception::get_type_name() const
{
    return _type_name.c_str();
}

const char* ck_exception::get_msg() const
{
    return _msg.c_str();
}

void ck_exception::set_msg(std::string msg)
{
    _msg = msg;
}

void ck_exception::set_throw_point(int line, const char* file)
{
    _line_num = line;
    _src_file = file;
}

const char* ck_exception::what() const throw()
{
    return ck_string::format("%s thrown from [%s:%d]: \"%s\"",
                             get_type_name(),
                             get_src_file(),
                             get_line_num(),
                             get_msg()).c_str();
}

file_not_found_exception::file_not_found_exception()
    : ck_exception()
{
}

file_not_found_exception::~file_not_found_exception() throw()
{
}

file_not_found_exception::file_not_found_exception(const char* msg, ...) : ck_exception()
{
    va_list args;
    va_start(args, msg);
    set_msg(ck_string::format(msg, args));
    va_end(args);
}

file_not_found_exception::file_not_found_exception(const string& msg)
    : ck_exception(msg)
{
}
