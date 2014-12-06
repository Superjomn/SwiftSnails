#/bin/bash
set -x -e

mkdir -p local
cd glog-0.3.3/
./configure --prefix=`pwd`/../local/glog
make && make install

cd ..
cd openmpi-1.4.5/
./configure --prefix=`pwd`/../local/openmpi
make && make install

cd ..
cd zeromq-4.0.5/
./configure --prefix=`pwd`/../local/zeromq
make && make install
