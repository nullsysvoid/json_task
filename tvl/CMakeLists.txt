set(TARGET_NAME tvl)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${TARGET_NAME}
    tvl.c
)

target_include_directories(${TARGET_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/logger
        ${CMAKE_SOURCE_DIR}/hashtable
        ${CMAKE_SOURCE_DIR}/json_parser
        ${APR_INCLUDE_DIR}
)

target_link_libraries(${TARGET_NAME} PRIVATE
    logger
    jparser
)
