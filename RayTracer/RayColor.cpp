/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#include "Material/Material.h"

#include "RayColor.h"

Vector3d rayColor(const Ray& r, const std::vector<std::shared_ptr<Shape>>& shapeList, int depth, bool autoPriority) {
    HitResult hit = {};

    // In case of stack overflow.
    if (depth <= 0) return Vector3d::zero();

    /*
     * Compare depth priority automatically.
     */
    if (autoPriority) {
        HitResult nearHit = {};
        nearHit.t = infinity;
        for (const auto& shape : shapeList) {
            if (shape->hit(r, 0.001, infinity, hit)) { // Do not hit children.
                if (hit.t > 0.0 && hit.t < nearHit.t) {
                    nearHit = hit;
                }
            }
        }
        if (nearHit.t < infinity) {
            Ray rayScattered = {};
            Vector3d attenuation = {};
            if (nearHit.material->scatter(r, nearHit, attenuation, rayScattered)) {
                return attenuation * rayColor(rayScattered, shapeList, depth - 1, autoPriority);
            }
            else {
                return Vector3d::zero();
            }
        }
        else {
            // Default sky background.
            Vector3d unitDirection = normalize(r.direction());
            auto t = 0.5 * (unitDirection.y() + 1.0);
            return (1.0 - t) * Vector3d(1.0, 1.0, 1.0) + t * Vector3d(0.5, 0.7, 1.0);
        }
    }
    /*
    * Use designated priority.
    */
    else {
        for (const auto& shape : shapeList) {
            if (shape->hit(r, 0.001, infinity, hit)) { // Do not hit children.
                Ray rayScattered = {};
                Vector3d attenuation = {};
                if (hit.material->scatter(r, hit, attenuation, rayScattered)) {
                    return attenuation * rayColor(rayScattered, shapeList, depth - 1, autoPriority);
                }
                else {
                    return Vector3d::zero();
                }
            }
        }
        // Default sky background.
        Vector3d unitDirection = normalize(r.direction());
        auto t = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - t) * Vector3d(1.0, 1.0, 1.0) + t * Vector3d(0.5, 0.7, 1.0);
    }
}