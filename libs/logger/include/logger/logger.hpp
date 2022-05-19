#pragma once

#include <logger/logger_traits.hpp>
#include <logger/entry.hpp>

#include <ostream>

namespace log {

class logger {
public:
    static std::ostream dev_null;
    static logger_traits traits;

    static entry fatal();
    static entry error();
    static entry warning();
    static entry debug();
    static entry info();

};

}
