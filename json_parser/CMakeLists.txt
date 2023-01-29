set(TARGET_NAME jparser)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${TARGET_NAME}
    json_parser.c
)

target_include_directories(${TARGET_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/logger
)

target_link_libraries(${TARGET_NAME} PRIVATE
    logger
)
