
export HADOOP_HOME=/home/user/work/hdp/hadoop-2.6.0/
export JAVA_HOME=/home/user/work/hdp/jdk1.8.0_25/

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$JAVA_HOME/jre/lib/amd64/server/

CLASSPATH=${CLASSPATH}":"$JAVA_HOME\lib":"$JAVA_HOME\lib\dt.jar":"$JAVA_HOME\lib\tools.jar":"`find ${HADOOP_HOME}/share/hadoop | awk '{path=path":"$0}END{print path}'` # jar of hadoop
export CLASSPATH
#echo $CLASSPATH

mkdir -p ./ve/videodb/

./bin/vdceyeManager
