//
// Created by Szymon Jędras on 5/14/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <frame.hpp>

#include <logger/fwd.hpp>

#include <cstring>

using lg = log::logger;

frame::frame(int limit, int initial_size, std::ofstream&& out_file)
    : limit(limit), current_frame_number(0), buffer(), out_file(out_file), initial_size(initial_size) {}

frame::~frame() {
    if(final_buffer != nullptr)
        delete[] final_buffer;
}

std::vector<int> frame::get_to_request() {
    std::vector<int> res;
    received = 0;
    for(int i = current_frame_number; auto iter : received_map) {
        if(i == limit) {
            if(final_buffer == nullptr) {
                lg::info() << "asking to request final packet.\n";
                res.push_back(limit);
                received++;
            }
            break;
        }
        if(!iter) {
            lg::debug() << "asking to request packet #" << i << " packet.\n";
            res.push_back(i);
            received++;
        }

        i++;
    }
    return res;
}

void frame::record_received(char* buf, int frame_number, int size) {
    if(size != 1000) {
        received--;
        if(final_buffer == nullptr) {
            lg::info() << "received final packet of size " << size << "\n";
            final_buffer = new char[size];
            final_size = size;
            std::memcpy(final_buffer, buf, size);
        }
        else {
            lg::warning() << "received final packet again.\n";
        }
    }
    else if(frame_number == current_frame_number) {
        received--;
        lg::debug() << "received packet #" << frame_number << " which is first - flushing.\n";
        out_file.write(buf, size);
        print_rest();
    }
    else if(frame_number > current_frame_number) {
        received--;
        int i = current_frame_number - frame_number;
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
    return current_frame_number > limit && (final_buffer != nullptr || initial_size%1000 == 0);
}

void frame::shift(int by) {
    for(int i = 0; i+by < MAX_FRAME; i++) {
        received_map[i] = received_map[i + by];
    }
}

void frame::print_rest() {
    int i = 1;
    while(i < MAX_FRAME && received_map[i]) {
        out_file.write(buffer[i], 1000);
        received_map[i] = false;
        i++;
    }
    current_frame_number+=i;
    lg::debug() << "printed " << i << " packets. Finishing on " << current_frame_number << "\n";
    if(current_frame_number >= limit) {
        lg::debug() << "printed final packet\n";
        out_file.write(final_buffer, final_size);
    }
    shift(i);
    buffer.rotate(i);
}

bool frame::received_all() {
    if(received <= 0){
        lg::info() << "Received all returns true.\n";
        return true;
    }
    return false;
}
