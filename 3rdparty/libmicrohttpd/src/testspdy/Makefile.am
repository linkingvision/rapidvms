# This Makefile.am is in the public domain
SUBDIRS  = .

AM_CFLAGS = -DDATA_DIR=\"$(top_srcdir)/src/datadir/\"

if USE_COVERAGE
  AM_CFLAGS += -fprofile-arcs -ftest-coverage
endif

AM_CPPFLAGS = \
 -I$(top_srcdir) \
 -I$(top_srcdir)/src/include \
 -I$(top_srcdir)/src/applicationlayer \
 $(LIBCURL_CPPFLAGS)

if !HAVE_W32
PERF_GET_CONCURRENT=perf_get_concurrent
endif

if ENABLE_SPDY
if HAVE_OPENSSL
check_PROGRAMS = \
  test_daemon_start_stop \
  test_daemon_start_stop_many \
  test_struct_namevalue

if HAVE_SPDYLAY  
check_PROGRAMS += \
  test_new_connection  \
  test_request_response \
  test_notls \
  test_request_response_with_callback \
  test_misc \
  test_session_timeout
  #test_requests_with_assets 
if HAVE_CURL_BINARY
check_PROGRAMS +=  \
  test_proxies
endif 
endif
endif 
endif 


TESTS = $(check_PROGRAMS)


SPDY_SOURCES= \
 common.h common.c

SPDY_LDADD=  \
 $(top_builddir)/src/microspdy/libmicrospdy.la \
 -lz

test_daemon_start_stop_SOURCES = \
 test_daemon_start_stop.c \
 $(SPDY_SOURCES) 
test_daemon_start_stop_LDADD =  $(SPDY_LDADD)

test_daemon_start_stop_many_SOURCES = \
 test_daemon_start_stop_many.c  \
 $(SPDY_SOURCES) 
test_daemon_start_stop_many_LDADD = $(SPDY_LDADD)

test_struct_namevalue_SOURCES = \
 test_struct_namevalue.c  \
 $(SPDY_SOURCES) 
test_struct_namevalue_LDADD = $(SPDY_LDADD)

if HAVE_SPDYLAY
test_new_connection_SOURCES = \
 test_new_connection.c  \
 $(SPDY_SOURCES) 
test_new_connection_LDADD = $(SPDY_LDADD) \
 -lspdylay

test_request_response_SOURCES = \
 test_request_response.c  \
 $(SPDY_SOURCES) 
test_request_response_LDADD = $(SPDY_LDADD) \
 -lspdylay

test_notls_SOURCES = \
 test_notls.c  \
 $(SPDY_SOURCES) 
test_notls_LDADD = $(SPDY_LDADD) \
 -lspdylay

test_request_response_with_callback_SOURCES = \
 test_request_response_with_callback.c  \
 $(SPDY_SOURCES) 
test_request_response_with_callback_LDADD = $(SPDY_LDADD)

#test_requests_with_assets_SOURCES = \
# test_requests_with_assets.c  \
# $(SPDY_SOURCES) 
#test_requests_with_assets_LDADD = $(SPDY_LDADD)

test_misc_SOURCES = \
 test_misc.c  \
 $(SPDY_SOURCES) 
test_misc_LDADD = $(SPDY_LDADD)

test_session_timeout_SOURCES = \
 test_session_timeout.c  \
 $(SPDY_SOURCES) 
test_session_timeout_LDADD = $(SPDY_LDADD)


test_proxies_SOURCES = \
 test_proxies.c \
 $(SPDY_SOURCES) 
test_proxies_LDADD = $(SPDY_LDADD)

endif
