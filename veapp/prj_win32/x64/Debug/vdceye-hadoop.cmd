:: Turn off script echoing.
@echo off

:: Set your env to here
set HADOOP_HOME=C:\hadoop-2.6.0
set JAVA_HOME=C:\Program Files\Java\jdk1.8.0_25

cd /d %~dp0

:: Set Hadoop Env

set PATH=%PATH%;%JAVA_HOME%\bin;%JAVA_HOME%\jre\bin;%JAVA_HOME%\jre\bin\server

:: Set the vdceye home
set VEH_HOME=C:\veh
::set VEH_HOME=%HADOOP_HOME%\share\hadoop\

rmdir  /q %VEH_HOME%
mklink /d %VEH_HOME% %HADOOP_HOME%\share\hadoop\

set HADOOP_CLASSPATH_COMMON=C:\
set HADOOP_COMMON=%VEH_HOME%\common\
pushd %HADOOP_COMMON%
for /R %%i in (*.jar) do (call :concat1 %%i)
popd

set HADOOP_CLASSPATH_HDFS=C:\
set HADOOP_HDFS=%VEH_HOME%\hdfs\
pushd %HADOOP_HDFS%
for /R %%i in (*.jar) do (call :concat2 %%i)
popd


set CLASSPATH=.;%JAVA_HOME%\lib;%JAVA_HOME%\lib\dt.jar;%JAVA_HOME%\lib\tools.jar;%HADOOP_CLASSPATH_COMMON%;%HADOOP_CLASSPATH_HDFS%

echo %CLASSPATH%

.\"vdceye Manager x64.exe" 


:concat1
    set file=%1
    set HADOOP_CLASSPATH_COMMON=%HADOOP_CLASSPATH_COMMON%;%1
	
:concat2
    set file=%1
    set HADOOP_CLASSPATH_HDFS=%HADOOP_CLASSPATH_HDFS%;%1

