cmake_minimum_required(VERSION 3.15)
project(Sssnake LANGUAGES CXX)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

option(BUILD_TESTS "Build all tests" ON)


add_subdirectory(spdlog)
add_subdirectory(Catch2)
add_subdirectory(src)

if(${BUILD_TESTS})
    enable_testing()
    add_subdirectory(test)
endif()