//
// Created by Szymon Jędras on 5/14/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <frame.hpp>

#include <logger/fwd.hpp>

#include <cstring>
#include <iostream>
#include <iterator>
#include <iomanip>

using lg = log::logger;

frame::frame(int limit, int initial_size, std::ofstream&& out_file)
    : limit(limit)
    , received_map(MAX_FRAME, false)
    , current_frame_number(0)
    , buffer()
    , out_file(out_file)
    , initial_size(initial_size) {}

frame::~frame() {
    if(final_buffer != nullptr)
        delete[] final_buffer;
}

std::vector<int> frame::get_to_request() {
    std::vector<int> res;
    last_asked = current_frame_number;
    int i = current_frame_number;
    for(auto iter = received_map.begin(); iter < received_map.end(); iter++) {
        if(i == limit) {
            if(final_buffer == nullptr) {
                lg::info() << "asking to request final packet.\n";
                res.push_back(limit);
            }
            return res;
        }
        int diff = std::distance(received_map.begin(), iter);
        log::logger::dev_null << "iter diff:" << diff << "\n";
        if(!(*iter).operator bool()) {
            lg::debug() << "asking to request packet #" << i << " packet.\n";
            res.push_back(i);
        }

        i++;
    }
    while(res.size() < MAX_FRAME && i < limit) {
        res.push_back(i);
        i++;
    }
    return res;
}

void frame::record_received(char* buf, int frame_number, int size) {
    if(size != 1000) {
        if(final_buffer == nullptr) {
            lg::info() << "received final packet of size " << size << "\n";
            final_buffer = new char[size];
            final_size = size;
            std::memcpy(final_buffer, buf, size);
            if(current_frame_number == limit) {
                out_file.write(final_buffer, size);
                print_progress();
                current_frame_number++;
            }
        }
        else {
            lg::warning() << "received final packet again.\n";
        }
    }
    else if(frame_number > current_frame_number + MAX_FRAME) {
        lg::debug() << "received packet #" << frame_number
                    << ", while current_frame_number is " << current_frame_number
                    << ". Packet from the future.\n";
    }
    else if(frame_number == current_frame_number) {
        lg::debug() << "received packet #" << frame_number << " which is first - flushing.\n";
        print_progress();
        current_frame_number++;
        out_file.write(buf, size);
        received_map[0] = false;
        print_rest();
    }
    else if(frame_number > current_frame_number) {
        int i = frame_number - current_frame_number;
        if(!received_map[i]) {
            lg::debug() << "received packet #" << frame_number << " which isn't first - buffering.\n";
            received_map[i] = true;
            std::memcpy(buffer[i], buf, size);
        }
        else {
            lg::debug() << "received packet #" << frame_number << " which is a duplicate.\n";
        }
    }
    else {
        lg::debug() << "received packet #" << frame_number << " which is smaller than current frame number (="
                    << current_frame_number << "), possibly a duplicate.\n";
    }
}

bool frame::is_done() {
    bool first = current_frame_number > limit;
    bool second = (final_buffer != nullptr || initial_size%1000 == 0);
    return first && second;
}

void frame::shift(int by) {
    for(int i = 0; i < MAX_FRAME; i++) {
        if(i+by<MAX_FRAME)
            received_map[i] = received_map[i + by];
        else
            received_map[i] = false;
    }
}

void frame::print_rest() {
    int i = 1;
    while(i < MAX_FRAME && received_map[i]) {
        out_file.write(buffer[i], 1000);
        received_map[i] = false;
        i++;
        print_progress();
        current_frame_number++;
    }
    lg::debug() << "printed " << i << " packets. Finishing on " << current_frame_number << "\n";
    if(current_frame_number >= limit && final_buffer != nullptr) {
        lg::debug() << "printed final packet\n";
        out_file.write(final_buffer, final_size);
        print_progress();
        current_frame_number++;
    }
    shift(i);
    buffer.rotate(i);
}

void frame::print_progress() const {
    std::cout << std::fixed << std::setprecision(3) << (float) current_frame_number /
        (limit + (initial_size % 1000 != 0))*100 << "% done\n";
}

bool frame::received_all() {
    if(last_asked+MAX_FRAME <= current_frame_number){
        lg::info() << "Received all returns true.\n";
        return true;
    }
    return false;
}
