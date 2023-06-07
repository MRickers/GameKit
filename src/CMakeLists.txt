add_library(gamelib
helpers/Draw.hpp
helpers/Draw.cpp
)

target_include_directories(gamelib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(gamelib PROPERTIES 
CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON)

target_link_libraries(gamelib PRIVATE SDL2)

add_executable(${PROJECT_NAME}
main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE gamelib)