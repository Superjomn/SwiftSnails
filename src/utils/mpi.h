//
//  SpinLock.h
//  SwiftSnails
//
//  Created by Chunwei on 12/2/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//

#ifndef SwiftSnails_utils_mpi_h
#define SwiftSnails_utils_mpi_h
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "common.h"
#include <mpi.h>
namespace swift_snails {

class GlobalMPI : public VirtualObject {
public:
    GlobalMPI() {
        CHECK(0 == MPI_Init(NULL, NULL));
        CHECK(0 == MPI_Comm_rank(comm(), &_rank));
        CHECK(0 == MPI_Comm_size(comm(), &_size));
        _ip_table.assign(IP_WIDTH * _size, 0);
        std::string ip = get_local_ip();
        CHECK((int)ip.length() < IP_WIDTH);
        strcpy(&_ip_table[IP_WIDTH * _rank], ip.c_str());
        CHECK(0 == MPI_Allgather(MPI_IN_PLACE, 0, MPI_BYTE, &_ip_table[0], IP_WIDTH, MPI_BYTE, comm()));
    }

    ~GlobalMPI() {
        MPI_Finalize();
    }

    MPI_Comm comm() {
        return MPI_COMM_WORLD;
    }

    int rank() {
        return _rank;
    }

    int size() {
        return _size;
    }

    const char* ip() {
        return &_ip_table[rank() * IP_WIDTH];
    }

    const char* ip(int rank) {
        return &_ip_table[rank * IP_WIDTH];
    }

    void barrier() {
        CHECK(0 == MPI_Barrier(comm()));
    }
private:
    const int IP_WIDTH = 64;
    int _rank;
    int _size;
    std::vector<char> _ip_table;

    std::string get_local_ip() {
        int sockfd;
        char buf[512];
        struct ifconf ifconf;
        struct ifreq* ifreq;

        ifconf.ifc_len = 512;
        ifconf.ifc_buf = buf;
        PCHECK((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0);
        PCHECK(ioctl(sockfd, SIOCGIFCONF, &ifconf) >= 0);
        PCHECK(0 == close(sockfd));

        ifreq = (struct ifreq*)buf;
        for (int i = 0; i < int(ifconf.ifc_len / sizeof(struct ifreq)); i++) {
            std::string ip;
            ip = inet_ntoa(((struct sockaddr_in*)&ifreq->ifr_addr)->sin_addr);
            if (ip != "127.0.0.1") {
                return ip;
            }
            ifreq++;
        }
        LOG(FATAL) << "IP not found";
        return "";
    }

}; // end class GlobalMPI

inline GlobalMPI& global_mpi() {
    static GlobalMPI mpi;
    return mpi;
}


}; // end namespace swift_snails
#endif
