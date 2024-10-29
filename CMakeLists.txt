cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(cuda_introduction LANGUAGES CXX CUDA)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

############################### C++
# Vec3
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


add_executable(rt_one CPP/rt-in-one-weekend/main.cpp)
target_include_directories(rt_one PUBLIC CPP/rt-in-one-weekend/include)

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
    
