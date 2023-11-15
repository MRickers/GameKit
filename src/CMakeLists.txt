add_library(libGkCore STATIC 
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

target_include_directories(libGkCore PUBLIC ${PROJECT_HEADERS})
target_link_libraries(libGkCore PRIVATE SDL2 SDL2_ttf)
set_target_properties(libGkCore PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED On
)

if(NOT NOT_SUBPROJECT)
add_custom_command(TARGET libGkCore POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       ${CMAKE_CURRENT_SOURCE_DIR}/assets/Roboto-Regular.ttf $<TARGET_FILE_DIR:libGkCore>)

endif()


add_executable(inputHandlerExample
examples/inputHandler.cpp
)

target_link_libraries(inputHandlerExample PRIVATE libGkCore)
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

target_link_libraries(stateMachineExample PRIVATE libGkCore spdlog::spdlog_header_only)
set_target_properties(stateMachineExample PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED On
)