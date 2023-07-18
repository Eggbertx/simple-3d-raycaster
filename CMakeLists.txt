cmake_minimum_required(VERSION 3.10)

project(simple-3d-raycaster VERSION 0.1 LANGUAGES C)
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

configure_file(include/simple3D_config.h.in include/simple3D_config.h)
set(SOURCES
	src/keys.c
	src/main.c
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE
	${GLUT_LIBRARIES}
	${OPENGL_LIBRARIES}
)
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_BINARY_DIR}/include")
