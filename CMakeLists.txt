cmake_minimum_required(VERSION 3.22)
project(PrimaryEngine VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR}bin)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(PRIMARY_SRC
	main.cpp
	primary_app.cpp primary_app.hpp
	)

list(TRANSFORM PRIMARY_SRC PREPEND "src/")

add_executable(${PROJECT_NAME} ${PRIMARY_SRC})

target_compile_options(${PROJECT_NAME} PRIVATE -static)

add_subdirectory(external/glfw)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -d)

target_link_directories(${PROJECT_NAME} PRIVATE external/glfw/src)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
