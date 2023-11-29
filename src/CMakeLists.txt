add_library(gkcore STATIC 
helpers/Draw.cpp
helpers/ClockChrono.cpp
helpers/ClockSDL.cpp
core/InputHandler.cpp
core/StateInputHandler.cpp
core/StateMachine.cpp
core/ui/TextBox.cpp
vector/Vector2d.cpp
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

add_executable(stateMachineExample
examples/stateMachine.cpp
)

add_custom_command(TARGET stateMachineExample POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       ${CMAKE_CURRENT_SOURCE_DIR}/assets/Roboto-Regular.ttf $<TARGET_FILE_DIR:stateMachineExample>)

target_link_libraries(stateMachineExample PRIVATE gkcore spdlog::spdlog_header_only)
set_target_properties(stateMachineExample PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED On
)