cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME test)

project(test LANGUAGES C)

add_executable(${TARGET_NAME}
    main.c
)

target_link_libraries(${TARGET_NAME} PRIVATE
    jparser
    ${APR_LIBRARY}
    hashtable
    tvl
)

target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/hashtable
    ${CMAKE_SOURCE_DIR}/logger
    ${CMAKE_SOURCE_DIR}/json_parser
    ${CMAKE_SOURCE_DIR}/tvl
)

target_compile_definitions(${TARGET_NAME} PUBLIC
    DEBUG=1
)
