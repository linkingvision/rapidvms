REM cmake -G "Visual Studio 12 2013"
REM VS2013 x86 Native Tools Command Prompt:
REM PROTOBUF_USE_DLLS
rmdir  build\win32 /s /q
rmdir  win32 /s /q

mkdir build & cd build

REM build Release 
mkdir win32 & cd win32
mkdir release & cd release
cmake -G "NMake Makefiles"  -DWITH_BOOST=OFF -DWITH_MYSQL=OFF -DWITH_ODBC=OFF -DWITH_POSTGRESQL=OFF -DWITH_ORACLE=OFF -DWITH_DB2=OFF -DWITH_FIREBIRD=ON -DFIREBIRD_INCLUDE_DIR=../../../../../firebird-R3_0_0/output_Win32/include -DFIREBIRD_LIBRARIES=../../../../../firebird-R3_0_0/output_Win32/lib/fbclient_ms.lib -DSOCI_TESTS=OFF -DCMAKE_BUILD_TYPE=Release  -DCMAKE_INSTALL_PREFIX=../../../win32  ../../../
nmake 
nmake install

REM build  Debug
cd ../
mkdir debug & cd debug
cmake -G "NMake Makefiles"  -DWITH_BOOST=OFF -DWITH_MYSQL=OFF -DWITH_ODBC=OFF -DWITH_POSTGRESQL=OFF -DWITH_ORACLE=OFF -DWITH_DB2=OFF -DWITH_FIREBIRD=ON -DFIREBIRD_INCLUDE_DIR=../../../../../firebird-R3_0_0/output_Win32/include -DFIREBIRD_LIBRARIES=../../../../../firebird-R3_0_0/output_Win32/lib/fbclient_ms.lib -DSOCI_TESTS=OFF -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_INSTALL_PREFIX=../../../win32  ../../../
nmake 
nmake install

cd ../../../




pause