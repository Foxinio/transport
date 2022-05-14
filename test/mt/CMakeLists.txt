add_executable(moduleTests
        ../main.cpp
        )

target_sources(moduleTests
    PRIVATE)

target_include_directories(moduleTests PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(moduleTests
    PRIVATE
        GTest::gtest
        GTest::gtest_main)
#        GTest::gmock
#        GTest::gmock_main)
