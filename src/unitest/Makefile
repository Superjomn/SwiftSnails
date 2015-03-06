CXX=g++
CC=gcc
BIN=./bin

CXXFLAGS= -std=c++11 -pthread -lpthread -Wall -g -DHAVE_PCAP -lgtest -lgtest_main

LOCAL_ROOT=../../third/local
GTEST_HOME=$(LOCAL_ROOT)/gtest

THIRD_INCPATH=-I$(LOCAL_ROOT)/glog/include \
	-I$(LOCAL_ROOT)/zeromq/include \
	-I/usr/local/Cellar/google-sparsehash/2.0.2/include \
	-I/usr/include/pthread \
	-I$(GTEST_HOME)/include

THIRD_LIB=$(LOCAL_ROOT)/glog/lib/libglog.a  $(LOCAL_ROOT)/zeromq/lib/libzmq.a 


$(BIN)/test :  main.cpp \
	utils/Buffer_test.h ../utils/Buffer.h \
	utils/string_test.h ../utils/string.h \
	utils/queue_test.h  ../utils/queue.h
	$(CXX) main.cpp $(THIRD_INCPATH) -L$(GTEST_HOME)/lib/.libs -Xlinker $(THIRD_LIB)  $(CXXFLAGS) -o $(BIN)/test


