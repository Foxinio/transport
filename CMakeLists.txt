cmake_minimum_required(VERSION 3.15)
project(pracownia-3
        VERSION 0.1
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ${CMAKE_HOME_DIRECTORY}/include)

add_subdirectory(src)
#add_subdirectory(test)

add_subdirectory(libs/logger)

include_directories(${CMAKE_SOURCE_DIR}/include)
