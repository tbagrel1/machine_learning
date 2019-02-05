cmake_minimum_required(VERSION 3.13)
project(brain_tumor_demo C)

set(CMAKE_C_STANDARD 99)

add_executable(brain_tumor_demo brain_image_factory.c image_utils.c image_utils.h)
