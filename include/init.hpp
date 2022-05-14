//
// Created by Szymon Jędras on 5/14/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#pragma once

#include <netinet/in.h>

#include <fstream>

class init {
public:
    int sockfd;
    sockaddr_in addr;
    std::ofstream out_file;
    int no_bits;

    init(int argc, char* argv[]);
};
