#include <logger/logger.hpp>

#include <logger/logger_traits.hpp>
#include <logger/entry.hpp>

namespace log {

logger_traits logger::traits{};
std::ostream logger::dev_null{nullptr};

entry logger::fatal() {
    if (traits.get_current_level() >= level::fatal)
        return traits.get_output();
    return dev_null;
}

entry logger::error() {
    if (traits.get_current_level() >= level::error)
        return traits.get_output();
    return dev_null;
}

entry logger::warning() {
    if (traits.get_current_level() >= level::warning)
        return traits.get_output();
    return dev_null;
}

entry logger::debug() {
    if (traits.get_current_level() >= level::debug)
        return traits.get_output();
    return dev_null;
}

entry logger::info() {
    if (traits.get_current_level() >= level::info)
        return traits.get_output();
    return dev_null;
}

}
