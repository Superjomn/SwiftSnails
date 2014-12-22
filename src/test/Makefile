CXX=g++
CC=gcc
BIN=./bin
CXXFLAGS= -std=c++11 -pthread -Wall -g -DHAVE_PCAP
THIRD_INCPATH=-I../../third/local/glog/include \
	-I../../third/local/openmpi/include \
	-I../../third/local/zeromq/include

THIRD_LIB=../../third/local/glog/lib/libglog.a  \
		-L ../../third/local/openmpi/lib \
		-lmpi_cxx \
		-lmpi

.PHONY:all clean test

all: $(BIN)/utils_string.out \
	 $(BIN)/utils_ConfigParser.out  \
	 $(BIN)/utils_HashFunction.out \
	 $(BIN)/utils_mpi.out \
	 $(BIN)/utils_buffer.out


$(BIN)/utils_string.out : utils/string.cpp ../utils/string.h
	$(CXX) utils/string.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/utils_string.out 

$(BIN)/utils_ConfigParser.out : utils/ConfigParser.cpp ../utils/ConfigParser.h
	$(CXX) utils/ConfigParser.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/utils_ConfigParser.out
	
$(BIN)/utils_HashFunction.out : utils/HashFunction.cpp ../utils/HashFunction.h
	$(CXX) utils/HashFunction.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/utils_HashFunction.out


$(BIN)/utils_mpi.out : utils/mpi.cpp ../utils/mpi.h
	$(CXX) utils/mpi.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/utils_mpi.out

$(BIN)/utils_buffer.out : utils/Buffer.cpp ../utils/Buffer.h
	$(CXX) utils/Buffer.cpp $(CXXFLAGS) $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/utils_buffer.out
