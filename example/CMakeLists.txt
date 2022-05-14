cmake_minimum_required(VERSION 3.16)
project(libcamera-gl-driver-example)

set(APP_SRC
	main.cpp
)

add_executable(${PROJECT_NAME}
	${APP_SRC}
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
	libcamera-gl-driver-lib
)


# add_executable(libcamera_meme
# 	main.cpp
# 	camera_grabber.cpp
# 	dma_buf_alloc.cpp
# 	gl_hsv_thresholder.cpp
# 	libcamera_opengl_utility.cpp
# )
# target_include_directories(libcamera_meme PUBLIC ${OPENGL_INCLUDE_DIRS} ${LIBDRM_INCLUDE_DIRS} ${LIBCAMERA_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
# target_link_libraries(libcamera_meme PUBLIC OpenGL::GL OpenGL::EGL Threads::Threads ${LIBCAMERA_LINK_LIBRARIES} ${OpenCV_LIBS})