REM cmake -G "Visual Studio 12 2013"
REM VS2013 x86 Native Tools Command Prompt:
rmdir  build\win32 /s /q
rmdir  win32 /s /q

mkdir build & cd build

REM build Release 
mkdir win32 & cd win32
mkdir release & cd release
cmake -G "NMake Makefiles"  -Dprotobuf_MSVC_STATIC_RUNTIME=ON -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=../../../win32  ../../../
nmake 
nmake install

REM build  Debug
cd ../
mkdir debug & cd debug
cmake -G "NMake Makefiles"  -DCMAKE_BUILD_TYPE=Debug -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=../../../win32  ../../../
nmake 
nmake install

cd ../../../




pause