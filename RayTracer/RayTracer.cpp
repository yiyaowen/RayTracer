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

std::vector<std::shared_ptr<Shape>> randomScene();

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
        Vector3d position = { 13.0, 2.0, 3.0 };
        Vector3d lookAt = { 0.0, 0.0, 0.0 };
        Vector3d up = { 0.0, 1.0, 0.0 };
        Camera camera(aspectRatio, 0.1, 10.0, 20.0, position, lookAt, up);

        // Scene

        auto shapeList = randomScene();

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

std::vector<std::shared_ptr<Shape>> randomScene() {
    std::vector<std::shared_ptr<Shape>> shapeList = {};

    auto groundMaterial = std::make_shared<Lambertian>(Vector3d(0.5, 0.5, 0.5));
    shapeList.push_back(std::make_shared<Sphere>(Vector3d(0.0, -1000.0, 0.0), 1000.0, "scene", groundMaterial));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11;  b < 11; ++b) {
            auto choose = randomReal();
            Vector3d center(a + 0.9 * randomReal(), 0.2, b + 0.9 * randomReal());

            if ((center - Vector3d(4.0, 0.2, 0.0)).length() > 0.9) {
                std::shared_ptr<Material> sphereMaterial;

                if (choose < 0.8) {
                    // Diffuse
                    auto albedo = randomVec3d() * randomVec3d();
                    sphereMaterial = std::make_shared<Lambertian>(albedo);
                    shapeList.push_back(std::make_shared<Sphere>(center, 0.2, "diffuse_ball", sphereMaterial));
                }
                else if (choose < 0.95) {
                    // Metal
                    auto albedo = randomVec3d(0.5, 1.0);
                    auto fuzz = randomReal(0.0, 0.5);
                    sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
                    shapeList.push_back(std::make_shared<Sphere>(center, 0.2, "metal_ball", sphereMaterial));
                }
                else {
                    // Glass
                    sphereMaterial = std::make_shared<Dielectric>(1.5);
                    shapeList.push_back(std::make_shared<Sphere>(center, 0.2, "glass_ball", sphereMaterial));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    shapeList.push_back(std::make_shared<Sphere>(Vector3d(0.0, 1.0, 0.0), 1.0, "ball1", material1));

    auto material2 = std::make_shared<Lambertian>(Vector3d(0.4, 0.2, 0.1));
    shapeList.push_back(std::make_shared<Sphere>(Vector3d(-4.0, 1.0, 0.0), 1.0, "ball2", material2));

    auto material3 = std::make_shared<Metal>(Vector3d(0.7, 0.6, 0.5), 0.0);
    shapeList.push_back(std::make_shared<Sphere>(Vector3d(4.0, 1.0, 0.0), 1.0, "ball3", material3));

    return shapeList;
}