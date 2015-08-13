NMAKE /f Makefile.nmake clean
NMAKE /f Makefile.nmake DEBUG=1

copy /y .\argtable2.dll  .\win64\Debug
copy /y .\argtable2.lib  .\win64\Debug

NMAKE /f Makefile.nmake clean

NMAKE /f Makefile.nmake 
copy /y .\argtable2.dll  .\win64\Release
copy /y .\argtable2.lib  .\win64\Release

NMAKE /f Makefile.nmake clean