set -x
set -e

APP=word2vec

cd ~/
cp SwiftSnails/src/apps/$APP/bin/worker ~/cluster_test/swift_worker
cp SwiftSnails/src/apps/$APP/bin/server ~/cluster_test/swift_server
cp SwiftSnails/src/apps/$APP/bin/master ~/cluster_test/swift_master
