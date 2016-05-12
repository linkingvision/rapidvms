REM cmake -G "Visual Studio 12 2013"
REM VS2013 x64 Native Tools Command Prompt:
rmdir  build\x64 /s /q
rmdir  x64 /s /q

mkdir build & cd build

REM build Release 
mkdir x64 & cd x64
mkdir release & cd release
cmake -G "NMake Makefiles"  -DWITH_BOOST=OFF -DWITH_MYSQL=OFF -DWITH_ODBC=OFF -DWITH_POSTGRESQL=OFF -DWITH_ORACLE=OFF -DWITH_DB2=OFF -DWITH_FIREBIRD=ON -DFIREBIRD_INCLUDE_DIR=../../../../../firebird/output_x64/include -DFIREBIRD_LIBRARIES=../../../../../firebird/output_x64/lib/fbclient_ms.lib -DSOCI_TESTS=OFF -DCMAKE_BUILD_TYPE=Release  -DCMAKE_INSTALL_PREFIX=../../../x64  ../../../
nmake 
nmake install

REM build  Debug
cd ../
mkdir debug & cd debug
cmake -G "NMake Makefiles"  -DWITH_BOOST=OFF -DWITH_MYSQL=OFF -DWITH_ODBC=OFF -DWITH_POSTGRESQL=OFF -DWITH_ORACLE=OFF -DWITH_DB2=OFF -DWITH_FIREBIRD=ON -DFIREBIRD_INCLUDE_DIR=../../../../../firebird/output_x64/include -DFIREBIRD_LIBRARIES=../../../../../firebird/output_x64/lib/fbclient_ms.lib -DSOCI_TESTS=OFF -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_INSTALL_PREFIX=../../../x64  ../../../
nmake 
nmake install

cd ../../../




pause