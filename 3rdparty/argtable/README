Installing argtable2 on Unix systems
------------------------------------
The basic compile and install procedure for unix systems is:

    $ cd argtable2-x
    $ ./configure
    $ make
    $ make check (optional)
    $ sudo make install
    $ make clean

Remember to update your system's LD_LIBRARY_PATH to include
the location of the dynamic library (normally /usr/local/lib/).

The configuration script will install argtable into /usr/local/
by default. To install into a different directory, specify
that directory path when you run configure, as in

    $ ./configure --prefix=~fred/mystuff                                    

See install.txt for a detailed description of the configure script.

OS X notes:
    1) Mac OS X uses DYLD_LIBRARY_PATH instead of LD_LIBRARY_PATH.

DJGPP notes:
    1) DJGPP users may need to specify the compiler name at configure
       time, as in 
            $ ./configure CC=gcc.exe



Installing argtable2 on DOS/Windows systems
-------------------------------------------
To build argtable.lib:

    > cd src
    > nmake -f Makefile.nmake

Then copy the argtable2.h, argtable2.lib, argtable2.dll, impargtable2.lib
files to wherever you choose.

To build the example programs:
    
    > cd example
    > nmake -f Makefile.nmake
    
Notes:
   1) The arg_rex and arg_date command line argument types are not supported
      under Microsoft Windows because the Microsoft compilers do not provide
      the necessary regex.h and strptime() functions.

   2) Watcom compilers may have trouble building the DLL version of the library,
      giving an error like this:
      Error! E3033: file _2FK.AAA: line(15): directive error near 'Files\dm\lib'
      To workaround this, edit line 34 of src/Makefile.nmake and remove the 
      "/def:argtable2.def" from the linker stanza 
           
      line 33:      argtable2.dll: $(OBJS)
      line 34:          link /DLL /OUT:$@ $** /IMPLIB:impargtable2.lib



Installing argtable2 on DOS/Windows systems with date and regex support
-----------------------------------------------------------------------
These instructions are provided by Paolo Bormida <paolo.bormida@fastwebnet.it>

1) Install MINGW and MSYS (these instructions are based on those tools).
2) Install a regular expression library such as the TRE library from
   http://www.laurikari.net/tre.
3) Install GnuWin32 LibGW32C (GlibC Windows port) from 
   http://gnuwin32.sourceforge.net/packages/libgw32c.htm
4a) Unpack LibGW32C and TRE in a folder of your choice.
4b) Unpack argtable2 in a folder of your choice and (optional) create a
    "build" folder under it.
5) Run MSYS shell
6) Move into argtable root folder (if you created a "build" folder move into it)
   with CD
7) Run argtable2 "configure" script with the following command line
   configure --prefix=<argtable root folder> 
   CPPFLAGS="-I<tre root folder>/include/tre -idirafter <gw32c folder>/include/glibc" 
   LDFLAGS="-L<tre root folder>/lib -L<gw32c/lib" 
   LIBS="-ltre -lgw32c"

   N.B If you created a "build" folder in step 4b run "../configure" instead ;-)
		
   You will get a makefile to build argtable with (good) support for regular
   expressions and (poor) support for date arguments. The problem is that the
   strptime implementation in LibGW32C is rather poor, does not take locale into
   account (at least so it seems to me by looking at the code)



Cross-compiling argtable2 for Windows from Linux
------------------------------------------------
You can crosscompile argtable2 on Linux with Win32-platforms as target
using mingw32. However, as with native Windows compilers, the arg_rex
and arg_date command line argument types are not supported by mingw32
because it currently offers no regex.h nor strptime() functions.

The following procedure was tested on a Debian "Sid" install with the
mingw32 default packages installed.

1) Make sure, you have the mingw32-crosscompiler installed. There are   
   precompiled packages for several distributions available. You can   
   get the sources from http://www.mingw.org/

2) Set the apropriate environment variables:

        $ export PATH=/path/to/your/mingw/bin:$PATH

   Usually the path is something like
       /usr/i586-mingw32/i586-mingw32/bin
   or
       /usr/local/i586-mingw32/bin

   Sometimes it helps to set also the COMPILER_PATH environment    
   variable to your mingw-binary-path.

3) Run configure:

        $ cd argtable2-x
        $ ./configure --build=i586-linux --host=i586-mingw32msvc \
                 --target=i586-mingw32msvc \
                 --prefix=/path/to/install

   You most certainly want to give a different prefix than the default
   one. The resulting lib will not be useable by your usual ELF-loader.

   See argtable's INSTALL file for further configure options.

4) Build and install argtable2:
 
        $ make
        $ make install

Please note, that this creates a static library (libargtable2.a) and a
shared one (libargtable2.so), which you link to your object code in the
usual way (-L/path/to/prefix/lib -largtable2 -I/path/to/prefix/include).
Building of DLLs is currently not supported, but may be done by hand.
A .def-file is already included in the src/ directory.



Modifying the (Unix) Makefiles
------------------------------
The Makefiles are automatically generated from the Makefile.am templates
using automake-1.9 and libtool (ensure both of these tools are installed).
You must run automake-1.9 from the root node of the source tree after
modifying any of the Makefile.am files for your changes to take effect.

       $ cd argtable2-x
       $ automake-1.9
       $ ./configure
       $ make
    


Using PKG-CONFIG with autoconf
------------------------------
These instructions were provided by Michael Brown who added support for
pkg-config in argtable2-12.

The 'pkg-config' utility can be used to automatically find CFLAGS and
LDFLAGS for external modules, such as argtable, when building other
modules against it. It integrates nicely into autoconf so that if
your application needs the flags for argtable, you can simply declare

    PKG_CHECK_MODULES([ARGTABLE2], [argtable2])

and autoconf will set two vars for you, namely ARGTABLE2_LIBS and
ARGTABLE2_CFLAGS. Typically, _LIBS would be "-largtable2" plus any
library search path dirs that may be needed, "-L/path/". And _CFLAGS
would be any preprocessor defines that are needed, for example,
include paths, "-I/path/".

There are several benefits. Firstly, developers do not need to hardcode
cflags/ldflags in every makefile for libs that use argtable. Secondly,
changes to the argtable cflags/ldflags can be distributed via the
pkg-config files without users having to update their makefiles. Lastly.
and perhaps most importantly, it makes it very easy to build argtable
in a non-standard location and still compile other things against it.

Here is an example

    $ pwd
    /home/michael_e_brown/hapi/git/argtable2-11

    $ PKG_CONFIG_PATH=$PWD pkg-config argtable2 --libs
    -L/home/michael_e_brown/hapi/git/argtable2-11/src/.libs -largtable2

    $ PKG_CONFIG_PATH=$PWD pkg-config argtable2 --cflags
    -I/home/michael_e_brown/hapi/git/argtable2-11/src

I have not installed argtable on my box, but I can easily compile
other programs against it. In this example, pkg-config is looking in
the argtable2-uninstalled.pc that is dropped in the top dir by
configure in argtable2. When you install it, argtable2.pc is installed
in $(libdir)/pkgconfig, which is on the normal pkg-config search path.
So, I dont have to change any of my scripts or makefiles no matter
where argtable is located.


Building argtable using CMake
-----------------------------
Rob Zaborowski has provided CMakeLists.txt files for argtable that
allow CMake users to create a custom build project for argtable 
for virtually any operating system. See http://www.cmake.org for
more about the CMake tool.
