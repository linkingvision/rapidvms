#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(libchardet, 0.0.9, [SounOS@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/CharDistribution.cpp])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stddef.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strdup])

AC_CONFIG_FILES([Makefile
        pkgconfig/libchardet.pc
        pkgconfig/Makefile
		libchardet.spec
		src/Makefile])
AC_OUTPUT
