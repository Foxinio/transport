//
// Created by Szymon Jędras on 5/14/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//

#include <sys_wrappers.hpp>

#include <queue>
#include <vector>
#include <fstream>
#include <memory>
#include <random>
#include <optional>
#include <chrono>
#include <thread>
#include <stdexcept>

using namespace std::chrono;
using namespace std::chrono_literals;

class fixture {
    struct message {
        int start;
        int length;
        time_point<high_resolution_clock, duration<float>> extraction_time;
        message(int start, int length, time_point<high_resolution_clock, duration<float>> extraction_time)
            : start(start), length(length), extraction_time(extraction_time) {}
        message() = default;
    };
    class message_comparator{
    public:
        bool operator()(const message& l, const message& r) {
            return l.extraction_time < r.extraction_time;
        }
    };

    std::ofstream output;
    std::priority_queue<message, std::vector<message>, message_comparator> pending;
    std::unique_ptr<sockaddr_in> addr;
    std::uniform_int_distribution<int> dist{0,99};
    std::uniform_real_distribution<float> time_dist{0.5, 1.5};
    std::optional<std::mt19937> prng;

    int sockfd;
public:
    fixture() = default;

    explicit fixture(const char* file_path, int fd)
        : output(file_path)
        , sockfd(fd)
        , prng(0) { }

    void random_time_insert(int start, int length) {
        time_point<high_resolution_clock, duration<float>> extraction_time =
                high_resolution_clock::now() + duration<float>(time_dist(*prng));
//        if(pending.size() > 1000)
//            output << "\t[ERROR] Pending overfilled: " << pending.size() << "\n";
        pending.emplace(start, length, extraction_time);
    }

    void make_chances(int start, int length) {
        if(!prng)
            throw std::runtime_error("usage of not initialized variable.");

        int var = dist(*prng);
        if(var < 10) {
            random_time_insert(start, length);
            make_chances(start, length);
        }
        if(var < 50) {
            random_time_insert(start, length);
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

    long pop_message(char* ptr, size_t n, sockaddr_in* in_addr) {
        auto [start, length, extraction] = pending.top();
        if(extraction > high_resolution_clock::now())
            throw std::runtime_error("tried to extract message before it was ready\n");
        pending.pop();
        *in_addr = *this->addr;
        int res = snprintf(ptr, n, "DATA %d %d\n", start, length);
        if(res > n || res < 0) {
            in_addr->sin_addr.s_addr++;
        }
        else {
            int res1 = snprintf(ptr+res, n-res, "Start:%d/%d ", start, length);
//            std::fill(ptr+res+res1, ptr+n, 'a'+(start/1000)%24);
            std::fill(ptr+res+res1, ptr+n, ' ');
            ptr[res+length-1] = '\n';
        }
        return res;
    }

    int poll(int timeout) {
        if(!pending.empty()) {
            if(high_resolution_clock::now() + milliseconds(timeout) < pending.top().extraction_time) {
                std::this_thread::sleep_for(milliseconds(timeout));
                return 0;
            }
            else {
                std::this_thread::sleep_until(pending.top().extraction_time);
                return 1;
            }
        }
        return 0;
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
    singleton = fixture("../../../fixture_output.txt", 10);
    return 10;
}

long Recvfrom(int fd, void *buf, size_t n, int flags, struct sockaddr *addr, socklen_t *addr_len) {
    if(fd == singleton.get_fd()) {
        return singleton.pop_message((char*)buf, n, (sockaddr_in*)addr);
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
        return singleton.poll(timeout);
    }
    errno = EBADF;
    return -1;
}
