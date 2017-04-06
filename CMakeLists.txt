cmake_minimum_required(VERSION 3.6)
project(RayTracer)

set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

#set(CMAKE_MODULE_PATH cmake_modules)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#set(SFML_ROOT "C:\\SFML")

find_package(SFML 2.1 COMPONENTS system window graphics REQUIRED)
include_directories(${SFML_INCLUDE_DIR} ${PROJECT_SOURCE_DIR})


set(SOURCE_FILES main.cpp Screen.h geom_utils.h primitive.h scene.h
        RayTracer.h kd.h structures.h colour.h geom_utils.cpp colour.cpp)
add_executable(RayTracer ${SOURCE_FILES})
target_link_libraries(RayTracer ${SFML_LIBRARIES})
