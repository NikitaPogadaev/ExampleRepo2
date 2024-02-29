cmake_minimum_required(VERSION 2.8)
project (Fibonacci)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++20")

# find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

find_package(
  Boost 1.74 REQUIRED
  COMPONENTS atomic
             container
             )

include_directories(include)
add_executable(exe main.cpp)

add_subdirectory(src)
# target_link_libraries(exe Func Class Boost::program_options)
target_link_libraries(exe Func Class
 Boost::headers
          Boost::atomic
          Boost::container
         )
