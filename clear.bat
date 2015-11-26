attrib /s -h -a -s -r *.suo
del /s *.aps *.ncb *.obj *.opt *.plg  *.clw *.suo *.orig *.sdf *.tlog *.ipch *.o *.sbr *.bsc
del /s *.~ *.dat *.pdb *.ilk *.pch *.idb *.dep
del /s *.manifest *.manifest.res *.res *.o
del /s BuildLog.htm 

FOR /D %%p IN ("xcmnlib\prj_win32\*.*") DO rmdir "%%p" /s /q
FOR /D %%p IN ("veuilib\prj_win32\*.*") DO rmdir "%%p" /s /q
FOR /D %%p IN ("veuilib\onvifcpplib\prj_win32\*.*") DO rmdir "%%p" /s /q
FOR /D %%p IN ("velib\prj_win32\*.*") DO rmdir "%%p" /s /q

call scripts\dllclear.bat

pause

