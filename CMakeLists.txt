cmake_minimum_required(VERSION 3.20)
project(WORDLE)

set(CMAKE_CXX_STANDARD 14)

# Ensure source encoding is treated as UTF-8 on MSVC to handle Polish letters in literals
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include(FetchContent)

add_executable(WORDLE
        main.cpp)

# Ensure targets can find project headers when IDE indexes the project
target_include_directories(WORDLE PRIVATE ${CMAKE_SOURCE_DIR})

enable_testing()

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(wordle_functions OBJECT main.cpp)
target_compile_definitions(wordle_functions PRIVATE main=wordle_game_main)

add_executable(WORDLE_TEST
        tests.cpp
        $<TARGET_OBJECTS:wordle_functions>)

target_include_directories(WORDLE_TEST PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(WORDLE_TEST PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(WORDLE_TEST)
