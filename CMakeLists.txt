cmake_minimum_required(VERSION 3.22)
project(PrimaryEngine VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR}bin)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(glew-cmake_BUILD_SHARED OFF)						# glew stuff
set(ONLY_LIBS ON)										# glew stuff
set(PKG_CONFIG_REPRESENTATIVE_TARGET libglew_static)	# glew stuff

set(PRIMARY_SRC
	main.cpp
	primary_app.cpp primary_app.hpp
	prim_exception.hpp
	logger.hpp logger.cpp
	renderer.hpp renderer.cpp
	vertex_buffer.hpp vertex_buffer.cpp
	index_buffer.hpp index_buffer.cpp
	vertex_buffer_layout.hpp vertex_buffer_layout.cpp
	vertex_array.hpp vertex_array.cpp
	shader.hpp shader.cpp
	stb_image.h stb_image.cpp
	texture.hpp texture.cpp
	mesh.hpp mesh.cpp
	mesh_composition.hpp
	model.hpp
	)

list(TRANSFORM PRIMARY_SRC PREPEND "src/")

add_executable(${PROJECT_NAME} ${PRIMARY_SRC})

target_compile_options(${PROJECT_NAME} PRIVATE -static)

add_subdirectory(external/glfw)
add_subdirectory(external/glew)

# link statically after generating glfw and glew
set(CMAKE_LINK_SEARCH_START_STATIC ON)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -d)

target_include_directories(${PROJECT_NAME} PRIVATE external/glfw/include external/glew/include external/glm)

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/external/glfw/src ${CMAKE_BINARY_DIR}/external/glew/lib)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
elseif(UNIX)
	target_link_libraries(${PROJECT_NAME} PRIVATE GL)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE glfw libglew_static)
