cmake_minimum_required(VERSION 3.15)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

project(GameKit
        LANGUAGES C CXX
        VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
        DESCRIPTION "Game development framework based on SDL2"
        )

find_package(PkgConfig REQUIRED) 

find_package(SDL2 REQUIRED)

if(UNIX)
# find_package does not find sdl2 ttf under ubuntu
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
else()
find_package(SDL2_ttf REQUIRED)

endif()

set(PROJECT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include)

option(BUILD_TESTS "Build all tests" ON)


add_subdirectory(spdlog)
add_subdirectory(Catch2)
add_subdirectory(src)

if(${BUILD_TESTS})
    enable_testing()
    add_subdirectory(test)
endif()