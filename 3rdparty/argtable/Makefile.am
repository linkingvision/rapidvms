SUBDIRS = src tests doc
ACLOCAL_AMFLAGS=-I m4

pcfiledir=$(libdir)/pkgconfig
pcfile_DATA=argtable2.pc

exampledir=@exampledir@
dist_example_DATA  = example/Makefile example/README.txt
dist_example_DATA += example/argcustom.c
dist_example_DATA += example/argxxx.c
dist_example_DATA += example/argxxx.h
dist_example_DATA += example/callbacks.c
dist_example_DATA += example/echo.c
dist_example_DATA += example/hasoptvalue.c
dist_example_DATA += example/ls.c
dist_example_DATA += example/multisyntax.c
dist_example_DATA += example/mv.c
dist_example_DATA += example/myprog.c
dist_example_DATA += example/myprog_C89.c
dist_example_DATA += example/rm.c
dist_example_DATA += example/uname.c

# Extra rule to ensure the Windows-specific and CMake files are included in the distribution tarball.
dist-hook:
	cp -p $(top_srcdir)/CMakeLists.txt $(distdir)
	cp -p $(top_srcdir)/src/CMakeLists.txt $(distdir)/src
	cp -p $(top_srcdir)/src/Makefile.nmake $(distdir)/src
	cp -p $(top_srcdir)/src/argtable2.def $(distdir)/src
	cp -p $(top_srcdir)/example/Makefile.nmake $(distdir)/example

