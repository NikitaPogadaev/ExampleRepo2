find_package(GTest)

add_executable(Tests EXCLUDE_FROM_ALL MainTests.cpp FibTests.cpp)
target_link_libraries(Tests PUBLIC ${GTEST_LIBRARIES} Func Class)
