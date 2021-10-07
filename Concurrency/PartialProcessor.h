/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#ifndef PARTIAL_PROCESSOR_H
#define PARTIAL_PROCESSOR_H

#include <utility>

#include "Camera/Camera.h"
#include "Shape/Shape.h"

#include "RayTracer/RayTracer.h"

struct PartialSceneInfo {
    std::pair<int, int> fullSize = {};
    std::pair<int, int> widthRange = {};
    std::pair<int, int> heightRange = {};
    std::shared_ptr<Camera> camera = nullptr;
    const std::vector<std::shared_ptr<Shape>>& shapeList;
    int maxDepth = 0;
    int sampleCount = 0;

    explicit PartialSceneInfo(const std::vector<std::shared_ptr<Shape>>& shapes) : shapeList(shapes) {}
};

class PartialProcessor {
public:
    PartialProcessor(const PartialSceneInfo& sceneInfo, int ID) : m_sceneInfo(sceneInfo), m_ID(ID) {
        m_partialWidth = sceneInfo.widthRange.second - sceneInfo.widthRange.first + 1;
        m_partialHeight = sceneInfo.heightRange.second - sceneInfo.heightRange.first + 1;
        m_partialImage.resize(m_partialWidth * m_partialHeight);
    }

    void process();

    void writeToFullImage(std::vector<Vector3i>& fullImage) const;

private:
    void writeColor(int partialX, int partialY, Vector3i color);

    void writeColor(int partialX, int partialY, Vector3d color, bool gammaCorrection = true);

private:
    PartialSceneInfo m_sceneInfo;

    int m_ID = 0;

    int m_partialWidth = 0;
    int m_partialHeight = 0;

    std::vector<Vector3i> m_partialImage = {};
};

#endif // PARTIAL_PROCESSOR_H
