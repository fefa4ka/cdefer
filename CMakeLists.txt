cmake_minimum_required(VERSION 3.1)

project(lr VERSION 1.0
    DESCRIPTION "C Header Library for Defer"
    LANGUAGES C)

add_executable(example example.c)
target_include_directories(example PUBLIC .)

