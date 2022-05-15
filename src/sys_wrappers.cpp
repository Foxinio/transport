//
// Created by Szymon Jędras on 5/14/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <sys_wrappers.hpp>

#include <logger/fwd.hpp>
#include <utils.hpp>

#include <sys/socket.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>


using lg = log::logger;

int Bind(int fd, __CONST_SOCKADDR_ARG addr, socklen_t len) {
    int res = bind(fd, addr, len);
    if(res < 0) {
        perror("bind error");
        std::exit(EXIT_FAILURE);
    }
    return res;
}
int Socket(int domain, int type, int protocol) {
    int res = socket(domain, type, protocol);
    if(res < 0) {
        perror("poll error");
        std::exit(EXIT_FAILURE);
    }
    return res;
}


int Poll(int sockfd, int timeout) {
    pollfd fds = {
        .fd=sockfd,
        .events=POLLIN,
        .revents=0
    };

    int res = poll(&fds, 1, timeout);
    if(res < 0) {
        perror("poll error");
        std::exit(EXIT_FAILURE);
    }
    return res;
}

long Recvfrom(int fd, void *buf, size_t n, int flags, sockaddr *addr, socklen_t *addr_len) {
    lg::debug() << "received packet from " << inet::get_addr(((sockaddr_in*)addr)->sin_addr.s_addr)
                << ":" << ((sockaddr_in*)addr)->sin_port << "\n";
    long res = recvfrom(fd, buf, n, flags, addr, addr_len);
    if(res < 0 && !(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)) {
        perror("recvfrom error");
        std::exit(EXIT_FAILURE);
    }
    return res;
}

long Sendto(int fd, const void *buf, size_t n, int flags, const sockaddr *addr, socklen_t addr_len) {
    lg::debug() << "sending packet to " << inet::get_addr(((sockaddr_in*)addr)->sin_addr.s_addr)
                << ":" << ((sockaddr_in*)addr)->sin_port << "\n";
    long res = sendto(fd, buf, n, flags, addr, addr_len);
    if(res < 0 && errno == EINTR) {
        res = sendto(fd, buf, n, flags, addr, addr_len);
    }
    return res;
}
