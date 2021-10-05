/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#ifndef CAMERA_H
#define CAMERA_H

#include "Ray/Ray.h"

#include "RayTracer/RayTracer.h"

class Camera {
public:
    Camera(double aspectRatio, double viewportHeight) {
        m_aspectRatio = aspectRatio;
        m_viewportHeight = viewportHeight;
        m_viewportWidth = aspectRatio * viewportHeight;

        m_horizontal = { m_viewportWidth, 0.0, 0.0 };
        m_vertical = { 0.0, m_viewportHeight, 0.0 };
        m_lowerLeftCorner = m_origin - m_horizontal / 2 - m_vertical / 2 - Vector3d(0.0, 0.0, m_focalLength);
    }

    Ray getRay(double u, double v) const {
        return { m_origin, m_lowerLeftCorner + u * m_horizontal + v * m_vertical - m_origin };
    }

private:
    double m_aspectRatio = 1.0;
    double m_viewportHeight = 2.0;
    double m_viewportWidth = 2.0;
    double m_focalLength = 1.0;

    Vector3d m_origin = {};
    Vector3d m_horizontal = {};
    Vector3d m_vertical = {};
    Vector3d m_lowerLeftCorner = {};
};

#endif // CAMERA_H
