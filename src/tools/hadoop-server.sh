set -x

HADOOP_ROOT="/home/chunwei/hadoop/"

hadoop fs -rmr test/2.test

APP="../SwiftSnails/src/apps/logistic_regression"

$HADOOP_ROOT/bin/hadoop jar $HADOOP_ROOT/contrib/streaming/hadoop-streaming-1.2.1.jar \
    -D mapred.map.tasks=3 \
    -D mapred.reduce.tasks=6 \
    -D mapred.job.map.capacity=30 \
    -D mapred.job.reduce.capacity=10 \
    -D mapreduce.map.maxattempts=1 \
    -D mapreduce.reduce.maxattempts=1 \
    -input test/1.txt \
    -output test/2.test \
    -file "server.conf" \
    -file "word2vec.conf" \
    -file "./swift_server" \
    -file "run_server.sh" \
    -mapper "cat" \
    -reducer "./run_server.sh"
