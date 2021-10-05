/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#include "Sphere.h"

bool Sphere::hit(const Ray& r, double t_min, double t_max, HitResult& result) const {
    Vector3d oc = r.origin() - m_center;
    auto a = r.direction().length2();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length2() - m_radius * m_radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0) return false;

    auto sqrtd = sqrt(discriminant);

    // In general, oc is in the opposite direction of r.direction(), i.e. half_b < 0.
    // Here we try to check the nearest intersect position firstly.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || root > t_max) {
            return false;
        }
    }

    result.t = root;
    result.position = r.at(result.t);
//    result.isOuter = (r.origin() - m_center).length() > m_radius; // Bug occurred when near surface!
    Vector3d outwardNormal = result.position - m_center;
    result.isOuter = dot(r.direction(), outwardNormal) < 0.0;
    result.normal = result.isOuter ? outwardNormal : -outwardNormal;
    result.material = m_material;

    return true;
}
