//
// Created by Szymon Jędras on 5/12/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//

#include <sender.hpp>

#include <sys_wrappers.hpp>
#include <utils.hpp>

#include <logger/fwd.hpp>

#include <sys/socket.h>
#include <sys/poll.h>

#include <iostream>
#include <chrono>
#include <cstring>
#include <vector>

using lg = log::logger;

sender::sender(int sockfd, sockaddr_in addr, std::ofstream&& output, int file_size)
    : sockfd(sockfd), addr(addr), file_size(file_size), output(file_size/1000, file_size, std::move(output)) {
}


void sender::read_incoming() {
    char buffer[1024];
    if(recv_and_verify(buffer, 1024)) {
        char *new_line = strstr(buffer, "\n");
        *new_line = 0;
        int start = -1,
                length = -1;
        if (sscanf(buffer, "DATA %d %d", &start, &length) != 2) {
            lg::fatal() << "sscanf failed\n";
            std::exit(EXIT_FAILURE);
        }
        output.record_received(new_line + 1, start / 1000, length);
    }
    else {
        lg::warning() << "Received packet from wrong sender.\n";
    }
}
void sender::request_data(int page, int size) {
    char buffer[24] = {0};
    int n = snprintf(buffer, 24, "GET %d %d\n", page*1000, size);
    if(n < 0) {
        lg::fatal() << "sprintf failed\n";
        std::exit(EXIT_FAILURE);
    }
    lg::debug() << "sending request for #" << page << "\n";
    if(size < 1000)
        lg::info() << "requested packet of size smaller then 1000\n";
    Sendto(sockfd, buffer, n, 0, (const sockaddr*)&addr, sizeof(sockaddr_in));
}

int sender::run() {
    using namespace std::chrono;
    using namespace std::chrono_literals;
    const seconds timeout = 3s;

    auto last_sent = time_point_cast<milliseconds>(high_resolution_clock::now()-timeout);
    while(!output.is_done()) {
        int duration = (int)duration_cast<milliseconds>(last_sent + timeout - high_resolution_clock::now()).count();

        lg::debug() << "entering Poll with wait time: " << std::max(duration, 0) << "ms\n";
        if(output.received_all() || Poll(sockfd, std::max(duration, 0)) == 0) {
            for(auto req : output.get_to_request()) {
                int req_size = std::min(file_size - req*1000, 1000);
                request_data(req, req_size);
            }

            last_sent = time_point_cast<milliseconds>(high_resolution_clock::now());
        }
        else {
            read_incoming();
        }
    }

    return EXIT_SUCCESS;
}

bool sender::recv_and_verify(char *buffer, int i) {
    sockaddr_in received;
    socklen_t len = sizeof(sockaddr_in);
    Recvfrom(sockfd, buffer, 1024, 0, (sockaddr*)&received, &len);
    lg::debug() << "Received packet from " << inet::get_addr(received.sin_addr.s_addr) << ":" << received.sin_port << "\n";
    return received.sin_addr.s_addr == addr.sin_addr.s_addr && received.sin_port == addr.sin_port;
}
