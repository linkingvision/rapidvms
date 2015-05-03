
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

#ifndef cppkit_os_sprintf_h
#define cppkit_os_sprintf_h

#include <cstdio>
#include <cctype>
#include "cppkit/ck_types.h"
#include "cppkit/ck_string.h"

#ifdef IS_WINDOWS
  #include <stdarg.h>
  #pragma warning(push)
  #pragma warning(disable:4996) // disable vs warning about _vsnprintf
#endif

namespace cppkit
{
#ifdef IS_LINUX
  int ck_printf(const char* format...);
#elif defined(IS_WINDOWS)
  #define ck_printf printf
#else
  int ck_printf(const char* format...);
#endif

#ifdef IS_WINDOWS
  #define ck_snprintf _snprintf
#else
  #define ck_snprintf snprintf
#endif

#ifdef IS_WINDOWS
  inline int ck_vsnprintf(char* str, size_t size, ck_string format, va_list args)
  {
	  if (str == NULL || size == 0)
		  return _vscprintf(format.c_str(), args);
	  else
		  return _vsnprintf(str, size, format.c_str(), args);
  }

  inline int ck_vsnprintf(char* str, size_t size, char* format, va_list args)
  {
    if(str == NULL || size == 0)
        return _vscprintf(format, args);
    else
        return _vsnprintf(str, size, format, args);
  }
#else
  int ck_vsnprintf(char* str, size_t size, ck_string format, va_list args);
#endif

#ifdef IS_WINDOWS
  #define ck_sscanf sscanf_s
#else
  int ck_sscanf(const char* str, const char* format...);
#endif

#ifdef IS_WINDOWS
  #ifndef va_copy
    #define va_copy(dst, src) ((void)((dst) = (src)))
  #endif
#endif
}

#ifdef IS_WINDOWS
  #pragma warning(pop)
#endif

#endif
