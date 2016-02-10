REM ============================================VECVR
REM =============================
set QT_WIN32=C:\Qt\Qt5.5.1\5.5\msvc2013\
REM Win32 Debug
xcopy /y /e 3rdparty\ffmpeg-win\ffmpeg-win32\bin  vecvr\prj_win32\Win32\Debug

xcopy /y /e 3rdparty\poco\bin\PocoDatad.dll  vecvr\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\poco\bin\PocoDataSQLited.dll  vecvr\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\poco\bin\PocoFoundationd.dll  vecvr\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\poco\bin\PocoJSONd.dll  vecvr\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\poco\bin\PocoNetd.dll  vecvr\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\poco\bin\PocoUtild.dll  vecvr\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\poco\bin\PocoXMLd.dll  vecvr\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\poco\bin\PocoZipd.dll  vecvr\prj_win32\Win32\Debug

xcopy /y /e 3rdparty\openssl-win\openssl-win32\lib\libeay32.dll  vecvr\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\openssl-win\openssl-win32\lib\ssleay32.dll  vecvr\prj_win32\Win32\Debug

xcopy /y /e 3rdparty\protobuf\cmake\win32\bin\libprotobufd.dll  vecvr\prj_win32\Win32\Debug



REM Win32 Release
xcopy /y /e 3rdparty\ffmpeg-win\ffmpeg-win32\bin  vecvr\prj_win32\Win32\Release

xcopy /y /e 3rdparty\poco\bin\PocoData.dll  vecvr\prj_win32\Win32\Release
xcopy /y /e 3rdparty\poco\bin\PocoDataSQLite.dll  vecvr\prj_win32\Win32\Release
xcopy /y /e 3rdparty\poco\bin\PocoFoundation.dll  vecvr\prj_win32\Win32\Release
xcopy /y /e 3rdparty\poco\bin\PocoJSON.dll  vecvr\prj_win32\Win32\Release
xcopy /y /e 3rdparty\poco\bin\PocoNet.dll  vecvr\prj_win32\Win32\Release
xcopy /y /e 3rdparty\poco\bin\PocoUtil.dll  vecvr\prj_win32\Win32\Release
xcopy /y /e 3rdparty\poco\bin\PocoXML.dll  vecvr\prj_win32\Win32\Release
xcopy /y /e 3rdparty\poco\bin\PocoZip.dll  vecvr\prj_win32\Win32\Release

xcopy /y /e 3rdparty\openssl-win\openssl-win32\lib\libeay32.dll  vecvr\prj_win32\Win32\Release
xcopy /y /e 3rdparty\openssl-win\openssl-win32\lib\ssleay32.dll  vecvr\prj_win32\Win32\Release

REM xcopy /y /e 3rdparty\protobuf\cmake\win32\bin\libprotobuf.dll  vecvr\prj_win32\Win32\Release

REM QT_WIN32
xcopy /y /e %QT_WIN32%\plugins\imageformats\*.dll  vecvr\prj_win32\Win32\Debug\imageformats\
xcopy /y /e %QT_WIN32%\plugins\platforms\*.dll  vecvr\prj_win32\Win32\Debug\platforms\


xcopy /y /e %QT_WIN32%\plugins\imageformats\*.dll  vecvr\prj_win32\Win32\Release\imageformats\
xcopy /y /e %QT_WIN32%\plugins\platforms\*.dll  vecvr\prj_win32\Win32\Release\platforms\

REM Only copy to Release version
xcopy /y /e %QT_WIN32%\bin\*.dll  vecvr\prj_win32\Win32\Release\

del  vecvr\prj_win32\Win32\Release\*d.dll /f /s /q
del  vecvr\prj_win32\Win32\Release\*Web*.dll /f /s /q
del  vecvr\prj_win32\Win32\Release\*Test.dll /f /s /q
del  vecvr\prj_win32\Win32\Release\*Quick*.dll /f /s /q
del  vecvr\prj_win32\Win32\Release\*Multimedia*.dll /f /s /q
del  vecvr\prj_win32\Win32\Release\*DBus*.dll /f /s /q
del  vecvr\prj_win32\Win32\Release\*Designer*.dll /f /s /q
del  vecvr\prj_win32\Win32\Release\*Nfc*.dll /f /s /q
del  vecvr\prj_win32\Win32\Release\*Qml*.dll /f /s /q

xcopy /y /e velib\lib\win32\Release\cover.dll  vecvr\prj_win32\Win32\Release
xcopy /y /e velib\lib\win32\Debug\cover.dll  vecvr\prj_win32\Win32\Debug

xcopy /y /e output\windows\vs2013\x86\*.dll  vecvr\prj_win32\Win32\Release



REM =============================
REM ============================================VECVR

