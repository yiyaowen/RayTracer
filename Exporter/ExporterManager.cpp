/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#include "ExporterManager.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

ExporterManager::~ExporterManager() {
    // In case that forget to release file handle.
    if (m_fout != nullptr) {
        m_fout->close();
        m_fout.reset(nullptr);
    }
}

void ExporterManager::startWrite(size_t width, size_t height) {
    m_width = width;
    m_height = height;
    m_buffer.resize(width * height);
}

bool ExporterManager::endWrite(const std::string& filename, FileType type) {
    switch (type) {
        case PPM: {
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
        case PNG: {
            std::vector<unsigned char> buffer(m_width * m_height * 4); // RGBA
            for (size_t i = 0; i < m_buffer.size(); ++i) {
                buffer[4 * i] = m_buffer[i].r();
                buffer[4 * i + 1] = m_buffer[i].g();
                buffer[4 * i + 2] = m_buffer[i].b();
                buffer[4 * i + 3] = 255;
            }
            stbi_write_png(filename.c_str(), m_width, m_height, 4, buffer.data(), 0);

            return true;
        }
        default:
            return false;
    }
}

void ExporterManager::writeColor(size_t x, size_t y, Vector3i color) {
    m_buffer[x + y * m_width] = color;
}

void ExporterManager::writeColor(size_t x, size_t y, Vector3d color, bool gammaCorrection) {
    if (gammaCorrection) {
        color = { sqrt(color.r()), sqrt(color.g()), sqrt(color.b()) }; // Simple gamma2 correction
    }
    color *= 254.999;
    Vector3i rgb = { static_cast<int>(color.r()), static_cast<int>(color.g()), static_cast<int>(color.b()) };
    writeColor(x, y, rgb);
}
