REM ============================================client
REM =============================
set QT_WIN32=C:\Qt\Qt5.5.1\5.5\msvc2013\
REM Win32 Debug
xcopy /y /e 3rdparty\ffmpeg-win\ffmpeg-win32\bin  client\prj_win32\Win32\Debug

xcopy /y /e 3rdparty\poco\bin\PocoDatad.dll  client\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\poco\bin\PocoDataSQLited.dll  client\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\poco\bin\PocoFoundationd.dll  client\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\poco\bin\PocoJSONd.dll  client\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\poco\bin\PocoNetd.dll  client\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\poco\bin\PocoUtild.dll  client\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\poco\bin\PocoXMLd.dll  client\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\poco\bin\PocoZipd.dll  client\prj_win32\Win32\Debug

xcopy /y /e 3rdparty\openssl-win\openssl-win32\lib\libeay32.dll  client\prj_win32\Win32\Debug
xcopy /y /e 3rdparty\openssl-win\openssl-win32\lib\ssleay32.dll  client\prj_win32\Win32\Debug

xcopy /y /e 3rdparty\protobuf\cmake\win32\bin\libprotobufd.dll  client\prj_win32\Win32\Debug



REM Win32 Release
xcopy /y /e 3rdparty\ffmpeg-win\ffmpeg-win32\bin  client\prj_win32\Win32\Release

xcopy /y /e 3rdparty\poco\bin\PocoData.dll  client\prj_win32\Win32\Release
xcopy /y /e 3rdparty\poco\bin\PocoDataSQLite.dll  client\prj_win32\Win32\Release
xcopy /y /e 3rdparty\poco\bin\PocoFoundation.dll  client\prj_win32\Win32\Release
xcopy /y /e 3rdparty\poco\bin\PocoJSON.dll  client\prj_win32\Win32\Release
xcopy /y /e 3rdparty\poco\bin\PocoNet.dll  client\prj_win32\Win32\Release
xcopy /y /e 3rdparty\poco\bin\PocoUtil.dll  client\prj_win32\Win32\Release
xcopy /y /e 3rdparty\poco\bin\PocoXML.dll  client\prj_win32\Win32\Release
xcopy /y /e 3rdparty\poco\bin\PocoZip.dll  client\prj_win32\Win32\Release

xcopy /y /e 3rdparty\openssl-win\openssl-win32\lib\libeay32.dll  client\prj_win32\Win32\Release
xcopy /y /e 3rdparty\openssl-win\openssl-win32\lib\ssleay32.dll  client\prj_win32\Win32\Release

xcopy /y /e 3rdparty\protobuf\cmake\win32\bin\libprotobuf.dll  client\prj_win32\Win32\Release

REM QT_WIN32
xcopy /y /e %QT_WIN32%\plugins\imageformats\*.dll  client\prj_win32\Win32\Debug\imageformats\
xcopy /y /e %QT_WIN32%\plugins\platforms\*.dll  client\prj_win32\Win32\Debug\platforms\


xcopy /y /e %QT_WIN32%\plugins\imageformats\*.dll  client\prj_win32\Win32\Release\imageformats\
xcopy /y /e %QT_WIN32%\plugins\platforms\*.dll  client\prj_win32\Win32\Release\platforms\

REM Only copy to Release version
xcopy /y /e %QT_WIN32%\bin\*.dll  client\prj_win32\Win32\Release\

del  client\prj_win32\Win32\Release\*d.dll /f /s /q
del  client\prj_win32\Win32\Release\*Web*.dll /f /s /q
del  client\prj_win32\Win32\Release\*Test.dll /f /s /q
del  client\prj_win32\Win32\Release\*Quick*.dll /f /s /q
del  client\prj_win32\Win32\Release\*Multimedia*.dll /f /s /q
del  client\prj_win32\Win32\Release\*DBus*.dll /f /s /q
del  client\prj_win32\Win32\Release\*Designer*.dll /f /s /q
del  client\prj_win32\Win32\Release\*Nfc*.dll /f /s /q
del  client\prj_win32\Win32\Release\*Qml*.dll /f /s /q

REM x64

REM =============================
REM ============================================client

