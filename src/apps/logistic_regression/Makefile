CXX=g++-4.9
CC=gcc
BIN=./bin

#CXXFLAGS= -static -g -std=c++11 -pthread -lpthread -Wall -DHAVE_PCAP -lgtest -lgtest_main
CXXFLAGS= -g -pipe -Wextra -Wall -Wno-parentheses \
  -Wno-literal-suffix \
  -Wno-unused-parameter \
  -Wno-unused-local-typedefs \
-pthread -lpthread -DHAVE_PCAP -lgtest -lgtest_main -std=c++11 

LOCAL_ROOT=../../../third/local
GTEST_HOME=$(LOCAL_ROOT)/gtest

THIRD_INCPATH=-I$(LOCAL_ROOT)/glog/include \
	-I$(LOCAL_ROOT) \
	-I$(LOCAL_ROOT)/zeromq/include \
	-I/usr/local/Cellar/google-sparsehash/2.0.2/include \
	-I/usr/include/pthread \
	-I$(GTEST_HOME)/include

THIRD_LIB=$(LOCAL_ROOT)/glog/lib/libglog.a  $(LOCAL_ROOT)/zeromq/lib/libzmq.a 

.PHONY:all clean test cur

ALL: master worker

master: master.cpp
	$(CXX) master.cpp $(THIRD_INCPATH) -L$(GTEST_HOME)/lib/.libs -Xlinker $(THIRD_LIB)  $(CXXFLAGS) -o $(BIN)/master

worker: worker.cpp
	$(CXX) worker.cpp $(THIRD_INCPATH) -L$(GTEST_HOME)/lib/.libs -Xlinker $(THIRD_LIB)  $(CXXFLAGS) -o $(BIN)/worker

server: server.cpp
	$(CXX) server.cpp $(THIRD_INCPATH) -L$(GTEST_HOME)/lib/.libs -Xlinker $(THIRD_LIB)  $(CXXFLAGS) -o $(BIN)/server


clean:
	rm -f $(BIN)/*
