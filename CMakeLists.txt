cmake_minimum_required(VERSION 3.10)

project(simple-3d-raycaster VERSION 0.1 LANGUAGES C)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)

configure_file(include/simple3D_util.h.in include/simple3D_util.h)
set(SOURCES
	src/simple3D_util.c
	src/keys.c
	src/actors.c
	src/map.c
	src/graphics.c
	src/drawing.c
	src/main.c
)

add_executable(${PROJECT_NAME} ${SOURCES})
if(NOT WIN32)
	target_link_libraries(${PROJECT_NAME} m)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE
	${SDL2_LIBRARIES}
	${SDL2IMAGE_LIBRARIES}
	${OPENGL_LIBRARIES}
)
target_include_directories(${PROJECT_NAME}
	PRIVATE "${PROJECT_BINARY_DIR}/include" ${SDL_INCLUDE_DIRS})
