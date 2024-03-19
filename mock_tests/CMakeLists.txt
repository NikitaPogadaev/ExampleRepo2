
find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(MockTest EXCLUDE_FROM_ALL MockTest.cpp)

target_link_libraries(MockTest PUBLIC gtest gmock gtest_main Func Class)

