NMAKE /f Makefile.nmake clean
NMAKE /f Makefile.nmake DEBUG=1

copy /y .\argtable2.dll  .\win32\Debug
copy /y .\argtable2.lib  .\win32\Debug

NMAKE /f Makefile.nmake clean

NMAKE /f Makefile.nmake 
copy /y .\argtable2.dll  .\win32\Release
copy /y .\argtable2.lib  .\win32\Release

NMAKE /f Makefile.nmake clean