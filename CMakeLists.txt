cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(cuda_introduction LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ROOT_CPP_DIR CPP/rt-in-one-weekend)

############################### C++
# Chapter 1

add_executable(rt_one CPP/rt-in-one-weekend/main.cpp)
target_include_directories(rt_one PUBLIC CPP/rt-in-one-weekend/include)

add_executable(cpp_add CPP/add.cpp)

# Chapter 2
include(FetchContent)
FetchContent_Declare(
googletest
URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(Vec3Test CPP/test/Vec3Test.cpp)
target_include_directories(Vec3Test PUBLIC CPP/rt-in-one-weekend/include)

target_link_libraries(Vec3Test GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(Vec3Test)

  # Chapter 4
  
  add_executable(blue_ray ${ROOT_CPP_DIR}/src/ray_first.cpp)
  target_include_directories(blue_ray PUBLIC CPP/rt-in-one-weekend/include)
  
  # Chapter 5
  
  add_executable(chapter_5 ${ROOT_CPP_DIR}/src/chapter5.cpp ${ROOT_CPP_DIR}/src/HitableList.cpp ${ROOT_CPP_DIR}/src/Sphere.cpp)
  target_include_directories(chapter_5 PUBLIC CPP/rt-in-one-weekend/include)



# CUDA

# Chapter 1

add_executable(gpu_1 GPU/src/chapter-1.cu)
target_include_directories(gpu_1 PUBLIC GPU/include)
set_property(TARGET gpu_1
             PROPERTY CUDA_SEPARABLE_COMPILATION ON)
set_property(TARGET gpu_1
             PROPERTY CUDA_ARCHITECTURES "61")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(gpu_1 PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-G>)
endif()

# Chapter 2

add_executable(gpu_2 GPU/src/chapter-2.cu)
target_include_directories(gpu_2 PUBLIC GPU/include)
set_property(TARGET gpu_2
             PROPERTY CUDA_SEPARABLE_COMPILATION ON)
set_property(TARGET gpu_2
             PROPERTY CUDA_ARCHITECTURES "61")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(gpu_2 PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-G>)
endif()

# Chapter 3 

add_executable(gpu_3 GPU/src/chapter-3.cu)
target_include_directories(gpu_3 PUBLIC GPU/include)
set_property(TARGET gpu_3
             PROPERTY CUDA_SEPARABLE_COMPILATION ON)
set_property(TARGET gpu_3
             PROPERTY CUDA_ARCHITECTURES "61")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(gpu_3 PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-G>)
endif()

# Chapter 4

add_executable(gpu_4 GPU/src/chapter-4.cu)
target_include_directories(gpu_4 PUBLIC GPU/include)
set_property(TARGET gpu_4
             PROPERTY CUDA_SEPARABLE_COMPILATION ON)
set_property(TARGET gpu_4
             PROPERTY CUDA_ARCHITECTURES "61")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(gpu_4 PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-G>)
endif()

# init

add_executable(simple_add GPU/add.cu)
set_property(TARGET simple_add
             PROPERTY CUDA_SEPARABLE_COMPILATION ON)
set_property(TARGET simple_add
             PROPERTY CUDA_ARCHITECTURES "61")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(simple_add PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-G>)
endif()

