add_executable(unitTests
        ../main.cpp
        )

target_sources(unitTests
    PRIVATE
        ut1.cpp)

target_include_directories(unitTests PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(unitTests
    PRIVATE
        GTest::gtest_main
        GTest::gtest)
#        GTest::gmock
#        GTest::gmock_main)
