# This Makefile.am is in the public domain
SUBDIRS  = .

AM_CPPFLAGS = \
  -I$(top_srcdir)/src/include \
  -DDATA_DIR=\"$(top_srcdir)/src/datadir/\"

AM_CFLAGS = @LIBGCRYPT_CFLAGS@

CPU_COUNT_DEF = -DCPU_COUNT=$(CPU_COUNT)

if USE_COVERAGE
  AM_CFLAGS += --coverage
endif

if ENABLE_SPDY
spdyex = \
  spdy_event_loop \
  spdy_fileserver \
  spdy_response_with_callback

if HAVE_SPDYLAY
spdyex += mhd2spdy
endif
endif


# example programs
noinst_PROGRAMS = \
 benchmark \
 benchmark_https \
 chunked_example \
 minimal_example \
 dual_stack_example \
 minimal_example_comet \
 querystring_example \
 fileserver_example \
 fileserver_example_dirs \
 fileserver_example_external_select \
 refuse_post_example \
 $(spdyex)


if ENABLE_HTTPS
noinst_PROGRAMS += https_fileserver_example
endif
if HAVE_POSTPROCESSOR
noinst_PROGRAMS += \
  post_example
if HAVE_MAGIC
noinst_PROGRAMS += \
  demo \
  demo_https
endif
endif

if ENABLE_DAUTH
noinst_PROGRAMS += \
 digest_auth_example
endif

if ENABLE_BAUTH
noinst_PROGRAMS += \
 authorization_example
endif

if HAVE_W32
AM_CFLAGS += -DWINDOWS
endif

minimal_example_SOURCES = \
 minimal_example.c
minimal_example_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la

chunked_example_SOURCES = \
 chunked_example.c
chunked_example_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la

demo_SOURCES = \
 demo.c
demo_CFLAGS = \
 $(PTHREAD_CFLAGS) $(AM_CFLAGS)
demo_CPPFLAGS = \
 $(AM_CPPFLAGS) $(CPU_COUNT_DEF)
demo_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la  \
 $(PTHREAD_LIBS) -lmagic

demo_https_SOURCES = \
 demo_https.c
demo_https_CFLAGS = \
 $(PTHREAD_CFLAGS) $(AM_CFLAGS)
demo_https_CPPFLAGS = \
 $(AM_CPPFLAGS) $(CPU_COUNT_DEF)
demo_https_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la  \
 $(PTHREAD_LIBS) -lmagic

mhd2spdy_SOURCES = \
 mhd2spdy.c \
 mhd2spdy_spdy.c mhd2spdy_spdy.h \
 mhd2spdy_http.c mhd2spdy_http.h \
 mhd2spdy_structures.c mhd2spdy_structures.h
mhd2spdy_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la  \
 -lssl -lcrypto -lspdylay

benchmark_SOURCES = \
 benchmark.c
benchmark_CPPFLAGS = \
 $(AM_CPPFLAGS) $(CPU_COUNT_DEF)
benchmark_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la

benchmark_https_SOURCES = \
 benchmark_https.c
benchmark_https_CPPFLAGS = \
 $(AM_CPPFLAGS) $(CPU_COUNT_DEF)
benchmark_https_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la

dual_stack_example_SOURCES = \
 dual_stack_example.c
dual_stack_example_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la

post_example_SOURCES = \
 post_example.c
post_example_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la

minimal_example_comet_SOURCES = \
 minimal_example_comet.c
minimal_example_comet_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la

authorization_example_SOURCES = \
 authorization_example.c
authorization_example_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la

digest_auth_example_SOURCES = \
 digest_auth_example.c
digest_auth_example_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la

refuse_post_example_SOURCES = \
 refuse_post_example.c
refuse_post_example_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la

querystring_example_SOURCES = \
 querystring_example.c
querystring_example_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la

fileserver_example_SOURCES = \
 fileserver_example.c
fileserver_example_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la

fileserver_example_dirs_SOURCES = \
 fileserver_example_dirs.c
fileserver_example_dirs_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la

fileserver_example_external_select_SOURCES = \
 fileserver_example_external_select.c
fileserver_example_external_select_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la

https_fileserver_example_SOURCES = \
https_fileserver_example.c
https_fileserver_example_CPPFLAGS = \
 $(AM_CPPFLAGS) $(GNUTLS_CPPFLAGS)
https_fileserver_example_LDADD = \
 $(top_builddir)/src/microhttpd/libmicrohttpd.la


spdy_event_loop_SOURCES = \
 spdy_event_loop.c
spdy_event_loop_LDADD = \
  $(top_builddir)/src/microspdy/libmicrospdy.la \
 -lz

spdy_fileserver_SOURCES = \
 spdy_fileserver.c
spdy_fileserver_LDADD = \
  $(top_builddir)/src/microspdy/libmicrospdy.la \
 -lz

spdy_response_with_callback_SOURCES = \
 spdy_response_with_callback.c
spdy_response_with_callback_LDADD = \
  $(top_builddir)/src/microspdy/libmicrospdy.la \
 -lz
