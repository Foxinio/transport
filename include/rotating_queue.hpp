//
// Created by foxinio on 5/14/22.
//
#pragma once

template<int size>
class rotating_queue {
    char buffer[size*1000];
    int head;

public:
    char* operator[](int i) {
        return buffer + 1000*((head+i)%size);
    }
    void rotate(int by) {
        head = (head + by) % size;
//        if(head > size)
//            throw std::out_of_range("rotating buffer out of range, head:" + std::to_string(head) +
//                                    ", increased by " + std::to_string(by));
    }
};
