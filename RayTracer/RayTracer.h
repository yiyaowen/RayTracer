/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#ifndef RAY_TRACER_H
#define RAY_TRACER_H

// Includes

#include <algorithm>

#include <cmath>

#include <chrono>

#include <exception>

#include <fstream>

#include <functional>

#include <iostream>

#include <memory>

#include <optional>

#include <string>

#include <vector>

#include "GraphMath/GraphMath.hpp"
using namespace gmath;

// Constants

#include <limits>
constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932384626;

// Tools

inline double radians(double degree) {
    return degree * pi / 180.0;
}

inline double degree(double radians) {
    return radians * 180.0 / pi;
}

#include <random>

extern std::default_random_engine defaultRandomEngine;

inline double randomReal() {
    std::uniform_real_distribution<double> range(0.0, 1.0f); // [0.0, 1.0]
    return range(defaultRandomEngine);
}

inline double randomReal(double min, double max) {
    std::uniform_real_distribution<double> range(min, max); // [min, max]
    return range(defaultRandomEngine);
}

inline Vector3d randomVec3d() {
    return { randomReal(), randomReal(), randomReal() };
}

inline Vector3d randomVec3d(double min, double max) {
    return { randomReal(min, max), randomReal(min, max), randomReal(min, max) };
}

// Rejection method
inline Vector3d randomUnitSphere() {
    while (true) {
        auto p = randomVec3d(-1.0, 1.0);
        if (p.length2() >= 1.0) continue;
        return p;
    }
}

inline Vector3d randomUnitSphereSurface() {
    return normalize(randomUnitSphere());
}

inline Vector3d randomUnitHemisphere(const Vector3d& normal) {
    auto v = randomUnitSphere();
    if (dot(v, normal) >= 0.0) {
        return v;
    }
    else {
        return -v;
    }
}

#endif // RAY_TRACER_H