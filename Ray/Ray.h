/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#ifndef RAY_H
#define RAY_H

#include "RayTracer/RayTracer.h"

class Ray {
public:
    Ray() = default;
    Ray(const Vector3d& origin, const Vector3d& direction) : m_origin(origin), m_direction(direction) {}

    Vector3d at(double t) const { return m_origin + t * m_direction; }

public:
    inline Vector3d origin() const { return m_origin; }
    inline void setOrigin(const Vector3d& value) { m_origin = value; }

    inline Vector3d direction() const { return m_direction; }
    inline void setDirection(const Vector3d& value) { m_direction = value; }

private:
    Vector3d m_origin = {};
    Vector3d m_direction = {};
};

#endif // RAY_H