cmake_minimum_required(VERSION 3.18)
project(sEmestralkaF3 C)

set(CMAKE_C_STANDARD 99)

add_executable(sEmestralkaE3 main.c)
add_executable(server server.c)
add_executable(client client.c)

target_link_libraries(server pthread)
target_link_libraries(client pthread)