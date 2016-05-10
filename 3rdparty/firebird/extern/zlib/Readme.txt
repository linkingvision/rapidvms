The zlib.exe is a SFX archive with set of include files required to build 
Firebird with zlib support and compiled zlib DLL's for both win32 and x64 
architectures.

The source code of zlib library was downloaded from 

  http://zlib.net/zlib128.zip

It was built with MSVC10 compilers using commands specified at win32/Makefile.msc:

win32: 
nmake -f win32/Makefile.msc LOC="-DASMV -DASMINF" OBJA="inffas32.obj match686.obj"

x64:
nmake -f win32/Makefile.msc AS=ml64 LOC="-DASMV -DASMINF -I." OBJA="inffasx64.obj gvmat64.obj inffas8664.obj"
