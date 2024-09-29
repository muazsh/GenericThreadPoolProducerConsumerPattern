cmake_minimum_required(VERSION 3.10) 
project(GenericThreadPoolProducerConsumerPattern)                    

set(CMAKE_CXX_STANDARD 14)           
set(CMAKE_CXX_STANDARD_REQUIRED ON)  

# Add the directory containing your source files
add_library(GTPPCPLib ITask.h Producer.h Consumer.h Worker.h ThreadPool.h ThreadPool.cpp Worker.cpp)
target_include_directories(GTPPCPLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})