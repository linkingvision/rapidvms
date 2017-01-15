
mkdir client\prj_win32\Win32\Release\translations
mkdir client\prj_win32\Win32\Debug\translations
mkdir client\prj_win32\x64\Release\translations
mkdir client\prj_win32\x64\Debug\translations

xcopy /y /e client\src\main\translations\*.qm  client\prj_win32\Win32\Release\translations
xcopy /y /e client\src\main\translations\*.qm  client\prj_win32\Win32\Debug\translations

xcopy /y /e client\src\main\translations\*.qm  client\prj_win32\x64\Release\translations
xcopy /y /e client\src\main\translations\*.qm  client\prj_win32\x64\Debug\translations


xcopy /y /e veuilib\src\translations\*.qm  client\prj_win32\Win32\Release\translations
xcopy /y /e veuilib\src\translations\*.qm  client\prj_win32\Win32\Debug\translations

xcopy /y /e veuilib\src\translations\*.qm  client\prj_win32\x64\Release\translations
xcopy /y /e veuilib\src\translations\*.qm  client\prj_win32\x64\Debug\translations

pause




