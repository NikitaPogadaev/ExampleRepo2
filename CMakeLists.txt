
cmake_minimum_required(VERSION 3.14)

project(Fibonacci LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=undefined -fsanitize=address")
set(CMAKE_EXECUTABLE_SUFFIX ".out")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


# find_package(
#   Boost 1.74 REQUIRED
#   COMPONENTS atomic
#              )

include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(exe main.cpp)

add_subdirectory(src)
# add_subdirectory(tests)
# add_subdirectory(mock_tests)
target_link_libraries(exe Func Class)
