cmake_minimum_required(VERSION 3.15)
project(Snake LANGUAGES CXX)

find_package(SDL2 REQUIRED)

option(BUILD_TESTS "Build all tests" ON)


add_subdirectory(spdlog)
add_subdirectory(Catch2)
add_subdirectory(src)

if(${BUILD_TESTS})
    enable_testing()
    add_subdirectory(test)
endif()