cmake_minimum_required(VERSION 3.10.2)

project(ftts)

cmake_host_system_information(RESULT _os QUERY OS_NAME)
cmake_host_system_information(RESULT _platform QUERY OS_PLATFORM)

message(STATUS "OS: " ${_os})
message(STATUS "Platform: " ${_platform})

if( MSVC )
 set (CMAKE_C_FLAGS "-D__AVX__")
 set (CMAKE_CXX_FLAGS "/std:c++17 /GR -D__AVX__")
else()
 set (CMAKE_C_FLAGS "-O3 -g -mavx -mfma")
 set (CMAKE_CXX_FLAGS "-std=c++17 -O3 -g -mavx -mfma")
endif()

if( MSVC )
  add_compile_options( "/EHsc" )
  add_compile_options( "/sdl-" )
endif()

add_library(ftts STATIC
	src/ftts.cpp)
target_include_directories(ftts PUBLIC ./include PRIVATE ./src)
