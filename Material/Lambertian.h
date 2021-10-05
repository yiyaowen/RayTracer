/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "Material.h"
#include "Shape/Shape.h"

class Lambertian : public Material {
public:
    explicit Lambertian(const Vector3d& albedo) : m_albedo(albedo) {}

    bool scatter(const Ray &rayIn, const HitResult &result, Vector3d &attenuation, Ray &rayScattered) const override {
        auto scatterDirection = result.normal + randomUnitSphereSurface();

        if (scatterDirection.nearZero()) {
            scatterDirection = result.normal;
        }

        rayScattered = Ray(result.position, scatterDirection);
        attenuation = m_albedo;
        return true;
    }

protected:
    Vector3d m_albedo = {};
};

#endif // LAMBERTIAN_H