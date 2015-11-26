REM ============================================VECVR
REM =============================
rmdir  vecvr\prj_win32\Debug /s /q
rmdir  vecvr\prj_win32\Debug_x64 /s /q
rmdir  vecvr\prj_win32\Release /s /q
rmdir  vecvr\prj_win32\Release_x64 /s /q

rmdir  vecvr\prj_win32\GeneratedFiles /s /q
rmdir  vecvr\prj_win32\ipch /s /q

REM Win32
del  vecvr\prj_win32\Win32\Debug\*.* /f /s /q
del  vecvr\prj_win32\Win32\Release\*.* /f /s /q

rmdir  vecvr\prj_win32\Win32\Debug\imageformats /s /q
rmdir  vecvr\prj_win32\Win32\Debug\platforms /s /q
rmdir  vecvr\prj_win32\Win32\Debug\translations /s /q

rmdir  vecvr\prj_win32\Win32\Release\imageformats /s /q
rmdir  vecvr\prj_win32\Win32\Release\platforms /s /q
rmdir  vecvr\prj_win32\Win32\Release\translations /s /q

REM x64
del  vecvr\prj_win32\x64\Debug\*.* /f /s /q
del /f /s /q vecvr\prj_win32\x64\Release\*.* /f /s /q

rmdir  vecvr\prj_win32\x64\Debug\imageformats /s /q
rmdir  vecvr\prj_win32\x64\Debug\platforms /s /q
rmdir  vecvr\prj_win32\x64\Debug\translations /s /q

rmdir  vecvr\prj_win32\x64\Release\imageformats /s /q
rmdir  vecvr\prj_win32\x64\Release\platforms /s /q
rmdir  vecvr\prj_win32\x64\Release\translations /s /q
REM =============================
REM ============================================VECVR


REM ============================================Client
REM =============================
rmdir  client\prj_win32\Debug /s /q
rmdir  client\prj_win32\Debug_x64 /s /q
rmdir  client\prj_win32\Release /s /q
rmdir  client\prj_win32\Release_x64 /s /q

rmdir  client\prj_win32\GeneratedFiles /s /q
rmdir  client\prj_win32\ipch /s /q

REM Win32
del  client\prj_win32\Win32\Debug\*.* /f /s /q
del  client\prj_win32\Win32\Release\*.* /f /s /q

rmdir  client\prj_win32\Win32\Debug\imageformats /s /q
rmdir  client\prj_win32\Win32\Debug\platforms /s /q
rmdir  client\prj_win32\Win32\Debug\translations /s /q

rmdir  client\prj_win32\Win32\Release\imageformats /s /q
rmdir  client\prj_win32\Win32\Release\platforms /s /q
rmdir  client\prj_win32\Win32\Release\translations /s /q

REM x64
del  client\prj_win32\x64\Debug\*.* /f /s /q
del /f /s /q client\prj_win32\x64\Release\*.* /f /s /q

rmdir  client\prj_win32\x64\Debug\imageformats /s /q
rmdir  client\prj_win32\x64\Debug\platforms /s /q
rmdir  client\prj_win32\x64\Debug\translations /s /q

rmdir  client\prj_win32\x64\Release\imageformats /s /q
rmdir  client\prj_win32\x64\Release\platforms /s /q
rmdir  client\prj_win32\x64\Release\translations /s /q
REM =============================
REM ============================================Client
