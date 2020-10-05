cmake_minimum_required(VERSION 3.10.2)
project(ftts)

cmake_host_system_information(RESULT _os QUERY OS_NAME)
cmake_host_system_information(RESULT _platform QUERY OS_PLATFORM)

message(STATUS "OS: " ${_os})
message(STATUS "Platform: " ${_platform})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_BINARY FORCE OFF)
option(BUILD_SHARED_LIBS FORCE OFF)

set(ICONV_INCLUDE_DIRS  "${ftts_BINARY_DIR}/thirdparty/libiconv")

add_subdirectory(./thirdparty/libiconv)
list(APPEND EXTRA_LIBS libiconv)
list(APPEND EXTRA_INCLUDES "${ftts_BINARY_DIR}/thirdparty/libiconv")

add_subdirectory(./thirdparty/mecab/mecab)
list(APPEND EXTRA_LIBS libmecab)
list(APPEND EXTRA_INCLUDES "./thirdparty/mecab/mecab/src")

add_subdirectory(./thirdparty/utf8proc)
list(APPEND EXTRA_LIBS utf8proc)
list(APPEND EXTRA_INCLUDES "./thirdparty/utf8proc")

add_library(ftts STATIC src/ftts.cpp)
target_include_directories(ftts PUBLIC ./include PRIVATE ./src ${EXTRA_INCLUDES})

add_executable (example src/example.cpp)
target_include_directories(example PRIVATE ./include ./src)
target_link_libraries (example LINK_PUBLIC ftts ${EXTRA_LIBS})
