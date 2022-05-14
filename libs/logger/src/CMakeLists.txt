add_library(logger)

target_include_directories(logger PUBLIC
        ${CMAKE_HOME_DIRECTORY}/include)

set(INCLUDE_FILES
        ${INCLUDE_DIR}/fwd.hpp
        ${INCLUDE_DIR}/entry.hpp
        ${INCLUDE_DIR}/logger.hpp
        ${INCLUDE_DIR}/logger_traits.hpp
)

target_sources(logger
    PUBLIC
        ${INCLUDE_FILES}
    PRIVATE
        logger.cpp
        entry.cpp
        logger_traits.cpp
)

install(TARGETS logger          DESTINATION lib)
install(FILES ${INCLUDE_FILES}  DESTINATION include/logger)
