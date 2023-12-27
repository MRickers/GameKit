add_library(gkcore STATIC 
helpers/drawer.cpp
helpers/clock_chrono.cpp
helpers/clock_sdl.cpp
core/input_handler.cpp
core/state_input_handler.cpp
core/state_machine.cpp
core/ui/text_box.cpp
vector/vector2d.cpp
App.cpp
)

target_include_directories(gkcore PUBLIC ${PROJECT_HEADERS})
target_link_libraries(gkcore PRIVATE SDL2 SDL2_ttf SDL2_image)
set_target_properties(gkcore PROPERTIES
CXX_STANDARD 17
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
CXX_STANDARD 17
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
CXX_STANDARD 17
CXX_STANDARD_REQUIRED On
)