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

option(MAKE_SHARED "Build shared library (default on, off builds static)" ON)
option(BUILD_EXAMPLE "Build simple example (default on)" ON)
option(BUILD_EXAMPLE_GUI "Build ImGui example (default off)" OFF)

add_subdirectory(driver)

if (BUILD_EXAMPLE)
	message("Building simple example")
	add_subdirectory(example/simple)
endif()

if (BUILD_EXAMPLE_GUI)
	message("Building ImGui example")
	add_subdirectory(example/gui)
endif()
