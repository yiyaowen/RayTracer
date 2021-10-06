/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#ifndef EXPORTER_MANAGER_H
#define EXPORTER_MANAGER_H

#include "RayTracer/RayTracer.h"

class ExporterManager {
public:
    using FileType = int;
    constexpr static FileType PPM = 0;
    constexpr static FileType PNG = 1;

public:
    ~ExporterManager();

    void startWrite(size_t width, size_t height);

    bool endWrite(const std::string& filename, FileType type);

    void writeColor(size_t x, size_t y, Vector3i color);

    void writeColor(size_t x, size_t y, Vector3d color, bool gammaCorrection = true);

private:
    std::unique_ptr<std::ofstream> m_fout = nullptr;

    size_t m_width = 0, m_height = 0;
    std::vector<Vector3i> m_buffer = {};
};

#endif // EXPORTER_MANAGER_H