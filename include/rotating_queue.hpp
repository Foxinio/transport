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
        i=i%size;
        if(head+i >= size)
            return buffer + 1000*(head+i - size);
        return buffer + 1000*(head+i);
    }
    void rotate(int by) {
        by = by%size;
        if(head+by >= size)
            head += by - size;
        head += by;
    }
};
