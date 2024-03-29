cmake_minimum_required(VERSION 3.22)
project(PrimaryEngine VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(BUILD_SHARED_LIBS OFF)
set(TEST_PROJECT_LIB_PATH "test_project/project/external/primary_engine")

set(PRIMARY_SRC
	main.cpp
	primary_app.cpp primary_app.hpp
	primary_app_options.hpp
	prim_exception.hpp
	input.cpp input.hpp input_utils.hpp
	logger.hpp logger.cpp
	timer.hpp
	utils.hpp utils.cpp
	globals.hpp
	scene_editor.hpp scene_editor.cpp
	scene_manager.hpp scene_manager.cpp
	ImGuiFileDialog.h ImGuiFileDialog.cpp ImGuiFileDialogConfig.h
	typedefs.hpp
	resource_manager.hpp resource_manager.cpp
	event.hpp
	)

set(MISC_SRC
	shape2d.hpp
	rectangle.hpp rectangle.cpp
)

set(NODES_SRC
	transform3d.hpp transform2d.hpp
	node.hpp node.cpp
	node2d.hpp node2d.cpp
	sprite.hpp sprite.cpp
	camera_base.hpp camera_base.cpp
	camera2d.hpp camera2d.cpp
	actor_camera2d.hpp actor_camera2d.cpp
	node_utils.hpp
	node_path.hpp node_path.cpp
	drawable.hpp drawable.cpp
	control.hpp control.cpp
	panel.hpp panel.cpp
	label.hpp label.cpp
	button.hpp button.cpp
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
	default_font_data.hpp default_shader_data.hpp default_texture_data.hpp
	glfw_extensions.hpp glfw_extensions.cpp
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
list(TRANSFORM MISC_SRC PREPEND "misc/")

list(APPEND PRIMARY_SRC ${NODES_SRC})
list(APPEND PRIMARY_SRC ${GRAPHICS_SRC})
list(APPEND PRIMARY_SRC ${MISC_SRC})

list(TRANSFORM PRIMARY_SRC PREPEND "src/")

list(APPEND PRIMARY_SRC ${IMGUI_SRC})

add_library(${PROJECT_NAME} SHARED ${PRIMARY_SRC})

#add_compile_definitions(USE_STD_FILESYSTEM)
add_compile_definitions(PRIMARY_BUILD_LIB)
add_compile_definitions(GLEW_STATIC)

# link statically after generating glfw and glew
set(CMAKE_LINK_SEARCH_START_STATIC ON)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -d)

target_include_directories(${PROJECT_NAME} PRIVATE
		${CMAKE_SOURCE_DIR} src external external/glfw external/glew external/glm external/imgui external/freetype2
	)

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/lib)

# remove c++20 volatile varnings (they will be fixed in c++23)
target_compile_options(${PROJECT_NAME} PUBLIC -Wno-deprecated-volatile -Wno-volatile)

# build type specifics
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	target_link_libraries(${PROJECT_NAME} PRIVATE glfw3 glewd freetyped)
else()
	target_link_libraries(${PROJECT_NAME} PRIVATE glfw3 glew freetype)
endif()

# system type specifics
if(WIN32)
	target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/lib/win)
	target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
elseif(UNIX)
	target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/lib/linux)
	target_link_libraries(${PROJECT_NAME} PRIVATE GL)
endif()