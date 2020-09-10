cmake_minimum_required(VERSION 3.10.2)

project(ftts)

cmake_host_system_information(RESULT _os QUERY OS_NAME)
cmake_host_system_information(RESULT _platform QUERY OS_PLATFORM)

message(STATUS "OS: " ${_os})
message(STATUS "Platform: " ${_platform})


add_subdirectory(./thirdparty/iconv)
list(APPEND EXTRA_LIBS iconv)
list(APPEND EXTRA_INCLUDES "./thirdparty/iconv/include")

add_subdirectory(./thirdparty/mecab/mecab)
list(APPEND EXTRA_LIBS libmecab)
list(APPEND EXTRA_INCLUDES "./thirdparty/mecab/mecab/src")

add_subdirectory(./thirdparty/uchardet)
list(APPEND EXTRA_LIBS uchardet)
list(APPEND EXTRA_INCLUDES "./thirdparty/uchardet/src")

add_subdirectory(./thirdparty/utf8proc)
list(APPEND EXTRA_LIBS utf8proc)
list(APPEND EXTRA_INCLUDES "./thirdparty/utf8proc")

add_library(ftts STATIC src/ftts.cpp)
target_include_directories(ftts PUBLIC ./include PRIVATE ./src)

add_executable (example src/example.cpp)
target_include_directories(example PRIVATE ./include ./src ${EXTRA_INCLUDES})
target_link_libraries (example LINK_PUBLIC ftts ${EXTRA_LIBS})
