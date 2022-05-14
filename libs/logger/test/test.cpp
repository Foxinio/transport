//
// Created by foxinio on 5/14/22.
//

#include <logger/logger.hpp>

#include <gtest/gtest.h>

using lg = log::logger;

TEST(simple_test, test1) {
    lg::traits.set_level(log::level::debug);

    lg::fatal() << "fatal log\n";
    lg::error() << "error log\n";
    lg::warning() << "warning log\n";
    lg::info() << "info log\n";
    lg::debug() << "debug log\n";
}

int main() {
    testing::InitGoogleTest();
    return ::RUN_ALL_TESTS();
}
