cmake_minimum_required(VERSION 3.19)

project(test LANGUAGES C)

add_executable(test
    main.c
    ../json_parser.c
    ../hashtable.c
    ../tvl.c
)

target_link_libraries(test PRIVATE ${APR_LIBRARY})
