cmake_minimum_required(VERSION 3.20)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE GRAPH_MATH_INCLUDE GraphMath/*.hpp)

add_executable(RayTracer
    # Headers
    ${GRAPH_MATH_INCLUDE}
    Camera/Camera.h
    Exporter/PPMManager.h
    Material/Dielectric.h
    Material/Lambertian.h
    Material/Material.h
    Material/Metal.h
    Ray/Ray.h
    RayTracer/RayTracer.h
    Shape/Shape.h
    Shape/Sphere.h

    # Sources
    Camera/Camera.cpp
    Exporter/PPMManager.cpp
    Ray/Ray.cpp
    RayTracer/RayTracer.cpp
    Shape/Sphere.cpp
)
