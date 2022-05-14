cmake_minimum_required(VERSION 3.16)
project(libcamera-gl-driver VERSION 0.0.1 DESCRIPTION  "LibCamera + OpenGL driver for GPU accelerated capture and HSV/grayscale")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL)
find_package(OpenCV REQUIRED)

pkg_check_modules(LIBDRM REQUIRED libdrm)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)

add_subdirectory(driver)
add_subdirectory(example)