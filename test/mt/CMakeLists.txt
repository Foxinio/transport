add_executable(moduleTests
        main.cpp
        sys_wrappers_fixture.cpp)

target_include_directories(moduleTests PUBLIC
        ${CMAKE_INCLUDE_CURRENT_DIR}
        )

target_include_directories(moduleTests PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(moduleTests
    PRIVATE
        logger
        transport-lib)
#        GTest::gtest
#        GTest::gtest_main)
#        GTest::gmock
#        GTest::gmock_main)
