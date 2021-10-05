/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#ifndef MATERIAL_H
#define MATERIAL_H

#include "Ray/Ray.h"

#include "RayTracer/RayTracer.h"

struct HitResult;

class Material {
public:
    virtual bool scatter(const Ray& rayIn, const HitResult& result, Vector3d& attenuation, Ray& rayScattered) const = 0;
};

#endif // MATERIAL_H