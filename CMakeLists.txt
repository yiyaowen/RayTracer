cmake_minimum_required(VERSION 3.20)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE GRAPH_MATH_INCLUDE GraphMath/*.hpp)

add_executable(RayTracer
    # Headers
    ${GRAPH_MATH_INCLUDE}
    Camera/Camera.h
    Concurrency/PartialProcessor.h
    Exporter/ExporterManager.h
    Exporter/stb_image_write.h
    Material/Dielectric.h
    Material/Lambertian.h
    Material/Material.h
    Material/Metal.h
    Ray/Ray.h
    RayTracer/RayColor.h
    RayTracer/RayTracer.h
    Scene/Scene.h
    Shape/Shape.h
    Shape/Sphere.h

    # Sources
    Concurrency/PartialProcessor.cpp
    Camera/Camera.cpp
    Exporter/ExporterManager.cpp
    Ray/Ray.cpp
    RayTracer/RayColor.cpp
    RayTracer/RayTracer.cpp
    Scene/Scene.cpp
    Shape/Sphere.cpp
)
