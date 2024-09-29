cmake_minimum_required(VERSION 3.10)
project(GTPPCP)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the subdirectories for the project and tests
add_subdirectory(GenericThreadPoolProducerConsumerPattern)
add_subdirectory(Test)