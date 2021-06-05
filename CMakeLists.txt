cmake_minimum_required(VERSION 3.19)
project(Ray_Tracing_IOW)

set(CMAKE_CXX_STANDARD 20)

add_executable(Ray_Tracing_IOW main.cpp ray.cpp ray.h camera.h
        objects/hitable.h objects/sphere.cpp objects/sphere.h objects/hitable_list.cpp objects/hitable_list.h
        materials/material.h materials/lambertian.cpp materials/lambertian.h materials/metal.cpp materials/metal.h hit_record.h materials/dielectric.cpp materials/dielectric.h
        sdl_window/color.cpp sdl_window/color.h sdl_window/renderer.cpp sdl_window/renderer.h)

find_library(sdl SDL2.lib "C:/dev/VulkanSDK/1.2.176.1/Third-Party/Bin/")

target_link_libraries(Ray_Tracing_IOW
        ${sdl})

target_include_directories(Ray_Tracing_IOW PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}"
        "C:/dev/stb/01.04"
        "C:/dev/VulkanSDK/1.2.176.1/Third-Party/Include/glm"
        "C:/dev/VulkanSDK/1.2.176.1/Third-Party/Include/SDL2")

configure_file("C:/dev/VulkanSDK/1.2.176.1/Third-Party/Bin/SDL2.dll"
        "${PROJECT_SOURCE_DIR}/cmake-build-debug/" COPYONLY)