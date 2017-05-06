REM ============================================VECVR
REM =============================
set QT_WIN64=C:\Qt\Qt5.7.0-x64\5.7\msvc2013_64\
REM x64 Debug
xcopy /y /e 3rdparty\ffmpeg-win\ffmpeg-x64\bin  vecvr\prj_win32\x64\Debug

xcopy /y /e 3rdparty\poco\bin64\PocoData64d.dll  vecvr\prj_win32\x64\Debug
xcopy /y /e 3rdparty\poco\bin64\PocoDataSQLite64d.dll  vecvr\prj_win32\x64\Debug
xcopy /y /e 3rdparty\poco\bin64\PocoFoundation64d.dll  vecvr\prj_win32\x64\Debug
xcopy /y /e 3rdparty\poco\bin64\PocoJSON64d.dll  vecvr\prj_win32\x64\Debug
xcopy /y /e 3rdparty\poco\bin64\PocoNet64d.dll  vecvr\prj_win32\x64\Debug
xcopy /y /e 3rdparty\poco\bin64\PocoUtil64d.dll  vecvr\prj_win32\x64\Debug
xcopy /y /e 3rdparty\poco\bin64\PocoXML64d.dll  vecvr\prj_win32\x64\Debug
xcopy /y /e 3rdparty\poco\bin64\PocoZip64d.dll  vecvr\prj_win32\x64\Debug

xcopy /y /e 3rdparty\openssl-win\openssl-win64\lib\libeay32.dll  vecvr\prj_win32\x64\Debug
xcopy /y /e 3rdparty\openssl-win\openssl-win64\lib\ssleay32.dll  vecvr\prj_win32\x64\Debug
xcopy /y /e 3rdparty\openssl-win\openssl-win64\lib\openssl.exe  vecvr\prj_win32\x64\Debug

xcopy /y /e 3rdparty\protobuf\cmake\x64\bin\libprotobufd.dll  vecvr\prj_win32\x64\Debug



REM x64 Release
xcopy /y /e 3rdparty\ffmpeg-win\ffmpeg-x64\bin  vecvr\prj_win32\x64\Release

xcopy /y /e 3rdparty\poco\bin64\PocoData64.dll  vecvr\prj_win32\x64\Release
xcopy /y /e 3rdparty\poco\bin64\PocoDataSQLite64.dll  vecvr\prj_win32\x64\Release
xcopy /y /e 3rdparty\poco\bin64\PocoFoundation64.dll  vecvr\prj_win32\x64\Release
xcopy /y /e 3rdparty\poco\bin64\PocoJSON64.dll  vecvr\prj_win32\x64\Release
xcopy /y /e 3rdparty\poco\bin64\PocoNet64.dll  vecvr\prj_win32\x64\Release
xcopy /y /e 3rdparty\poco\bin64\PocoUtil64.dll  vecvr\prj_win32\x64\Release
xcopy /y /e 3rdparty\poco\bin64\PocoXML64.dll  vecvr\prj_win32\x64\Release
xcopy /y /e 3rdparty\poco\bin64\PocoZip64.dll  vecvr\prj_win32\x64\Release

xcopy /y /e 3rdparty\openssl-win\openssl-win64\lib\libeay32.dll  vecvr\prj_win32\x64\Release
xcopy /y /e 3rdparty\openssl-win\openssl-win64\lib\ssleay32.dll  vecvr\prj_win32\x64\Release
xcopy /y /e 3rdparty\openssl-win\openssl-win64\lib\openssl.exe  vecvr\prj_win32\x64\Release

xcopy /y /e 3rdparty\protobuf\cmake\x64\bin\libprotobuf.dll  vecvr\prj_win32\x64\Release

REM QT_WIN32
xcopy /y /e %QT_WIN32%\plugins\imageformats\*.dll  vecvr\prj_win32\x64\Debug\imageformats\
xcopy /y /e %QT_WIN32%\plugins\platforms\*.dll  vecvr\prj_win32\x64\Debug\platforms\


xcopy /y /e %QT_WIN64%\plugins\imageformats\*.dll  vecvr\prj_win32\x64\Release\imageformats\
xcopy /y /e %QT_WIN64%\plugins\platforms\*.dll  vecvr\prj_win32\x64\Release\platforms\

REM Only copy to Release version
xcopy /y /e %QT_WIN64%\bin\*.dll  vecvr\prj_win32\x64\Release\


del  vecvr\prj_win32\x64\Release\*d.dll /f /s /q
del  vecvr\prj_win32\x64\Release\*Web*.dll /f /s /q
del  vecvr\prj_win32\x64\Release\*Test.dll /f /s /q
del  vecvr\prj_win32\x64\Release\*Quick*.dll /f /s /q
del  vecvr\prj_win32\x64\Release\*Multimedia*.dll /f /s /q
del  vecvr\prj_win32\x64\Release\*DBus*.dll /f /s /q
del  vecvr\prj_win32\x64\Release\*Designer*.dll /f /s /q
del  vecvr\prj_win32\x64\Release\*Nfc*.dll /f /s /q
del  vecvr\prj_win32\x64\Release\*Qml*.dll /f /s /q

xcopy /y /e velib\lib\x64\Release\cover64.dll  vecvr\prj_win32\x64\Release
xcopy /y /e velib\lib\x64\Debug\cover64.dll  vecvr\prj_win32\x64\Debug
xcopy /y /e output\windows\vs2013\x64\*.dll  vecvr\prj_win32\x64\Release\


REM =============================
REM ============================================VECVR

