//
// Created by foxinio on 5/7/22.
//

#include <logger/logger_traits.hpp>

#include <filesystem>
#include <iostream>

namespace log {
logger_traits::logger_traits()
    : current_level(set_level_from_env())
    , output(std::nullopt) {
    set_output_from_env();
}

level logger_traits::set_level_from_env() {
    std::string logger_level = getenv("LOGGER_LEVEL");
    if(logger_level.empty())
        return DEFAULT_LEVEL;
    std::transform(logger_level.begin(), logger_level.end(), logger_level.begin(), [](char c){  return std::toupper(c); });
    if(long leveli = std::strtol(logger_level.c_str(), nullptr, 10); leveli != 0) {
        switch(leveli) {
            case 1:
                return level::debug;
            case 2:
                return level::info;
            case 3:
                return level::warning;
            case 4:
                return level::fatal;
            case 5:
                return level::fatal;
            default:
                return DEFAULT_LEVEL;
        }
    }
    else if(logger_level == "DEBUG")
        return level::debug;
    else if(logger_level == "INFO")
        return level::info;
    else if(logger_level == "WARNING")
        return level::warning;
    else if(logger_level == "ERROR")
        return level::fatal;
    else if(logger_level == "FATAL")
        return level::fatal;
    else if(logger_level == "NONE")
        return level::none;
    return DEFAULT_LEVEL;
}

void logger_traits::set_output_from_env() {
    output_file = getenv("LOGGER_PATH");
    change_output_file(output_file);
}

logger_traits::logger_traits(level current_level)
    : current_level(current_level)
    , output(std::nullopt) {}

logger_traits::logger_traits(level current_level, std::ofstream&& output)
    : current_level(current_level)
    , output({std::move(output)}) {}

logger_traits::logger_traits(level current_level, std::string_view output_file_path)
    : logger_traits(current_level, std::ofstream((std::string)output_file_path)) {}

void logger_traits::set_level(level lvl) {
    current_level = lvl;
}

void logger_traits::change_output_file(std::string_view file_path) {
    using namespace std::filesystem;

    if(output) {
        output->close();
    }
    output_file = file_path;

    if(output_file.empty()) {
        output = std::nullopt;
        return;
    }
    if(output_file.find_last_of('/') == std::string_view::npos ||
        !exists(path(output_file.substr(0, output_file.find_last_of('/'))))) {
        current_level = level::none;
        output = std::nullopt;
        return;
    }
    std::optional<std::ofstream> res{(std::string)output_file};
    if(!res->is_open()) {
        current_level = level::none;
        output = std::nullopt;
        return;
    }
    output = std::move(res);
}

void logger_traits::refresh_output() {
    change_output_file(output_file);
}

std::ostream &logger_traits::get_output() {
    if(output)
        return *output;
    return std::cerr;
}

level logger_traits::get_current_level() {
    return current_level;
}

std::string logger_traits::getenv(const char * var) {
    char* res = std::getenv(var);
    if(res == nullptr)
        return "";
    return res;
}
} // logger
