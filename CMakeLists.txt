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
set(OPENFST_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/openfst/src/include")

add_subdirectory(./thirdparty/libiconv)
list(APPEND EXTRA_LIBS libiconv)
list(APPEND EXTRA_INCLUDES "${ftts_BINARY_DIR}/thirdparty/libiconv")

add_subdirectory(./thirdparty/mecab/mecab)
list(APPEND EXTRA_LIBS libmecab)
list(APPEND EXTRA_INCLUDES "./thirdparty/mecab/mecab/src")

add_subdirectory(./thirdparty/utf8proc)
list(APPEND EXTRA_LIBS utf8proc)
list(APPEND EXTRA_INCLUDES "./thirdparty/utf8proc")

add_subdirectory(./thirdparty/openfst)
list(APPEND EXTRA_LIBS fst)
list(APPEND EXTRA_INCLUDES "./thirdparty/openfst/src/include")

add_subdirectory(./thirdparty/Phonetisaurus)
list(APPEND EXTRA_LIBS phonetisaurus)
list(APPEND EXTRA_INCLUDES "./thirdparty/Phonetisaurus/src")
list(APPEND EXTRA_INCLUDES "./thirdparty/Phonetisaurus/src/3rdparty/utfcpp")

add_library(ftts SHARED 
	./include/ftts.h
	./src/ftts.cpp
	./src/Processor.h
	./src/Processor.cpp
	./src/JSProcessor.h
	./src/JSProcessor.cpp
	./src/LJSProcessor.h
	./src/LJSProcessor.cpp
	./src/PCH.h
	./src/PCH.cpp)

target_include_directories(ftts PUBLIC ./include PRIVATE ./src ${EXTRA_INCLUDES})
target_link_libraries (ftts LINK_PUBLIC ${EXTRA_LIBS})

add_executable (example src/example.cpp)
target_include_directories(example PRIVATE ./include ./src)
target_link_libraries (example LINK_PUBLIC ftts)
