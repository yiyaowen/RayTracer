/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#include "PPMManager.h"

PPMManager::~PPMManager() {
    // In case that forget to release file handle.
    if (m_fout != nullptr) {
        m_fout->close();
        m_fout.reset(nullptr);
    }
}

void PPMManager::startWrite(size_t width, size_t height) {
    m_width = width;
    m_height = height;
    m_buffer.resize(width * height);
}

bool PPMManager::endWrite(const std::string& filename) {
    m_fout = std::make_unique<std::ofstream>(filename);

    auto& fout = *m_fout;
    if (!fout.is_open()) {
        return false;
    }

    fout << "P3\n" <<  m_width << ' ' << m_height << "\n255\n";
    for (int j = 0; j < m_height; ++j) {
        for (int i = 0; i < m_width; ++i) {
            fout << m_buffer[i + j * m_width] << '\n';
        }
    }

    m_fout->close();
    m_fout.reset(nullptr);

    return true;
}

void PPMManager::writeColor(size_t x, size_t y, Vector3i color) {
    m_buffer[x + y * m_width] = color;
}

void PPMManager::writeColor(size_t x, size_t y, Vector3d color, bool gammaCorrection) {
    if (gammaCorrection) {
        color = { sqrt(color.r()), sqrt(color.g()), sqrt(color.b()) }; // Simple gamma2 correction
    }
    color *= 255.999;
    Vector3i rgb = { static_cast<int>(color.r()), static_cast<int>(color.g()), static_cast<int>(color.b()) };
    writeColor(x, y, rgb);
}
