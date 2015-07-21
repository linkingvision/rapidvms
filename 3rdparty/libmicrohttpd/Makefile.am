# This Makefile.am is in the public domain
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = contrib src m4 .
EXTRA_DIST = acinclude.m4 libmicrohttpd.pc.in libmicrospdy.pc.in  \
  w32/VS2013/libmicrohttpd.sln w32/VS2013/libmicrohttpd.vcxproj w32/VS2013/libmicrohttpd.vcxproj.filters \
  w32/VS2013/hellobrowser.vcxproj w32/VS2013/hellobrowser.vcxproj.filters w32/VS2013/MHD_config.h \
  w32/VS2013/gen_dll_res.ps1 w32/VS2013/microhttpd_dll_res_vc.rc.in w32/VS2013/microhttpd_dll_res_vc.rc

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libmicrohttpd.pc libmicrospdy.pc

if BUILD_DOC
SUBDIRS += doc
endif
