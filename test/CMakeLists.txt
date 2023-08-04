
add_executable(TestCatch TestCatch.cpp)
target_link_libraries(TestCatch Catch2::Catch2WithMain)
add_test(NAME TestCatchTest COMMAND TestCatch)

add_executable(TestSpdLog TestSpdLog.cpp)
target_link_libraries(TestSpdLog Catch2::Catch2WithMain spdlog::spdlog_header_only)
add_test(NAME TestSpdLogTest COMMAND TestSpdLog)

add_executable(TestGameException TestGameException.cpp)
target_include_directories(TestGameException PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(TestGameException Catch2::Catch2WithMain)
set_target_properties(TestGameException PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON
)
add_test(NAME TestGameExceptionTest COMMAND TestGameException)

add_executable(TestTimer 
TestTimer.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/helpers/ClockChrono.hpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/helpers/ClockChrono.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/helpers/Timer.hpp
)
target_include_directories(TestTimer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(TestTimer Catch2::Catch2WithMain)
add_test(NAME TestTimerTest COMMAND TestTimer)

add_executable(TestSnake 
TestSnake.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/app/snake/Snake.hpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/app/snake/Snake.cpp
)
target_include_directories(TestSnake PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(TestSnake Catch2::Catch2WithMain SDL2)
add_test(NAME TestSnakeTest COMMAND TestSnake)

add_executable(TestVector2d
TestVector2d.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/vector/Vector2d.hpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/vector/Vector2d.cpp
)
target_include_directories(TestVector2d PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(TestVector2d Catch2::Catch2WithMain)
set_target_properties(TestVector2d PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON
)
add_test(NAME TestVector2dTest COMMAND TestVector2d)

add_executable(TestInputHandler
TestInputHandler.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/core/InputHandler.hpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/core/InputHandler.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/vector/Vector2d.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/vector/Vector2d.hpp
)
target_include_directories(TestInputHandler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(TestInputHandler Catch2::Catch2WithMain SDL2)
set_target_properties(TestInputHandler PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON
)
add_test(NAME TestInputHandlerTest COMMAND TestVector2d)


add_executable(TestStateManager
TestStateManager.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/core/StateManager.hpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/core/StateManager.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/core/InputHandler.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/core/InputHandler.hpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/vector/Vector2d.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/vector/Vector2d.hpp
)
target_include_directories(TestStateManager PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(TestStateManager Catch2::Catch2WithMain SDL2)
set_target_properties(TestStateManager PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON
)
add_test(NAME TestStateManager COMMAND TestVector2d)