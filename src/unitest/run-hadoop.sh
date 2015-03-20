set -x

BIN="/Users/apple/project/SwiftSnails/src/bin"

hadoop fs -rmr test/8.txt

hadoop jar ~/LocalApps/hadoop/contrib/streaming/hadoop-streaming-1.2.1.jar \
    -D mapred.map.tasks=6 \
    -D mapred.job.map.capacity=30 \
    -input test/1.txt \
    -output test/8.txt \
    -file "$BIN/../worker.conf" \
    -file "$BIN/worker" \
    -mapper "./worker -config ./worker.conf" \
    -reducer "NONE"
