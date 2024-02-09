cmake_minimum_required(VERSION 3.26)

project(damuffinc)

include_directories(include/ src/)

file(GLOB_RECURSE HEADERS include/*.h)
file(GLOB_RECURSE SOURCES src/*.c)

add_executable(damuffinc main.c ${HEADERS} ${SOURCES})