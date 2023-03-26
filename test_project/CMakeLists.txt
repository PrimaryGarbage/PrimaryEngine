cmake_minimum_required(VERSION 3.22)
project(PrimaryEngineTest LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(SRC_FILES 
    main.cpp
    test_scenes_creator.hpp
)

list(TRANSFORM SRC_FILES PREPEND "src/")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/lib)

target_include_directories(${PROJECT_NAME} PRIVATE external/primary_engine/include)

#target_link_options(${PROJECT_NAME} PRIVATE -static)

# remove c++20 volatile varnings (they will be fixed in c++23)
target_compile_options(${PROJECT_NAME} PUBLIC -Wno-volatile)

target_link_libraries(${PROJECT_NAME} PrimaryEngine-d)