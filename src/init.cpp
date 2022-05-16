//
// Created by Szymon Jędras on 5/14/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <init.hpp>

#include <sys_wrappers.hpp>

#include <logger/fwd.hpp>

#include <arpa/inet.h>
#include <cstring>

using lg = log::logger;

init::init(int argc, char* argv[]) {
    if(argc != 5) {
        lg::fatal() << "Expected 4 arguments: " << (argc > 0 ? argv[0] : "./transport")
                  << " [ip address] [port] [output file] [number of bits]\n";
        std::exit(EXIT_FAILURE);
    }

    if((sockfd = Socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        lg::fatal() << "error on socket call: " << strerror(errno) << "\n";
        std::exit(EXIT_FAILURE);
    }

    if((addr.sin_port = std::strtol(argv[2], nullptr, 10)) == 0) {
        lg::fatal() << "port arg is not a number\n";
        std::exit(EXIT_FAILURE);
    }

    if((no_bits = std::strtol(argv[4], nullptr, 10)) == 0) {
        lg::fatal() << "number of bits arg is not a number\n";
        std::exit(EXIT_FAILURE);
    }

    if(no_bits > 10'001'000) {
        lg::fatal() << "number of bits is too big\n";
        std::exit(EXIT_FAILURE);
    }

    if(!inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr)) {
        lg::fatal() << "given address is not IPv4 address.\n";
        std::exit(EXIT_FAILURE);
    }

    if(out_file = std::ofstream{argv[3]}; !out_file.is_open()) {
        lg::fatal() << "Failed to open output file.\n";
        std::exit(EXIT_FAILURE);
    }

    sockaddr_in bind_input {
        .sin_family = AF_INET,
        .sin_port = 0,
        .sin_addr = {.s_addr = INADDR_ANY},
        .sin_zero = {}
    };
    Bind(sockfd, (const sockaddr*)&bind_input, sizeof(sockaddr));

    addr.sin_family = AF_INET;
}
