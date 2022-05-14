//
// Created by foxinio on 5/7/22.
//
#pragma once

#include <sstream>

namespace log {

class entry {
    std::stringstream buffer;
    std::ostream& output;
public:
    entry(const entry&) = delete;
    entry operator=(const entry&) = delete;
    entry(entry&&) = delete;
    entry operator=(entry&&) = delete;

    entry(std::ostream& output);
    virtual ~entry();
    operator std::ostream& ();
};

template<typename T>
std::ostream& operator<<(entry e, const T& t) {
    (std::ostream&)e << t;
    return e;
}
} // logger

