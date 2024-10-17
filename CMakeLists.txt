cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(cuda_introduction LANGUAGES CXX CUDA)

# CUDA

add_executable(simple_add GPU/add.cu)
set_property(TARGET simple_add
             PROPERTY CUDA_SEPARABLE_COMPILATION ON)
