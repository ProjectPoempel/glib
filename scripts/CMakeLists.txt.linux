cmake_minimum_required(VERSION 3.5)

project(glib_dll LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)
find_package(OpenAL REQUIRED)

add_subdirectory(depend/glib-RapidXML)
add_subdirectory(depend/freetype)

add_definitions(-DGLIBDLL_EXPORTS)

file(GLOB_RECURSE SRC src/*.cpp)

add_library(glib_dll SHARED ${SRC})
target_include_directories(glib_dll PUBLIC "include" "depend/glib-RapidXML/RapidXML" "depend/stb" "depend/freetype/include" "depend/AudioFile")
target_link_libraries(glib_dll PUBLIC RapidXML glfw openal avcodec avformat swscale assimp)