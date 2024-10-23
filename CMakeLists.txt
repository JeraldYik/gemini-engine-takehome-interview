cmake_minimum_required(VERSION 3.18)

# Set the project name and version
project(gemini_interview)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable target
add_executable(match_engine main.cpp)

# Add the library target
add_library(Orderbook STATIC Orderbook.cpp)

# Set the target properties
target_include_directories(match_engine PUBLIC .)
target_link_libraries(match_engine Orderbook)

