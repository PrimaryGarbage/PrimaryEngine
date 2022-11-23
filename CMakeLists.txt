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
	input.cpp input.hpp input_utils.hpp
	logger.hpp logger.cpp
	timer.hpp
	utils.hpp
	globals.hpp
	scene_editor.hpp scene_editor.cpp
	scene_manager.hpp scene_manager.cpp
	test_scenes_creator.hpp
	ImGuiFileDialog.h ImGuiFileDialog.cpp ImGuiFileDialogConfig.h
	typedefs.hpp
	resource_manager.hpp resource_manager.cpp
	)

set(NODES_SRC
	transform3d.hpp transform2d.hpp
	node_base.hpp node_base.cpp
	node.hpp node.cpp
	node2d.hpp node2d.cpp
	sprite.hpp sprite.cpp
	camera_base.hpp camera_base.cpp
	camera2d.hpp camera2d.cpp
	actor_camera2d.hpp actor_camera2d.cpp
	node_utils.hpp
	node_path.hpp node_path.cpp
	drawable.hpp drawable.cpp
	node_factory.hpp
	control.hpp control.cpp
	label.hpp label.cpp
)

set(GRAPHICS_SRC
	renderer.hpp renderer.cpp
	vertex_buffer.hpp vertex_buffer.cpp
	index_buffer.hpp index_buffer.cpp
	vertex_buffer_layout.hpp vertex_buffer_layout.cpp
	#vertex_array.hpp vertex_array.cpp
	shader.hpp shader.cpp
	stb_image.h stb_image.cpp
	texture.hpp texture.cpp
	mesh.hpp mesh.cpp
	mesh_composition.hpp
	primitives.hpp primitives.cpp
	image.hpp image.cpp image_utils.hpp
	font.hpp font.cpp
)

set(IMGUI_SRC
	imgui.h imgui.cpp imgui_demo.cpp
	imgui_impl_glfw.h imgui_impl_glfw.cpp
	imgui_impl_opengl3.h imgui_impl_opengl3.cpp imgui_impl_opengl3_loader.h
	imconfig.h imgui_draw.cpp imgui_internal.h
	imgui_tables.cpp imgui_widgets.cpp
	imstb_rectpack.h imstb_textedit.h imstb_truetype.h
)

list(TRANSFORM NODES_SRC PREPEND "nodes/")
list(TRANSFORM GRAPHICS_SRC PREPEND "graphics/")
list(TRANSFORM IMGUI_SRC PREPEND "external/imgui/")

list(APPEND PRIMARY_SRC ${NODES_SRC})
list(APPEND PRIMARY_SRC ${GRAPHICS_SRC})

list(TRANSFORM PRIMARY_SRC PREPEND "src/")

list(APPEND PRIMARY_SRC ${IMGUI_SRC})

add_executable(${PROJECT_NAME} ${PRIMARY_SRC})

target_compile_options(${PROJECT_NAME} PRIVATE -static)

add_compile_definitions(USE_STD_FILESYSTEM)

add_subdirectory(external/glfw)
add_subdirectory(external/glew)

# link statically after generating glfw and glew
set(CMAKE_LINK_SEARCH_START_STATIC ON)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -d)

target_include_directories(${PROJECT_NAME} PRIVATE
		external/glfw/include external/glew/include external/glm src/graphics src/nodes src external/imgui 
	)

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/external/glfw/src ${CMAKE_BINARY_DIR}/external/glew/lib, ${CMAKE_BINARY_DIR}/external/freetype2)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
	file(COPY external/freetype2/builds/freetype.lib DESTINATION ${CMAKE_BINARY_DIR}/external/freetype2)
elseif(UNIX)
	target_link_libraries(${PROJECT_NAME} PRIVATE GL)
	add_subdirectory(external/freetype2)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE glfw libglew_static freetype)
