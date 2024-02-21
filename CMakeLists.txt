cmake_minimum_required(VERSION 2.8)
project (Fibonacci)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++20")

include_directories(include)
add_executable(exe main.cpp)

add_subdirectory(src)
target_link_libraries(exe Func Class)