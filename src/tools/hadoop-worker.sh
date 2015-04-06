set -x

HADOOP_ROOT="/home/chunwei/hadoop/"

hadoop fs -rmr "/user/chunwei/test/1.test"

$HADOOP_ROOT/bin/hadoop jar $HADOOP_ROOT/contrib/streaming/hadoop-streaming-1.2.1.jar \
    -D mapred.map.tasks=5 \
    -D mapred.reduce.tasks=8 \
    -D mapred.job.map.capacity=30 \
    -D mapred.job.reduce.capacity=10 \
    -D mapreduce.map.maxattempts=1 \
    -D mapreduce.reduce.maxattempts=1 \
    -input "/user/chunwei/test/test.data" \
    -output "/user/chunwei/test/1.test" \
    -file "worker.conf" \
    -file "./swift_worker" \
    -file "word2vec.conf" \
    -file "run_worker.sh" \
    -mapper "cat" \
    -reducer "./run_worker.sh"
