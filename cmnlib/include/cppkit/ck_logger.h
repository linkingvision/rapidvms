
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

#ifndef cppkit_logger_h
#define cppkit_logger_h

#include "cppkit/ck_types.h"
#include "cppkit/os/ck_exports.h"

#include <fstream>
#include <memory>
#include <cstdarg>
#include <string>
#include <mutex>

/**
 * @def CK_LGR_DFLT_MOD_ID
 * this macro is used to set a module for logging so that filtering can be applied.
 * to leverage this add the following two preprocessor directives to your CPP file, and then
 * change the define directive to have the module name you want.
 *
 * #undef CK_LGR_DFLT_MOD_ID
 * #define CK_LGR_DFLT_MOD_ID "YourModuleName"
 */
#define CK_LGR_DFLT_MOD_ID "cppkit:default_module"

namespace cppkit
{

/// Defines the different logging levels. The values map to syslog levels.
enum LOG_LEVEL
{
  LOGLEVEL_CRITICAL = 1,  /// Should be corrected immediately, but indicates failure in a primary
                          /// system. 2 skipped (syslog level ALERT)
  LOGLEVEL_ERROR    = 3,  /// Non-urgent failures - these should be relayed to developers.
  LOGLEVEL_WARNING  = 4,  /// Not an error, but indication that an error will occur if action is not
                          /// taken, e.g. file system 85% full.
  LOGLEVEL_NOTICE   = 5,  /// Events that are unusual but not error conditions - might be summarized in
                          /// an email to developers to spot potential problems.
  LOGLEVEL_INFO     = 6,  /// Normal operational messages - may be harvested for reporting, measuring
                          /// throughput, etc.
  LOGLEVEL_TRACE    = 7,  /// Provides Tracebility of methods being entered or exited - may be harvested
                          /// for reporting, measuring throughput, etc.
  LOGLEVEL_DEBUG    = 8,  /// Info useful to developers for debugging the application, not useful during
                          /// operations.
};

const char kLogCriticalPrefix[] = "[CRITICAL]";
const char kLogErrorPrefix[]    = "[ERROR]";
const char kLogWarningPrefix[]  = "[WARNING]";
const char kLogNoticePrefix[]   = "[NOTICE]";
const char kLogInfoPrefix[]     = "[INFO]";
const char kLogTracePrefix[]    = "[TRACE]";
const char kLogDebugPrefix[]    = "[DEBUG]";

// Flags for controlling what information is included in log messages.
const uint32_t kLogFileLine   = 0x00000001;
const uint32_t kLogThreadID   = 0x00000002;
const uint32_t kLogTimestamp  = 0x00000004;

/// Simple printf-style macros for logging messages at different levels.
#define CK_LOG_CRITICAL(format, ...) cppkit::ck_log::write(CK_LGR_DFLT_MOD_ID, cppkit::LOGLEVEL_CRITICAL, __LINE__, __FILE__, format,  ##__VA_ARGS__)
#define CK_LOG_ERROR(format, ...)    cppkit::ck_log::write(CK_LGR_DFLT_MOD_ID, cppkit::LOGLEVEL_ERROR,    __LINE__, __FILE__, format,  ##__VA_ARGS__)
#define CK_LOG_WARNING(format, ...)  cppkit::ck_log::write(CK_LGR_DFLT_MOD_ID, cppkit::LOGLEVEL_WARNING,  __LINE__, __FILE__, format,  ##__VA_ARGS__)
#define CK_LOG_NOTICE(format, ...)   cppkit::ck_log::write(CK_LGR_DFLT_MOD_ID, cppkit::LOGLEVEL_NOTICE,   __LINE__, __FILE__, format,  ##__VA_ARGS__)
#define CK_LOG_INFO(format, ...)     cppkit::ck_log::write(CK_LGR_DFLT_MOD_ID, cppkit::LOGLEVEL_INFO,     __LINE__, __FILE__, format,  ##__VA_ARGS__)
#define CK_LOG_TRACE(format, ...)    cppkit::ck_log::write(CK_LGR_DFLT_MOD_ID, cppkit::LOGLEVEL_TRACE,    __LINE__, __FILE__, format,  ##__VA_ARGS__)
#define CK_LOG_DEBUG(format, ...)    cppkit::ck_log::write(CK_LGR_DFLT_MOD_ID, cppkit::LOGLEVEL_DEBUG,    __LINE__, __FILE__, format,  ##__VA_ARGS__)
#define CK_LOG_ASSERT(x) if(!(x)) { cppkit::ck_log::write(CK_LGR_DFLT_MOD_ID, cppkit::LOGLEVEL_ERROR,     __LINE__, __FILE__, "[Assertion Failed]"); cppkit::ck_log::log_backtrace(); }

class ck_log
{
public:
    CK_API static bool log_enabled();
    CK_API static void set_log_enabled(bool enabled);

    CK_API static uint32_t get_log_flags();
    CK_API static void set_log_flags(const uint32_t flags);

    CK_API static LOG_LEVEL get_log_level();
    CK_API static void set_log_level(const LOG_LEVEL level);

    CK_API static std::string log_level_to_string(const LOG_LEVEL level);
    CK_API static LOG_LEVEL string_to_log_level(const std::string& str);

    CK_API static bool print_to_std_out_enabled();
    CK_API static void set_print_to_std_out_enabled(bool enabled);

    CK_API static bool print_to_file_enabled();
    CK_API static void disable_print_to_file();
    CK_API static void enable_print_to_file(const std::string& filename, bool append);

    //Installs signal handlers for SIGUSR1 and SIGUSR2 to increase
    //and decrease the log level respectively.
    CK_API static void install_log_level_sig_handler();

    CK_API static void log_backtrace();

    CK_API static bool write(const char* module,
                            const LOG_LEVEL level,
                            const char* format,
                            ...);

    // NOTE: 'line' is defined before 'file' because there is a potential ambiguity if the above
    //       definition of Write() is called as Write(level,char*,int,char*,etc)... is the first
    //       char* the file name or the format string???
    CK_API static bool write(const char* module,
                            const LOG_LEVEL level,
                            const int line,
                            const char* file,
                            const char* format,
                            ...);

    CK_API static bool write(const char* module,
                            const LOG_LEVEL level,
                            const int line,
                            const char* file,
                            const char* format,
                            va_list& args);

    CK_API static void clear_filters();
    CK_API static void add_filter(const char* filter, LOG_LEVEL level=LOGLEVEL_DEBUG);

private:
    CK_API static bool _should_log(const char* module, const LOG_LEVEL level);
    CK_API static bool _write(const std::string& module,
                             const LOG_LEVEL level,
                             const std::string& prefix,
                             const std::string& msg);

    static uint32_t _curr_log_flags;
    static LOG_LEVEL _curr_log_level;

    static bool _log_enabled;
    static std::recursive_mutex _log_lock;
    static bool _print_to_std_out_enabled;

    static std::unique_ptr<std::ofstream> _file;

public:
    class static_init
    {
    public:
        static_init();
    };

    friend class static_init;

    const static static_init _static_init;
};

}

#endif
