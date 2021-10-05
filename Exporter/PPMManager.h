/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#ifndef PPM_MANAGER_H
#define PPM_MANAGER_H

#include "RayTracer/RayTracer.h"

class PPMManager {
public:
    ~PPMManager();

    void startWrite(size_t width, size_t height);

    bool endWrite(const std::string& filename);

    void writeColor(size_t x, size_t y, Vector3i color);

    void writeColor(size_t x, size_t y, Vector3d color, bool gammaCorrection);

private:
    std::unique_ptr<std::ofstream> m_fout = nullptr;

    size_t m_width = 0, m_height = 0;
    std::vector<Vector3i> m_buffer = {};
};

#endif // PPM_MANAGER_H