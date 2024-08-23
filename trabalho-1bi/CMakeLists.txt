cmake_minimum_required(VERSION 3.29)
project(trabalho_1bi C)

set(CMAKE_C_STANDARD 11)

add_executable(trabalho_1bi main.c)

configure_file(${CMAKE_SOURCE_DIR}/code.txt ${CMAKE_BINARY_DIR}/code.txt COPYONLY)