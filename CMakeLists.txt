cmake_minimum_required(VERSION 3.20)
project(Coroutines)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE HEADERS *.hpp)

include_directories(.)

add_executable(Coroutines main.cpp ${HEADERS})

target_compile_options(Coroutines PUBLIC "-fcoroutines-ts")