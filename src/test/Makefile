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

$(BIN)/swiftsnails_asyn_exec.out : core/AsynExec.cpp ../core/AsynExec.h ../core/BasicChannel.h
	$(CXX) core/AsynExec.cpp  $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/swiftsnails_asyn_exec.out
	
$(BIN)/swiftsnails_threadpool.out : core/ThreadPool.cpp ../core/ThreadPool.h 
	$(CXX) core/ThreadPool.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/swiftsnails_threadpool.out

$(BIN)/utils_hash_map.out : utils/hash_map.cpp
	$(CXX) utils/hash_map.cpp $(CXXFLAGS) $(THIRD_INCPATH)  -o $(BIN)/utils_hash_map.out

$(BIN)/swiftsnails_common.out: core/common.cpp ../core/common.h ../utils/common.h
	$(CXX) core/common.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/swiftsnails_common.out

$(BIN)/swiftsnails_client_basicclient.out : core/client/BasicClient.cpp ../core/client/BasicClient.h ../core/common.h ../utils/common.h
	$(CXX) core/client/BasicClient.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/swiftsnails_client_basicclient.out

$(BIN)/utils_spinlock.out : utils/Spinlock.cpp ../utils/SpinLock.h
	$(CXX) utils/Spinlock.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/utils_spinlock.out

$(BIN)/swiftsnails_server_basicserver.out : core/server/BasicServer.cpp ../core/server/BasicServer.h
	$(CXX) core/server/BasicServer.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/swiftsnails_server_basicserver.out
# test Master client and server --------
$(BIN)/swiftsnails_client_masterclient.out : core/client/MasterClient.cpp core/client/Master.h ../core/client/BasicClient.h ../core/common.h ../utils/common.h
	$(CXX) core/client/MasterClient.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/swiftsnails_client_masterclient.out

$(BIN)/swiftsnails_client_masterserver.out : core/client/MasterServer.cpp core/client/Master.h ../core/server/BasicServer.h ../core/common.h ../utils/common.h
	$(CXX) core/client/MasterServer.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/swiftsnails_client_masterserver.out

clean:
	rm $(BIN)/*.out
