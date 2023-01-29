set(TARGET_NAME hashtable)

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

add_library(${TARGET_NAME}
    hashtable.c
)

target_compile_definitions(${TARGET_NAME} PRIVATE
    INITTAG=1
    TABLE_INIT_SZ=32
)

target_include_directories(${TARGET_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/logger
    PUBLIC
        ${APR_INCLUDE_DIR}
)

target_link_libraries(${TARGET_NAME} PRIVATE
    logger
    ${APR_LIBRARY}
)
