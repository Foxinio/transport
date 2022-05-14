//
// Created by foxinio on 5/7/22.
//

#include <logger/entry.hpp>

namespace log {

entry::entry(std::ostream& output)
    : output(output), buffer() {}

entry::~entry() {
    output << buffer.str();
}

entry::operator std::ostream& () {
    return buffer;
}


} // logger
