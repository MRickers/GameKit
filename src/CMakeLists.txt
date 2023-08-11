add_library(libGkCore STATIC 
helpers/Draw.hpp
helpers/Draw.cpp
helpers/ClockChrono.hpp
helpers/ClockChrono.cpp
helpers/ClockSDL.hpp
helpers/ClockSDL.cpp
helpers/Types.hpp
helpers/GameException.hpp
core/GameObject.hpp
core/InputHandler.hpp
core/InputHandler.cpp
core/StateInputHandler.hpp
core/StateInputHandler.cpp
core/StateMachine.hpp
core/StateMachine.cpp
vector/Vector2d.hpp
vector/Vector2d.cpp
app/App.hpp
app/App.cpp
)
target_include_directories(libGkCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libGkCore PRIVATE SDL2 spdlog::spdlog_header_only)
set_target_properties(libGkCore PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED On
)

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

target_link_libraries(stateMachineExample PRIVATE libGkCore)
set_target_properties(stateMachineExample PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED On
)