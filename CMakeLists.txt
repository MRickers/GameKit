cmake_minimum_required(VERSION 3.15)
project(Snake LANGUAGES CXX)

find_package(SDL2 REQUIRED)

enable_testing()

add_subdirectory(spdlog)
add_subdirectory(Catch2)
add_subdirectory(test)
add_subdirectory(src)