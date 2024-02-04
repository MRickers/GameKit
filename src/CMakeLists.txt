add_library(gkcore STATIC 
helpers/drawer.cpp
helpers/clock_chrono.cpp
helpers/clock_sdl.cpp
core/input_handler.cpp
core/state_input_handler.cpp
core/state_machine.cpp
core/ui/text_box.cpp
core/sprite_sheet.cpp
core/base_animation.cpp
core/animation_directional.cpp
vector/vector2d.cpp
App.cpp
        core/Map.cpp
        core/Camera.cpp
        ../include/GameKit/core/Camera.hpp
)

target_include_directories(gkcore PUBLIC ${PROJECT_HEADERS})
target_link_libraries(gkcore PRIVATE SDL2 SDL2_ttf SDL2_image fmt::fmt-header-only)
set_target_properties(gkcore PROPERTIES
CXX_STANDARD 23
CXX_STANDARD_REQUIRED On
)

if(NOT NOT_SUBPROJECT)
add_custom_command(TARGET gkcore POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       ${CMAKE_CURRENT_SOURCE_DIR}/assets/Roboto-Regular.ttf $<TARGET_FILE_DIR:gkcore>)

endif()


add_executable(inputHandlerExample
examples/inputHandler.cpp
)

target_link_libraries(inputHandlerExample PRIVATE gkcore)
set_target_properties(inputHandlerExample PROPERTIES
CXX_STANDARD 23
CXX_STANDARD_REQUIRED On
)

add_executable(state_machine_example
examples/state_machine.cpp
)

add_custom_command(TARGET state_machine_example POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       ${CMAKE_CURRENT_SOURCE_DIR}/assets/Roboto-Regular.ttf $<TARGET_FILE_DIR:state_machine_example>)

target_link_libraries(state_machine_example PRIVATE gkcore spdlog::spdlog_header_only)
set_target_properties(state_machine_example PROPERTIES
CXX_STANDARD 23
CXX_STANDARD_REQUIRED On
)

set(SPRITE_SHEET_EXAMPLE sprite_sheet_example)

add_executable(${SPRITE_SHEET_EXAMPLE} examples/sprite_sheet_example.cpp)
target_link_libraries(${SPRITE_SHEET_EXAMPLE} PRIVATE gkcore spdlog::spdlog_header_only)
set_target_properties(${SPRITE_SHEET_EXAMPLE} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED On
)
add_custom_command(TARGET ${SPRITE_SHEET_EXAMPLE} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprite.sheet $<TARGET_FILE_DIR:${SPRITE_SHEET_EXAMPLE}>
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/gobby.png $<TARGET_FILE_DIR:${SPRITE_SHEET_EXAMPLE}>
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprite_texture.cfg $<TARGET_FILE_DIR:${SPRITE_SHEET_EXAMPLE}>
)