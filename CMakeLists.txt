cmake_minimum_required(VERSION 3.15)
project(Snake LANGUAGES CXX)

find_package(SDL2 REQUIRED)

add_subdirectory(src)