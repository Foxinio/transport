cmake_minimum_required(VERSION 3.15)
project(logger)

set(CMAKE_CXX_STANDARD 20)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/logger)
#add_subdirectory(test)
add_subdirectory(src)
