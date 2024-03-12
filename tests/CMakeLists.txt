find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(Tests EXCLUDE_FROM_ALL MainTests.cpp FibTests.cpp)
target_link_libraries(Tests PUBLIC ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} Func Class)

