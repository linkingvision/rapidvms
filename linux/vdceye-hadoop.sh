
export HADOOP_HOME=/home/hadoop-2.6.0
export JAVA_HOME=/home/hadoop-2.6.0/Java/jdk1.8.0_25

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$JAVA_HOME/jre/lib/amd64/server/

CLASSPATH=./
for f in $HADOOP_HOME/*.jar; do
  CLASSPATH=${CLASSPATH}:$f;
done
for f in $HADOOP_HOME/lib/*.jar; do
  CLASSPATH=${CLASSPATH}:$f;
done
export CLASSPATH=$CLASSPATH

mkdir -p ./ve/videodb/

./bin/vdceyeManager
