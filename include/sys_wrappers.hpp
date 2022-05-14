//
// Created by Szymon Jędras on 5/14/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#pragma once

#include <netinet/in.h>
#include <sys/poll.h>

int Bind(int fd, __CONST_SOCKADDR_ARG addr, socklen_t len);
int Socket(int domain, int type, int protocol);

long Recvfrom(int fd, void *buf, size_t n, int flags, struct sockaddr *addr, socklen_t *addr_len);
long Sendto(int fd, const void *buf, size_t n, int flags, const struct sockaddr *addr, socklen_t addr_len);

int Poll(int sockfd, int timeout);
