add_subdirectory(transport-lib)

add_executable(transport
        main.cpp
        sys_wrappers.cpp)

target_include_directories(transport PUBLIC
        ${CMAKE_INCLUDE_CURRENT_DIR}
        )

target_link_libraries(transport
    PRIVATE
        logger
        transport-lib)
