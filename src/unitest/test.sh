set -x

LOCAL_ROOT="../../third/local/"
GTEST_HOME="${LOCAL_ROOT}/gtest"

export LD_LIBRARY_PATH=${GTEST_HOME}/lib/.libs:$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=${GTEST_HOME}/lib/.libs 

make clean
make

#gdb-apple ./bin/test
./bin/test
