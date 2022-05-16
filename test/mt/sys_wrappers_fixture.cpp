//
// Created by Szymon Jędras on 5/14/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//

#include <sys_wrappers.hpp>

#include <queue>
#include <fstream>
#include <memory>
#include <random>
#include <optional>

class fixture {
    std::ofstream output;
    std::queue<std::pair<int,int>> pending;
    std::unique_ptr<sockaddr_in> addr;
    std::uniform_int_distribution<int> dist{0,99};
    std::optional<std::mt19937> prng;

    int sockfd;
public:
    fixture() = default;
    explicit fixture(const char* file_path, int fd)
        : output(file_path)
        , sockfd(fd)
        , prng(0) { }

    void make_chances(int start, int length) {
        if(prng) {
            int var = dist(*prng);
            if(var < 2) {
                pending.emplace(start, length);
                pending.emplace(start, length);
                pending.emplace(start, length);
            }
            if(var < 10) {
                pending.emplace(start, length);
                pending.emplace(start, length);
            }
            if(var < 50) {
                pending.emplace(start, length);
            }
        }
        else {
            pending.emplace(start, length);
        }
    }

    long register_message(char* ptr, size_t n, const sockaddr_in* in_addr) {
        char c = ptr[n-1];
        ptr[n-1] = 0;
        int start = -1,
            length = -1;
        output << ptr << "\n";
        if (sscanf((char*)ptr, "GET %d %d", &start, &length) == 2) {
            if(!this->addr && in_addr != nullptr) {
                addr = std::make_unique<sockaddr_in>(sockaddr_in{
                    .sin_family = in_addr->sin_family,
                    .sin_port = in_addr->sin_port,
                    .sin_addr = {in_addr->sin_addr.s_addr},
                    .sin_zero = {}
                });
            }
            make_chances(start, length);
        }
        ptr[n-1] = c;
        return n;
    }

    long pop_message(void* ptr, size_t n, sockaddr_in* in_addr) {
        auto [start, length] = pending.front();
        pending.pop();
        *in_addr = *this->addr;
        int res = snprintf((char*)ptr, n, "DATA %d %d\n", start, length);
        if(res > n || res < 0) {
            in_addr->sin_addr.s_addr++;
        }
        return res;
    }

    int poll() {
        return pending.size() > 0;
    }

    int get_fd() {
        return sockfd;
    }
};
fixture singleton;

int Bind(int fd, __CONST_SOCKADDR_ARG addr, socklen_t len) {
    return 0;
}
int Socket(int domain, int type, int protocol) {
    singleton = fixture("fixture_output.txt", 10);
    return 10;
}

long Recvfrom(int fd, void *buf, size_t n, int flags, struct sockaddr *addr, socklen_t *addr_len) {
    if(fd == singleton.get_fd()) {
        return singleton.pop_message(buf, n, (sockaddr_in*)addr);
    }
    errno = EBADF;
    return -1;
}

long Sendto(int fd, const void *buf, size_t n, int flags, const struct sockaddr *addr, socklen_t addr_len) {
    if(fd == singleton.get_fd()) {
        return singleton.register_message((char*)buf, n, (sockaddr_in*)addr);
    }
    errno = EBADF;
    return -1;
}

int Poll(int sockfd, int timeout) {
    if(sockfd == singleton.get_fd()) {
        return singleton.poll();
    }
    errno = EBADF;
    return -1;
}
