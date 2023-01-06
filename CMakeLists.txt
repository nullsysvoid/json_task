cmake_minimum_required(VERSION 3.19)

project(task_json LANGUAGES C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_path(APR_INCLUDE_DIR apr.h
    /usr/local/include/apr-1
    /usr/local/include/apr-1.0
    /usr/include/apr-1
    /usr/include/apr-1.0
    /usr/local/apr/include/apr-1
)

set(APR_NAMES ${APR_NAMES} apr-1)
find_library(APR_LIBRARY
    NAMES ${APR_NAMES}
    HINTS
    PATHS
    /usr/lib
    /usr/local/lib
    /usr/local/apr/lib
    /usr/lib/x86_64-linux-gnu
)

if(NOT APR_LIBRARY AND NOT APR_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find APR library")
endif()

include_directories("${APR_INCLUDE_DIR}")

add_executable(task_json
    main.c
    json_parser.c
    hashtable.c
    tvl.c
)

target_link_libraries(task_json PRIVATE ${APR_LIBRARY})

add_subdirectory(test build)
