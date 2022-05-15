//
// Created by Szymon Jędras on 5/12/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#pragma once

#include <frame.hpp>

#include <netinet/in.h>

class sender {
    int sockfd;
    sockaddr_in addr;
    int file_size;
    frame output;
public:
    sender(int sockfd, sockaddr_in addr, std::ofstream&& output, int file_size);

    void read_incoming();
    void request_data(int page, int size);
    int run();

    bool recv_and_verify(char *buffer, int i);
};
