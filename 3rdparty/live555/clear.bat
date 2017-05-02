
attrib /s -h -a -s -r *.suo
del /s *.aps *.ncb *.obj *.opt *.plg  *.clw *.suo *.orig *.sdf *.tlog *.ipch *.o *.sbr *.bsc 
del /s *.~ *.dat *.pdb *.ilk *.pch *.idb *.dep *.exp
del /s *.manifest *.manifest.res *.res *.o
del /s BuildLog.htm *.log *.tlog

FOR /D /R %%X IN (*.tlog) DO RD /S /Q "%%X"

rmdir %~dp0\Debug /s /q
rmdir %~dp0\Release /s /q
rmdir %~dp0\x64 /s /q


