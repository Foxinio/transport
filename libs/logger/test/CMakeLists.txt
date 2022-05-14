find_package(GTest 1.10.0 REQUIRED)

add_executable(loggerTests
        test.cpp)

target_include_directories(loggerTests PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(loggerTests
    PRIVATE
        logger
        GTest::gtest_main
        GTest::gtest)
