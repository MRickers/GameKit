
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