cmake_minimum_required(VERSION 3.29)
project(lista_duplamente_encadeada)

set(CMAKE_CXX_STANDARD 20)

add_executable(lista_duplamente_encadeada main.cpp)

configure_file(${CMAKE_SOURCE_DIR}/livros.txt ${CMAKE_BINARY_DIR}/livros.txt COPYONLY)