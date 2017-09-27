cmake_minimum_required(VERSION 3.5.1)
project(allocator)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_COMPILER clang++)
#set(SOURCE_FILES main.cpp MainHeap.h Allocator.h Heap.h Superblock.h Config.h Superblock.cpp Heap.cpp MainHeap.cpp Allocator.cpp main.cpp )
set(SOURCE_FILES main.cpp mtallocator.h)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -latomic -pthread -Wall -Wextra -fsanitize=address ")
add_executable(allocator ${SOURCE_FILES})
