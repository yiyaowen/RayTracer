/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#include "PartialProcessor.h"

#include "RayTracer/RayColor.h"

void PartialProcessor::process() {
//    std::cout << "Partial ID " << m_ID << " starts processing..." << std::endl;
    auto& info = m_sceneInfo;
    for (int j = info.heightRange.first; j <= info.heightRange.second; ++j) {
//        std::cout << "Partial ID " << m_ID << ", scan line " << j - info.heightRange.first + 1 << " / " << m_partialHeight << std::endl;
        for (int i = info.widthRange.first; i <= info.widthRange.second; ++i) {
            Vector3d color = Vector3d::zero();
            // Sample near points randomly.
            for (int s = 0; s < info.sampleCount; ++s) {
                auto u = (double(i) + randomReal()) / static_cast<double>(info.fullSize.first - 1);
                // Flip y-axis to make view-coord matches with NDC-coord.
                auto v = (double(info.fullSize.second - 1 - j) + randomReal()) / static_cast<double>(info.fullSize.second - 1);
                auto r = info.camera->getRay(u, v);
                color += rayColor(r, info.shapeList, info.maxDepth, true);
            }
            // Flip y-axis to make view-coord matches with NDC-coord.
            writeColor(i - info.widthRange.first, j - info.heightRange.first, color / info.sampleCount, true);
        }
    }
//    std::cout << "Partial ID " << m_ID << " finishes processing!" << std::endl;
}

void PartialProcessor::writeToFullImage(std::vector<Vector3i>& fullImage) const {
    auto& info = m_sceneInfo;
    for (int i = 0; i < m_partialWidth; ++i) {
        for (int j = 0; j < m_partialHeight; ++j) {
            fullImage[info.widthRange.first + i + (info.heightRange.first + j) * info.fullSize.first]
                = m_partialImage[i + j * m_partialWidth];
        }
    }
}

void PartialProcessor::writeColor(int partialX, int partialY, Vector3i color) {
    m_partialImage[partialX + partialY * m_partialWidth] = color;
}

void PartialProcessor::writeColor(int partialX, int partialY, Vector3d color, bool gammaCorrection) {
    if (gammaCorrection) {
        color = { sqrt(color.r()), sqrt(color.g()), sqrt(color.b()) }; // Simple gamma2 correction
    }
    color *= 254.999;
    Vector3i rgb = { static_cast<int>(color.r()), static_cast<int>(color.g()), static_cast<int>(color.b()) };
    writeColor(partialX, partialY, rgb);
}

