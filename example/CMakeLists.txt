cmake_minimum_required(VERSION 3.16)
project(week01_monitor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(monitor src/main.cpp src/config.cpp)
target_include_directories(monitor PRIVATE include)

add_executable(debug_demo src/debug_demo.cpp)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(monitor PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(debug_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
