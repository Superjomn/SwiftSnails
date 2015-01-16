CXX=g++
CC=gcc
BIN=./bin
CXXFLAGS= -std=c++11 -pthread -lpthread -Wall -g -DHAVE_PCAP 
THIRD_INCPATH=-I../../third/local/glog/include \
	-I../../third/local/openmpi/include \
	-I../../third/local/zeromq/include \
	-I/usr/local/Cellar/google-sparsehash/2.0.2/include \
	-I/usr/include/pthread

THIRD_LIB=../../third/local/glog/lib/libglog.a  ../../third/local/zeromq/lib/libzmq.a

.PHONY:all clean test cur


all: $(BIN)/utils_string.out \
	 $(BIN)/utils_ConfigParser.out  \
	 $(BIN)/utils_HashFunction.out \
	 $(BIN)/utils_buffer.out \
	 $(BIN)/swiftsnails_threadpool.out \
	 $(BIN)/swiftsnails_asyn_exec.out \
	 $(BIN)/utils_hash_map.out \
	 $(BIN)/swiftsnails_common.out \
	 $(BIN)/utils_spinlock.out \
	 $(BIN)/swiftsnails_client_basicclient.out \
	 $(BIN)/swiftsnails_server_basicserver.out

cur: $(BIN)/swiftsnails_client_masterclient.out $(BIN)/swiftsnails_client_masterserver.out

$(BIN)/utils_string.out : utils/string.cpp ../utils/string.h
	$(CXX) utils/string.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/utils_string.out 

$(BIN)/utils_ConfigParser.out : utils/ConfigParser.cpp ../utils/ConfigParser.h
	$(CXX) utils/ConfigParser.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/utils_ConfigParser.out
	
$(BIN)/utils_HashFunction.out : utils/HashFunction.cpp ../utils/HashFunction.h
	$(CXX) utils/HashFunction.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/utils_HashFunction.out

$(BIN)/utils_mpi.out : utils/mpi.cpp ../utils/mpi.h
	$(CXX) utils/mpi.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/utils_mpi.out

$(BIN)/utils_buffer.out : utils/Buffer.cpp ../utils/Buffer.h
	$(CXX) utils/Buffer.cpp  $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/utils_buffer.out

$(BIN)/swiftsnails_asyn_exec.out : SwiftSnails/AsynExec.cpp ../SwiftSnails/AsynExec.h ../SwiftSnails/BasicChannel.h
	$(CXX) SwiftSnails/AsynExec.cpp  $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/swiftsnails_asyn_exec.out
	
$(BIN)/swiftsnails_threadpool.out : SwiftSnails/ThreadPool.cpp ../SwiftSnails/ThreadPool.h 
	$(CXX) SwiftSnails/ThreadPool.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/swiftsnails_threadpool.out

$(BIN)/utils_hash_map.out : utils/hash_map.cpp
	$(CXX) utils/hash_map.cpp $(CXXFLAGS) $(THIRD_INCPATH)  -o $(BIN)/utils_hash_map.out

$(BIN)/swiftsnails_common.out: SwiftSnails/common.cpp ../SwiftSnails/common.h ../utils/common.h
	$(CXX) SwiftSnails/common.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/swiftsnails_common.out

$(BIN)/swiftsnails_client_basicclient.out : SwiftSnails/client/BasicClient.cpp ../SwiftSnails/client/BasicClient.h ../SwiftSnails/common.h ../utils/common.h
	$(CXX) SwiftSnails/client/BasicClient.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/swiftsnails_client_basicclient.out

$(BIN)/utils_spinlock.out : utils/Spinlock.cpp ../utils/SpinLock.h
	$(CXX) utils/Spinlock.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/utils_spinlock.out

$(BIN)/swiftsnails_server_basicserver.out : SwiftSnails/server/BasicServer.cpp ../SwiftSnails/server/Basic.h
	$(CXX) SwiftSnails/server/BasicServer.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/swiftsnails_server_basicserver.out
# test Master client and server --------
$(BIN)/swiftsnails_client_masterclient.out : SwiftSnails/client/MasterClient.cpp SwiftSnails/client/Master.h ../SwiftSnails/client/BasicClient.h ../SwiftSnails/common.h ../utils/common.h
	$(CXX) SwiftSnails/client/MasterClient.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/swiftsnails_client_masterclient.out

$(BIN)/swiftsnails_client_masterserver.out : SwiftSnails/client/MasterServer.cpp SwiftSnails/client/Master.h ../SwiftSnails/server/Basic.h ../SwiftSnails/common.h ../utils/common.h
	$(CXX) SwiftSnails/client/MasterServer.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/swiftsnails_client_masterserver.out

clean:
	rm $(BIN)/*.out
