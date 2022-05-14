#pragma once

#include <logger/logger_traits.hpp>
#include <logger/entry.hpp>

#include <ostream>

namespace log {

class logger {
    static std::ostream dev_null;
public:
    static logger_traits traits;

    static entry fatal();
    static entry error();
    static entry warning();
    static entry debug();
    static entry info();

};

}
