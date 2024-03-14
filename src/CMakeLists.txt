cmake_minimum_required(VERSION 3.27.1)
add_compile_options(-Wall -Wextra -Wpedantic)


PROJECT(server)

SET(source
    server/server.c
    server/daemon.c
    server/handler/main_handler_recv.c
    server/handler/http_err.c
    helper/cmd_parser.c
    helper/epollfn.c
    helper/test_function.c
    helper/http_parser.c
    helper/uriparser.c
    helper/utils.c
)


add_library(sourcelib STATIC "${source}")
add_executable(mainsource main.c)
add_compile_options(-Wall-pointer-types)

target_link_libraries(mainsource sourcelib)

set_target_properties(mainsource PROPERTIES OUTPUT_NAME "server")
