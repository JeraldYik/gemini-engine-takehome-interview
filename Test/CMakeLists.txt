cmake_minimum_required(VERSION 3.18)

# Set the project name
project(gemini_interview)

enable_testing()

# Set the C++ standard to use
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the project library
add_library(orderbook STATIC Orderbook.cpp)
add_library(input_handler STATIC InputHandler.cpp)

# Add the test executable
add_executable(orderbook_test OrderbookTest.cpp)

# Link the test executable with the project library and the Google Test library
find_package(GTest REQUIRED)
target_include_directories(orderbook_test PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(orderbook_test PUBLIC GTest::gtest GTest::gtest_main orderbook input_handler)

include(GoogleTest)
gtest_discover_tests(orderbook_test)


