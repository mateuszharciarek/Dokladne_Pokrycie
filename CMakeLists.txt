cmake_minimum_required(VERSION 3.6)
project(Dokladne_Pokrycie)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c)
add_executable(Dokladne_Pokrycie ${SOURCE_FILES})