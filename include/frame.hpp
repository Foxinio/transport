//
// Created by Szymon Jędras on 5/14/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#pragma once

#include <rotating_queue.hpp>

#include <vector>
#include <fstream>

class frame {
public:
    static constexpr int MAX_FRAME = 500;

private:
    rotating_queue<MAX_FRAME> buffer;
    std::vector<bool> received_map;
    int current_frame_number;
    int limit;
    char* final_buffer = nullptr;
    int final_size = 0;
    int initial_size;
    int last_asked = -MAX_FRAME;

    std::ofstream& out_file;
public:

    frame(int limit, int initial_size, std::ofstream&& out_file);
    ~frame();

    std::vector<int> get_to_request();
    void record_received(char* buf, int frame_number, int size = 1000);
    bool is_done();
    bool received_all();

private:
    void shift(int by);
    void print_rest();
    void print_progress() const;
};

