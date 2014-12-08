CXX=g++
CC=gcc
BIN=../bin
CXXFLAGS= -std=c++11 -pthread -Wall -g 
THIRD_INCPATH=-I../third/local/glog/include \
	-I../third/local/openmpi/include \
	-I../third/local/zeromq/include

THIRD_LIB=../third/local/glog/lib/libglog.a  \
		-L /Users/apple/project/SwiftSnails/third/local/openmpi/lib \
		-lmpi_cxx \
		-lmpi

.PHONY:all clean test
