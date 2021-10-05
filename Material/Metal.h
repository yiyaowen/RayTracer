/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#ifndef METAL_H
#define METAL_H

#include "Material.h"
#include "Shape/Shape.h"

class Metal : public Material {
public:
    Metal(const Vector3d& albedo, double fuzz = 0.0) : m_albedo(albedo), m_fuzz(fuzz < 1.0 ? fuzz : 1.0) {}

    bool scatter(const Ray &rayIn, const HitResult &result, Vector3d &attenuation, Ray &rayScattered) const override {
        Vector3d reflected = reflect(normalize(rayIn.direction()), result.normal);
        rayScattered = Ray(result.position, reflected + m_fuzz * randomUnitSphere());
        attenuation = m_albedo;
        return (dot(rayScattered.direction(), result.normal) > 0);
    }

private:
    Vector3d m_albedo = {};

    double m_fuzz = 0.0;
};

#endif // METAL_H