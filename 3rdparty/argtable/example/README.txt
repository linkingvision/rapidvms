A note on ANSI C89 versus C99 compilers.
----------------------------------------
These example programs use a style of array initialization that was
introduced in the ANSI C99 standard. Older (C89 compliant) compilers
will not compile them.
The Open Watcom compiler, for instance, is known to have this problem.

If this affects you, then an easy workaround is to initialize your
arrays according to the C89 standard as demonstrated in the
"myprog_C89.c" program.

Thanks to Justin Dearing for pointing this out (Jan 2004).


Compiling the example code under AIX
------------------------------------
The version of make shipped with AIX does not process the argtable
example Makefile correctly. However it will work with GNU gmake.

It is also necessary to add the "intl" library in the example Makefile
   LDLIBS    += -largtable2 -lintl
to avoid an unsatisfied external for gettext
   ld: 0711-317 ERROR: Undefined symbol: .gettext
which is found in libintl.a rather than libc.a on AIX.

Thanks to Michel Valin for these instructions (Feb 2009).

