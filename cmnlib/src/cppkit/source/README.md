cppkit
======

XXX - cppkit is a work in progress, and is currently in a VERY alpha state. Use at your own risk.

cppkit is a posix / win32 portability library that also aims to fill in a few cracks in C++11. Our primary focus is on
ease of use. Our goal is that developers familiar with higher level languages will be comfortable with cppkit.

cppkit ecosystem
================

cppkit aims to be the base library in an ecosystem of C++11 libraries and attempts to set some conventions to make
downstream use of the ecosystem easier. First and foremost, cppkit and all downstream libs should use cmake.

cppkit (and downstream libraries) will install their headers 
and libraries to a directory at the same level as the cppkit
project called "devel_artifacts". Headers for each library will
go into their own subdirectory under "devel_artifacts/include".
For example, a downstream library called "foo" would put it's
headers in "devel_artifacts/include/foo".

Libraries for all downstream projects will install into
"devel_artifacts/lib".

cppkit will install build support files into 
"devel_artifacts/build". Downstream projects should only have to
define a few variables and then include the appropriate build
support cmake file to get all of the default ecosystem
behaviour.

By using the common build support files downstream libraries
will automatically have "devel_artifacts/include" added to their
compilers include search path. They will also have
"devel_artifacts/lib" added to their library search path.
Projects should include the headers of other projects like this:

          #include "cppkit/ck_dynamic_library.h"

IMHO, this sort of header namespacing makes it nice and obvious
where everything is coming from (and avoids naming conflicts).

Downstream libraries should define their own C++ namespace, and
(unless they have VERY good reason not to) confine all of their
symbols to their own namespace.



future work
===========

I'd like to write an optional build system for the cppkit ecosystem. This build system would let you define a list of
projects and their dependancy order in a config file. You could then ask the build system to check out and build the
entire dependancy tree. This project would of course be optional (it would be perfectly valid to only utilize the
eco system described above and write your own higher level build infrastructure).


building cppkit - Linux
===============

cppkit uses cmake. To compile and install the library type the following from the top level cppkit directory:

       mkdir build
       cd build
       cmake ..
       make
       make install

To build and run the unit tests type the following from the "ut" directory:

       mkdir build
       cd build
       cmake ..
       make
       LD_LIBRARY_PATH=../../devel_artifacts/lib ./ut


building cppkit - Windows

- mkdir build
- cd build
- cmake --build . --clean-first
- cmake --build . --target install

