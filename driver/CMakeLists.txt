cmake_minimum_required(VERSION 3.16)
project(libcamera-gl-driver-lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIB_SRC
	src/camera_grabber.cpp
	src/dma_buf_alloc.cpp
	src/gl_hsv_thresholder.cpp
	src/libcamera_opengl_utility.cpp
	src/camera_runner.cpp
)

set(LIB_HDR
	inc/camera_grabber.h
	inc/dma_buf_alloc.h
	inc/gl_hsv_thresholder.h
	inc/libcamera_opengl_utility.h
	inc/concurrent_blocking_queue.h
	inc/camera_runner.h
)

add_library(${PROJECT_NAME} SHARED
	${LIB_SRC}
)

target_include_directories(${PROJECT_NAME} PUBLIC
	inc/
)

target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc/>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc/widget/>
	$<INSTALL_INTERFACE:${include_dest}/${PROJECT_NAME}>
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${OPENGL_INCLUDE_DIRS}
	${LIBDRM_INCLUDE_DIRS}
	${LIBCAMERA_INCLUDE_DIRS}
	${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
	OpenGL::GL
	OpenGL::EGL
	Threads::Threads
	${LIBCAMERA_LINK_LIBRARIES}
	${OpenCV_LIBS}
)

install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION lib
	PUBLIC_HEADER DESTINATION include/libcamera-gl-driver-lib
)
