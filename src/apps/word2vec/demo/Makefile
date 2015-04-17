#CXX=g++-4.9
CXX=g++
CC=gcc
BIN=./bin

#CXXFLAGS= -static -g -std=c++11 -pthread -lpthread -Wall -DHAVE_PCAP -lgtest -lgtest_main
CXXFLAGS= -O3 -g -pipe  \
  -lpthread -std=c++11 -pthread -DHAVE_PCAP -lgtest -lgtest_main  -lzmq -lglog \
  -Wextra -Wall -Wno-parentheses \
  -Wno-literal-suffix \
  -Wno-unused-parameter \
  -Wno-unused-local-typedefs 

LOCAL_ROOT=../../../third/local
GTEST_HOME=$(LOCAL_ROOT)/gtest

THIRD_INCPATH=-I$(LOCAL_ROOT)/glog/include \
	-I$(LOCAL_ROOT) \
	-I$(LOCAL_ROOT)/zeromq/include \
	-I/usr/local/Cellar/google-sparsehash/2.0.2/include \
	-I/usr/include/pthread \
	-I$(GTEST_HOME)/include

THIRD_LIB=-L$(LOCAL_ROOT)/glog/lib/  -L$(LOCAL_ROOT)/zeromq/lib/

.PHONY:all clean test cur

ALL: master worker server

master: master.cpp
	$(CXX) master.cpp $(THIRD_INCPATH) -L$(GTEST_HOME)/lib/.libs -Xlinker $(THIRD_LIB)  $(CXXFLAGS) -o $(BIN)/master

worker: worker.cpp
	$(CXX) worker.cpp $(THIRD_INCPATH) -L$(GTEST_HOME)/lib/.libs -Xlinker $(THIRD_LIB)  $(CXXFLAGS) -o $(BIN)/worker

server: server.cpp
	$(CXX) server.cpp $(THIRD_INCPATH) -L$(GTEST_HOME)/lib/.libs -Xlinker $(THIRD_LIB)  $(CXXFLAGS) -o $(BIN)/server

local: local_train.cpp
	$(CXX) local_train.cpp $(THIRD_INCPATH) -L$(GTEST_HOME)/lib/.libs -Xlinker $(THIRD_LIB)  $(CXXFLAGS) -o $(BIN)/local


clean:
	rm -f $(BIN)/*
