compile glog
==============

    ./configure --prefix=`pwd`/../local/glog
    make && make install

bug: compile error on mac os 10.10

solution: add "-stdlib=libstdc++" to every CC and GCC
