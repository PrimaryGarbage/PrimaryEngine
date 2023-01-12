cmake_minimum_required(VERSION 3.22)
project(PrimaryEngineTest LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR}bin)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(PRIMARY_ENGINE_LIB ${CMAKE_SOURCE_DIR}/external/primary_engine/libPrimaryEngine-d.dll)

set(SRC_FILES 
    main.cpp
    test_scenes_creator.hpp
)

list(TRANSFORM SRC_FILES PREPEND "src/")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_directories(${PROJECT_NAME} PRIVATE external)

target_include_directories(${PROJECT_NAME} PRIVATE src external/primary_engine/include)

target_link_options(${PROJECT_NAME} PRIVATE -static)

target_link_libraries(${PROJECT_NAME} ${PRIMARY_ENGINE_LIB})