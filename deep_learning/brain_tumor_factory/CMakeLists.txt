cmake_minimum_required(VERSION 3.13)
project(brain_tumor_factory C)

set(CMAKE_C_STANDARD 99)

add_executable(brain_tumor_factory brain_tumor_factory.c image_utils.c image_utils.h)

target_link_libraries(brain_tumor_factory m)
