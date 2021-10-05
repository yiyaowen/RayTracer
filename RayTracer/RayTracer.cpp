/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#include "Camera/Camera.h"
#include "Exporter/PPMManager.h"
#include "Material/Dielectric.h"
#include "Material/Lambertian.h"
#include "Material/Metal.h"
#include "Ray/Ray.h"
#include "Shape/Sphere.h"

#include "RayTracer.h"

std::default_random_engine defaultRandomEngine;

Vector3d rayColor(const Ray& r, const std::vector<std::shared_ptr<Shape>>& shapeList, int depth, bool autoPriority) {
    HitResult hit = {};

    // In case of stack overflow.
    if (depth <= 0) return Vector3d::zero();

    /*
     * Compare depth priority automatically.
     */
    if (autoPriority) {
        HitResult nearHit = {};
        nearHit.t = infinity;
        for (const auto& shape : shapeList) {
            if (shape->hit(r, 0.001, infinity, hit)) { // Do not hit children.
                if (hit.t > 0.0 && hit.t < nearHit.t) {
                    nearHit = hit;
                }
            }
        }
        if (nearHit.t < infinity) {
            Ray rayScattered = {};
            Vector3d attenuation = {};
            if (nearHit.material->scatter(r, nearHit, attenuation, rayScattered)) {
                return attenuation * rayColor(rayScattered, shapeList, depth - 1, autoPriority);
            }
            else {
                return Vector3d::zero();
            }
        }
        else {
            // Default sky background.
            Vector3d unitDirection = normalize(r.direction());
            auto t = 0.5 * (unitDirection.y() + 1.0);
            return (1.0 - t) * Vector3d(1.0, 1.0, 1.0) + t * Vector3d(0.5, 0.7, 1.0);
        }
    }
    /*
     * Use designated priority.
     */
    else {
        for (const auto& shape : shapeList) {
            if (shape->hit(r, 0.001, infinity, hit)) { // Do not hit children.
                Ray rayScattered = {};
                Vector3d attenuation = {};
                if (hit.material->scatter(r, hit, attenuation, rayScattered)) {
                    return attenuation * rayColor(rayScattered, shapeList, depth - 1, autoPriority);
                }
                else {
                    return Vector3d::zero();
                }
            }
        }
        // Default sky background.
        Vector3d unitDirection = normalize(r.direction());
        auto t = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - t) * Vector3d(1.0, 1.0, 1.0) + t * Vector3d(0.5, 0.7, 1.0);
    }
}

int main(int argc, char* argv[]) {
    try {
        // Init random engine.
        defaultRandomEngine = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

        // Image
        const double aspectRatio = 16.0 / 9.0;
        const int imageWidth = 800;
        const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
        const int sampleCount = 100;
        const int maxDepth = 50;

        // Camera
        Camera camera(aspectRatio, 2.0);

        // Scene
        auto materialGround = std::make_shared<Lambertian>(Vector3d(0.8, 0.8, 0.0));
        auto materialCenter = std::make_shared<Lambertian>(Vector3d(0.7, 0.3, 0.3));
        auto materialLeft = std::make_shared<Dielectric>(1.5);
        auto materialRight = std::make_shared<Metal>(Vector3d(0.8, 0.6, 0.2), 1.0);

        auto scene = std::make_shared<Sphere>(Vector3d(0.0, -100.5, -1.0), 100.0, "scene", materialGround);
        auto centerBall = std::make_shared<Sphere>(Vector3d(0.0, 0.0, -1.0), 0.5, "center_ball", materialCenter);
        auto leftBall = std::make_shared<Sphere>(Vector3d(-1.0, 0.0, -1.0), 0.5, "left_ball", materialLeft);
        auto leftInnerBall = std::make_shared<Sphere>(Vector3d(-1.0, 0.0, -1.0), -0.4, "left_inner_ball", materialLeft);
        auto rightBall = std::make_shared<Sphere>(Vector3d(1.0, 0.0, -1.0), 0.5, "right_ball", materialRight);

        bindShapes(scene, centerBall);
        bindShapes(centerBall, leftBall);
        bindShapes(leftBall, leftInnerBall);
        bindShapes(centerBall, rightBall);

        auto shapeList = scene->generatePriorityList({});

        // Render
        PPMManager ppm = {};
        ppm.startWrite(imageWidth, imageHeight);
        for (int j = 0; j < imageHeight; ++j) {
            std::cout << "Scan line " << j+1 << " / " << imageHeight << '\n';
            for (int i = 0; i < imageWidth; ++i) {
                Vector3d color = Vector3d::zero();
                // Sample near points randomly.
                for (int s = 0; s < sampleCount; ++s) {
                    auto u = (double(i) + randomReal()) / static_cast<double>(imageWidth - 1);
                    auto v = (double(j) + randomReal()) / static_cast<double>(imageHeight - 1);
                    auto r = camera.getRay(u, v);
                    color += rayColor(r, shapeList, maxDepth, true);
                }
                // Flip y-axis to make view-coord matches with NDC-coord.
                ppm.writeColor(i, imageHeight - 1 - j, color / sampleCount, true);
            }
        }
        ppm.endWrite("../Z Render Result/test.ppm");
    }
    catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }
}