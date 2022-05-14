//
// Created by foxinio on 5/7/22.
//
#pragma once

#include <fstream>
#include <optional>

namespace log {

enum class level : int {
    none,
    fatal,
    error,
    warning,
    info,
    debug,
};

class logger_traits {
    std::optional<std::ofstream> output;
    level current_level;
    std::string_view output_file;

    static const level DEFAULT_LEVEL = level::none;

    logger_traits();

    friend class logger;
public:
    logger_traits(level current_level);
    logger_traits(level current_level, std::ofstream&& output);
    logger_traits(level current_level, std::string_view output_file_path);

    void set_level(level);
    void change_output_file(std::string_view path);
    void refresh_output();

    std::ostream& get_output();
    level get_current_level();

private:
    level set_level_from_env();
    void set_output_from_env();
    std::string getenv(const char * var);
};

} // logger
