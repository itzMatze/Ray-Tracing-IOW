cmake_minimum_required(VERSION 3.19)
project(Ray_Tracing_IOW)

set(CMAKE_CXX_STANDARD 20)

add_executable(Ray_Tracing_IOW main.cpp ray.cpp ray.h objects/hitable.h objects/sphere.cpp objects/sphere.h objects/hitable_list.cpp objects/hitable_list.h camera.h materials/material.h materials/lambertian.cpp materials/lambertian.h materials/metal.cpp materials/metal.h hit_record.h materials/dielectric.cpp materials/dielectric.h)

target_include_directories(Ray_Tracing_IOW PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}"
        "C:/dev/stb/01.04"
        "C:/dev/VulkanSDK/1.2.176.1/Third-Party/Include/glm")
