add_executable(${CMAKE_PROJECT_NAME})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
        ${CMAKE_INCLUDE_CURRENT_DIR}
        )

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        loggerlib)

target_sources(${CMAKE_PROJECT_NAME}
    PUBLIC
        ${CMAKE_INCLUDE_CURRENT_DIR}/frame.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/init.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/sender.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/sys_wrappers.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/rotating_queue.hpp
    PRIVATE
        frame.cpp
        init.cpp
        main.cpp
        rotating_queue.cpp
        sender.cpp
        sys_wrappers.cpp
        )
