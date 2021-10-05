/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#ifndef SPHERE_H
#define SPHERE_H

#include "Ray/Ray.h"
#include "Shape.h"

class Sphere : public Shape {
public:
    Sphere() = default;
    Sphere(Vector3d center, double radius, const std::string& label="sphere",
           std::shared_ptr<Material> material = nullptr, uint32_t priority=0)
        : m_center(center), m_radius(radius), Shape(label, material, priority) {}

public:
    bool hit(const Ray &r, double t_min, double t_max, HitResult &result) const override;

private:
    Vector3d m_center = {};
    double m_radius = 0.0f;
};

#endif //SPHERE_H
