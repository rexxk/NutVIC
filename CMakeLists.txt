# NutVIC - Commodore VIC20/64/128 emulator
# By David Andersson 2026

cmake_minimum_required(VERSION 3.15)

include(FetchContent)


# Policy
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

# Content
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw GIT_TAG master)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui GIT_TAG docking)
FetchContent_MakeAvailable(imgui)


# Project
project(NutVIC)

# Static library - imgui
add_library(imgui ${imgui_SOURCE_DIR}/imgui.cpp
				  ${imgui_SOURCE_DIR}/imgui_demo.cpp					
				  ${imgui_SOURCE_DIR}/imgui_draw.cpp					
				  ${imgui_SOURCE_DIR}/imgui_tables.cpp					
				  ${imgui_SOURCE_DIR}/imgui_widgets.cpp					
				  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp					
				  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp					
)

target_include_directories(imgui PRIVATE ${imgui_SOURCE_DIR} ${glfw_SOURCE_DIR}/include)


# Static lib - glad
set(glad_SOURCE_DIR "vendor/glad")
add_library(glad ${glad_SOURCE_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${glad_SOURCE_DIR}/include)


# Executable
add_executable(NutVIC)

target_include_directories(NutVIC PRIVATE ${CMAKE_SOURCE_DIR}/src ${glfw_SOURCE_DIR}/include ${imgui_SOURCE_DIR} ${glad_SOURCE_DIR}/include)
target_link_libraries(NutVIC PRIVATE glfw imgui glad)

set_property(TARGET NutVIC PROPERTY CXX_STANDARD 26)
set_property(TARGET NutVIC PROPERTY DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

set(CMAKE_DEFAULT_STARTUP_PROJECT NutVIC)

add_subdirectory(src)
