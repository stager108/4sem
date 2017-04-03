cmake_minimum_required(VERSION 3.6)
project(RayTracer)

set (CMAKE_PREFIX_PATH "C:\\Qt\\5.8\\msvc2015_64")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)


set(SOURCE_FILES main.cpp mainwindow.cpp mainwindow.h Screen.h geom_utils.h primitive.h scene.h
        RayTracer.h kd.h structures.h colour.h geom_utils.cpp colour.cpp)
find_package(Qt5Widgets REQUIRED)
add_executable(RayTracer ${SOURCE_FILES})
target_link_libraries(RayTracer Qt5::Widgets)
