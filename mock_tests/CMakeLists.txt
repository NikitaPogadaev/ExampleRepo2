
find_package(GTest)
find_package(Threads)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(MockTest EXCLUDE_FROM_ALL MockTest.cpp)

target_link_libraries(MockTest PUBLIC ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} Threads::Threads gtest gmock gtest_main
Func Class
)

