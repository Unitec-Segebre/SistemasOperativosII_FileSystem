cmake_minimum_required(VERSION 3.8)
project(Clion)

set(CMAKE_CXX_STANDARD 11)

#include_directories(/usr/include/)
#link_directories(/usr/include/)
#target_link_libraries(Clion pthread)1

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lrt")
find_library(Clion rt)
set(SOURCE_FILES main.cpp BasicFileSystem.cpp BasicFileSystem.h BasicFileSystemMemory.cpp BasicFileSystemMemory.h BasicFileSystemDisk.cpp BasicFileSystemDisk.h FileSystem.cpp FileSystem.h)
add_executable(Clion ${SOURCE_FILES})

target_link_libraries(Clion rt)