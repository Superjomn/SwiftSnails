set -x

nohup sh hadoop-server.sh >server.log 2>&1 &
nohup sh hadoop-worker.sh >worker.log 2>&1 &
nohup sh run_master.sh > master.log 2>&1 &

tail -f master.log
