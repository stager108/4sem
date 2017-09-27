cmake_minimum_required(VERSION 3.7)
project(HaffmanTree)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        cinbitstream.h
        coutbitstream.h
        coder.h
        decoder.h
        arhivator.cpp
        myheap.h
        )

add_executable(arhivator ${SOURCE_FILES})
