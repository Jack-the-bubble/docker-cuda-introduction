cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(cuda_introduction LANGUAGES CXX CUDA)

# C++

add_executable(cpp_add CPP/add.cpp)

# CUDA

add_executable(simple_add GPU/add.cu)
set_property(TARGET simple_add
             PROPERTY CUDA_SEPARABLE_COMPILATION ON)
set_property(TARGET simple_add
             PROPERTY CUDA_ARCHITECTURES "61")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(simple_add PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-G>)
endif()
    
