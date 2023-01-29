cmake_minimum_required(VERSION 3.16)

project(task_json LANGUAGES C)

set(TARGET_NAME task_json)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(logger)
add_subdirectory(hashtable)
add_subdirectory(json_parser)
add_subdirectory(tvl)

add_executable(${TARGET_NAME}
    main.c
)

target_include_directories(${TARGET_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/logger
        ${CMAKE_SOURCE_DIR}/hashtable
        ${CMAKE_SOURCE_DIR}/json_parser
        ${CMAKE_SOURCE_DIR}/tvl
        ${APR_INCLUDE_DIR}
)

target_link_libraries(${TARGET_NAME} PRIVATE
    jparser
    ${APR_LIBRARY}
    hashtable
    tvl
)

add_subdirectory(test build)
