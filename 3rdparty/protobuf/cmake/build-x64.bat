REM cmake -G "Visual Studio 12 2013"
REM VS2013 x64 Native Tools Command Prompt:
rmdir  build\x64 /s /q
rmdir  x64 /s /q

mkdir build & cd build

REM build Release 
mkdir x64 & cd x64
mkdir release & cd release
cmake -G "NMake Makefiles"  -Dprotobuf_BUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=../../../x64  ../../../
nmake 
nmake install

REM build  Debug
cd ../
mkdir debug & cd debug
cmake -G "NMake Makefiles"  -Dprotobuf_BUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Debug -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=../../../x64  ../../../
nmake 
nmake install

cd ../../../




pause