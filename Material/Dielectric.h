/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "Material.h"
#include "Shape/Shape.h"

class Dielectric : public Material {
public:
    explicit Dielectric(double refractionIndex) : m_refractionIndex(refractionIndex) {}

    bool scatter(const Ray &rayIn, const HitResult &result, Vector3d &attenuation, Ray &rayScattered) const override {
        attenuation = Vector3d(1.0, 1.0, 1.0);
        double actualRefractionIndex = result.isOuter ? m_refractionIndex : (1.0 / m_refractionIndex);

        Vector3d unitDirection = normalize(rayIn.direction());
        double cosTheta = dot(result.normal, -unitDirection);
        double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

        bool isTotalReflected = actualRefractionIndex * sinTheta > 1.0;

        Vector3d rayRefracted = {};
        // Total reflection & Fresnel effect
//        if (isTotalReflected || schlickApproximation(cosTheta, actualRefractionIndex) > randomReal()) {
//            rayRefracted = reflect(unitDirection, result.normal);
//        }
//        else {
            rayRefracted = refract(unitDirection, result.normal, actualRefractionIndex);
//        }

        rayScattered = Ray(result.position, rayRefracted);
        return true;
    }

private:
    inline static double schlickApproximation(double cosine, double refractiveIndex) {
        double r0 = (1.0 - refractiveIndex) / (1.0 + refractiveIndex);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow(1.0 - cosine, 5);
    }

private:
    double m_refractionIndex = 0.0;
};

#endif // DIELECTRIC_H