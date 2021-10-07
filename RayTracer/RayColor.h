/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#ifndef RAY_COLOR_H
#define RAY_COLOR_H

#include <memory>
#include <vector>

#include "GraphMath/Vector3.hpp"
#include "Ray/Ray.h"
#include "Shape/Shape.h"

Vector3d rayColor(const Ray& r, const std::vector<std::shared_ptr<Shape>>& shapeList, int depth, bool autoPriority);

#endif // RAY_COLOR_H
