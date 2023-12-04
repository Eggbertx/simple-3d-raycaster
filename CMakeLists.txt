cmake_minimum_required(VERSION 3.10)

project(simple-3d-raycaster VERSION 0.1 LANGUAGES C)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

set(LINK_LIBRARIES
	${SDL2_LIBRARIES}
	${SDL2IMAGE_LIBRARIES}
	${OPENGL_LIBRARIES}
)

if(WIN32)
	find_package(SDL2_image REQUIRED)
else()
	include(FindPkgConfig)
	pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
	set(LINK_LIBRARIES
		${LINK_LIBRARIES}
		m
		${SDL2_IMAGE_LIBRARIES}
	)
endif()

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
target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIBRARIES})
target_include_directories(${PROJECT_NAME}
	PRIVATE "${PROJECT_BINARY_DIR}/include" ${SDL_INCLUDE_DIRS})
