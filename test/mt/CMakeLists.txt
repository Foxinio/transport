add_executable(moduleTests)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
        ${CMAKE_INCLUDE_CURRENT_DIR}
        )

target_sources(moduleTests
    PUBLIC
        ${CMAKE_HOME_DIRECTORY}/include/frame.hpp
        ${CMAKE_HOME_DIRECTORY}/include/init.hpp
        ${CMAKE_HOME_DIRECTORY}/include/sender.hpp
        ${CMAKE_HOME_DIRECTORY}/include/sys_wrappers.hpp
        ${CMAKE_HOME_DIRECTORY}/include/rotating_queue.hpp
        ${CMAKE_HOME_DIRECTORY}/include/utils.hpp
    PRIVATE
        ${CMAKE_HOME_DIRECTORY}/src/frame.cpp
        ${CMAKE_HOME_DIRECTORY}/src/init.cpp
        main.cpp
        ${CMAKE_HOME_DIRECTORY}/src/rotating_queue.cpp
        ${CMAKE_HOME_DIRECTORY}/src/sender.cpp
        sys_wrappers_fixture.cpp
        ${CMAKE_HOME_DIRECTORY}/src/utils.cpp)

target_include_directories(moduleTests PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(moduleTests
    PRIVATE
        logger)
#        GTest::gtest
#        GTest::gtest_main)
#        GTest::gmock
#        GTest::gmock_main)
