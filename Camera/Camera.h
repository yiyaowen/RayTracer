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
    Camera(double aspectRatio,
           double aperture,
           double focusDistance,
           double verticalFov,
           Vector3d position,
           Vector3d lookAt,
           Vector3d up)
   {
        m_aspectRatio = aspectRatio;
        m_lensRadius = aperture * 0.5;
        m_focusDistance = focusDistance;

        double theta = radians(verticalFov);
        double h = tan(0.5 * theta);
        m_viewportHeight = 2.0 * h;
        m_viewportWidth = aspectRatio * m_viewportHeight;

        w = normalize(position - lookAt);
        u = normalize(cross(up, w));
        v = cross(w, u);

        m_origin = position;
        m_horizontal = u * m_viewportWidth * m_focusDistance;
        m_vertical = v * m_viewportHeight * m_focusDistance;
        m_lowerLeftCorner = m_origin - m_horizontal * 0.5 - m_vertical * 0.5 - w * m_focusDistance;
    }

    Ray getRay(double s, double t) const {
        Vector3d rd =  m_lensRadius * randomUnitDisk();
        Vector3d offset = u * rd.x() + v * rd.y();

        return { m_origin + offset,
                 m_lowerLeftCorner + s * m_horizontal + t * m_vertical - m_origin - offset };
    }

private:
    double m_aspectRatio = 1.0;
    double m_lensRadius = 0.0f;
    double m_focusDistance = 0.0f;

    double m_viewportHeight = 2.0;
    double m_viewportWidth = 2.0;
    double m_focalLength = 1.0;

    Vector3d u = {}, v = {}, w = {};

    Vector3d m_origin = {};
    Vector3d m_horizontal = {};
    Vector3d m_vertical = {};
    Vector3d m_lowerLeftCorner = {};
};

#endif // CAMERA_H
