add_executable(${PROJECT_NAME}
main.cpp
helpers/Draw.hpp
helpers/Draw.cpp
helpers/ClockChrono.hpp
helpers/ClockChrono.cpp
helpers/ClockSDL.hpp
helpers/ClockSDL.cpp
helpers/Types.hpp
app/App.hpp
app/App.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 spdlog::spdlog_header_only)
set_target_properties(${PROJECT_NAME} PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED On
)