cmake_minimum_required(VERSION 3.16)
project(image_correction C)

set(CMAKE_C_STANDARD 11)

add_executable(image_correction main.c pgm.c pgm.h)