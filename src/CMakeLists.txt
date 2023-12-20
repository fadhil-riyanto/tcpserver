cmake_minimum_required(VERSION 3.27.1)
add_compile_options(-Wall -Wextra -Wpedantic)


PROJECT(server)

SET(source
    server/server.c
    server/daemon.c
    server/handler/main_handler_recv.c
    helper/cmd_parser.c
    helper/epollfn.c
)


add_library(sourcelib STATIC "${source}")
add_executable(mainsource main.c)

target_link_libraries(mainsource sourcelib)

set_target_properties(mainsource PROPERTIES OUTPUT_NAME "server")
