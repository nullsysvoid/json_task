cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME logger)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${TARGET_NAME}
    logger.c
)

target_compile_definitions(${TARGET_NAME} PRIVATE
    DEBUG=1
)
