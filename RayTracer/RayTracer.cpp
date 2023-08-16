/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#include <functional>
#include <future>
#include <thread>

#include "Exporter/ExporterManager.h"
#include "Concurrency/PartialProcessor.h"
#include "Scene/Scene.h"

#include "RayTracer.h"

std::default_random_engine defaultRandomEngine;

int main(int argc, char* argv[]) {
    try {
        // Init random engine.
        defaultRandomEngine = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

        // Image
        const double aspectRatio = 16.0 / 9.0;
        const int imageWidth = 800;
        const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
        const int maxDepth = 50;
        const int sampleCount = 100;

        /* Build scene start */
        auto buildSceneStart = std::chrono::high_resolution_clock::now();
        // Camera
        auto camera = randomBallsCamera(aspectRatio);

        // Scene
        auto shapeList = randomBallsScene();
        /* Build scene end */
        auto buildSceneEnd = std::chrono::high_resolution_clock::now();
        auto buildSceneCost = buildSceneEnd - buildSceneStart;
        auto buildSceneCostMs = std::chrono::duration_cast<std::chrono::milliseconds>(buildSceneCost).count();
        auto buildSceneCostUs = std::chrono::duration_cast<std::chrono::microseconds>(buildSceneCost).count();

        /* Render scene start */
        auto renderSceneStart = std::chrono::high_resolution_clock::now();
        // Render
        ExporterManager em = {};
        em.startWrite(imageWidth, imageHeight);

        // Split the image to take advantage of multithreading to accelerate rendering.
        PartialSceneInfo sceneInfo(shapeList);
        sceneInfo.fullSize = { imageWidth, imageHeight };
        sceneInfo.camera = camera;
        sceneInfo.maxDepth = maxDepth;
        sceneInfo.sampleCount = sampleCount;

        int dispatchCountX = 16;
        int dispatchCountY = 16;

        int partialWidth = imageWidth / dispatchCountX;
        int partialHeight = imageHeight / dispatchCountY;

        std::vector<PartialProcessor> partials;
        partials.reserve((dispatchCountX + 1) * (dispatchCountY + 1));

        std::vector<std::future<void>> tasks;
        tasks.reserve((dispatchCountX + 1) * (dispatchCountY + 1));

        std::cout << "Building partial processors..." << std::endl;
        int partialID = 0;
        for (int i = 0; i <= dispatchCountX; ++i) {
            for (int j = 0; j <= dispatchCountY; ++j) {
                sceneInfo.widthRange = {partialWidth * i, partialWidth * (i + 1) - 1 };
                if (sceneInfo.widthRange.second >= imageWidth) {
                    if (sceneInfo.widthRange.first >= imageWidth) continue;
                    sceneInfo.widthRange.second = imageWidth - 1;
                }
                sceneInfo.heightRange = {partialHeight * j, partialHeight * (j + 1) - 1 };
                if (sceneInfo.heightRange.second >= imageHeight) {
                    if (sceneInfo.heightRange.first >= imageHeight) continue;
                    sceneInfo.heightRange.second = imageHeight - 1;
                }
                partials.emplace_back(sceneInfo, partialID);
                tasks.emplace_back(std::async([&,partialID] { partials[partialID].process(); }));
                ++partialID;
            }
        }

        size_t totalCount = tasks.size();
        size_t lastFinished = 0, currFinished = 0;
        std::function<size_t()> finishedCount = [&]() -> size_t {
            return std::count_if(tasks.begin(), tasks.end(), [&](const std::future<void>& task) {
                return task.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
            });
        };
        while ((currFinished = finishedCount()) != totalCount) {
            if (currFinished > lastFinished) {
                std::cout << "Finished partial count " << currFinished << " / " << totalCount << std::endl;
                lastFinished = currFinished;
            }
            std::this_thread::yield();
        }


        std::cout << "Writing partial images to final full image...\n";
        for (const auto& partial : partials) {
            partial.writeToFullImage(em.buffer());
        }

        std::cout << "Generating render result...\n";
        em.endWrite("render_result.png", ExporterManager::PNG);
        /* Render scene end */
        auto renderSceneEnd = std::chrono::high_resolution_clock::now();
        auto renderSceneCost = renderSceneEnd - renderSceneStart;
        auto renderSceneCostMin = std::chrono::duration_cast<std::chrono::minutes>(renderSceneCost).count();
        auto renderSceneCostSec = std::chrono::duration_cast<std::chrono::seconds>(renderSceneCost).count();

        std::cout << "Build finished in " << buildSceneCostMs << " ms, " << buildSceneCostUs << " us. " <<
            "Render finished in " << renderSceneCostMin << " min, " << renderSceneCostSec << " sec.\n";
    }
    catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }
}