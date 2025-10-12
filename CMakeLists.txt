cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

include_directories(glad/include/)

project(OpenGLTemplate)

add_executable(opengl-template main.cc)
target_link_libraries(opengl-template PRIVATE glfw)
target_compile_options(opengl-template PRIVATE "-Wno-c23-extensions")
