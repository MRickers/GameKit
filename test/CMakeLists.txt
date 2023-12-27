
add_executable(TestCatch TestCatch.cpp)
target_link_libraries(TestCatch Catch2::Catch2WithMain)
add_test(NAME TestCatchTest COMMAND TestCatch)

add_executable(TestSpdLog TestSpdLog.cpp)
target_link_libraries(TestSpdLog Catch2::Catch2WithMain spdlog::spdlog_header_only)
add_test(NAME TestSpdLogTest COMMAND TestSpdLog)

add_executable(TestGameException TestGameException.cpp)
target_include_directories(TestGameException PRIVATE ${PROJECT_HEADERS})
target_link_libraries(TestGameException Catch2::Catch2WithMain)
set_target_properties(TestGameException PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON
)
add_test(NAME TestGameExceptionTest COMMAND TestGameException)

add_executable(TestTimer 
TestTimer.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/helpers/ClockChrono.cpp
)
target_include_directories(TestTimer PRIVATE ${PROJECT_HEADERS})
target_link_libraries(TestTimer Catch2::Catch2WithMain)
add_test(NAME TestTimerTest COMMAND TestTimer)

add_executable(TestVector2d
TestVector2d.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/vector/vector2d.cpp
)
target_include_directories(TestVector2d PRIVATE ${PROJECT_HEADERS})
target_link_libraries(TestVector2d Catch2::Catch2WithMain)
set_target_properties(TestVector2d PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON
)
add_test(NAME TestVector2dTest COMMAND TestVector2d)

add_executable(TestInputHandler
TestInputHandler.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/core/InputHandler.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/vector/vector2d.cpp
)
target_include_directories(TestInputHandler PRIVATE ${PROJECT_HEADERS})
target_link_libraries(TestInputHandler Catch2::Catch2WithMain SDL2)
set_target_properties(TestInputHandler PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON
)
add_test(NAME TestInputHandlerTest COMMAND TestInputHandler)


add_executable(TestStateMachine
TestStateMachine.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/core/state_machine.cpp
)
target_include_directories(TestStateMachine PRIVATE ${PROJECT_HEADERS})
target_link_libraries(TestStateMachine Catch2::Catch2WithMain spdlog::spdlog_header_only)
set_target_properties(TestStateMachine PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON
)
add_test(NAME TestStateMachine COMMAND TestStateMachine)

add_executable(TestTextBox
TestTextBox.cpp
)
target_link_libraries(TestTextBox Catch2::Catch2WithMain gkcore)
set_target_properties(TestTextBox PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON
)
add_test(NAME TestTextBox COMMAND TestTextBox)

add_executable(TestTextureManager
TestTextureManager.cpp
)
target_link_libraries(TestTextureManager Catch2::Catch2WithMain gkcore)
set_target_properties(TestTextureManager PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON
)
add_test(NAME TestTextureManager COMMAND TestTextureManager)

add_custom_command(TARGET TestTextureManager POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_CURRENT_SOURCE_DIR}/assets/test_texture.png $<TARGET_FILE_DIR:TestTextureManager>)

add_custom_command(TARGET TestTextureManager POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_CURRENT_SOURCE_DIR}/assets/texture.cfg $<TARGET_FILE_DIR:TestTextureManager>)