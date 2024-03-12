cmake_minimum_required(VERSION 3.13)
project (Fibonacci)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++20 -lgtest -lgmock -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=undefined -fsanitize=address")
set(CMAKE_EXECUTABLE_SUFFIX ".out")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


find_package(
  Boost 1.74 REQUIRED
  COMPONENTS atomic
             )

include_directories(include)
add_executable(exe main.cpp)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(mock_tests)
target_link_libraries(exe Func Class
 Boost::headers
          Boost::atomic

         )
