REM ============vecvr

rmdir  vecvr\prj_win32\Debug /s /q
rmdir  vecvr\prj_win32\Debug_x64 /s /q
rmdir  vecvr\prj_win32\Release /s /q
rmdir  vecvr\prj_win32\Release_x64 /s /q

rmdir  vecvr\prj_win32\GeneratedFiles /s /q
rmdir  vecvr\prj_win32\ipch /s /q

del /f /s /q vecvr\prj_win32\Win32\Debug\*.*
del /f /s /q vecvr\prj_win32\Win32\Release\*.*


pause