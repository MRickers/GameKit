add_executable(${PROJECT_NAME}
main.cpp
helpers/Draw.hpp
helpers/Draw.cpp
app/App.hpp
app/App.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)
set_target_properties(${PROJECT_NAME} PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED On
)