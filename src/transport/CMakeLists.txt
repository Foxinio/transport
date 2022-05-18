add_library(transport-lib OBJECT)

target_include_directories(transport-lib PUBLIC
        ${CMAKE_INCLUDE_CURRENT_DIR}
        )

target_link_libraries(transport-lib
    PRIVATE
        logger)

target_sources(transport-lib
    PUBLIC
        ${CMAKE_INCLUDE_CURRENT_DIR}/frame.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/init.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/sender.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/sys_wrappers.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/rotating_queue.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/utils.hpp
    PRIVATE
        frame.cpp
        init.cpp
        rotating_queue.cpp
        sender.cpp
        utils.cpp
        )
