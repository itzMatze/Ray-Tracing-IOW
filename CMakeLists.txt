cmake_minimum_required(VERSION 3.19)
project(Ray_Tracing_IOW)

set(CMAKE_CXX_STANDARD 20)

add_executable(Ray_Tracing_IOW main.cpp ray.cpp ray.h hitable.h sphere.cpp sphere.h hitable_list.cpp hitable_list.h)

target_include_directories(Ray_Tracing_IOW PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}"
        "C:/dev/stb/01.04"
        "C:/dev/VulkanSDK/1.2.176.1/Third-Party/Include/glm")
