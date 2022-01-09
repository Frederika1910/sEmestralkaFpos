cmake_minimum_required(VERSION 3.18)
project(sEmestralkaF3 C)

set(CMAKE_C_STANDARD 99)

set(PROJECT_HEADERS
        server.h
        client.h)
set(PROJECT_SOURCES
        main.c
        server.c
        client.c)

add_executable(semestralkaPexeso ${PROJECT_SOURCES} ${PROJECT_HEADERS})
add_executable(server server.c)
add_executable(client client.c)

target_link_libraries(semestralkaPexeso pthread)
target_link_libraries(server pthread)
target_link_libraries(client pthread)