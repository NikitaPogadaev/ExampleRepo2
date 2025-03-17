add_library(Func SHARED Fibfunction.cpp)
add_library(Class SHARED Fib.cpp)
target_link_libraries(Func PRIVATE spdlog::spdlog)
target_include_directories(Func PUBLIC ${CMAKE_SOURCE_DIR}/include)
