/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#include "Material/Dielectric.h"
#include "Material/Lambertian.h"
#include "Material/Metal.h"
#include "Scene.h"
#include "Shape/Sphere.h"

std::shared_ptr<Camera> testCamera(double aspectRatio) {
    Vector3d position = { 0.0, 0.0, 0.0 };
    Vector3d lookAt = { 0.0, 0.0, -1.0 };
    Vector3d up = { 0.0, 1.0, 0.0 };
    return std::make_shared<Camera>(aspectRatio, 0.0, 1.0, 90.0, position, lookAt, up);
}

std::vector<std::shared_ptr<Shape>> testScene() {
    std::vector<std::shared_ptr<Shape>> shapeList = {};

    auto groundMaterial = std::make_shared<Lambertian>(Vector3d(0.5, 0.5, 0.5));
    shapeList.push_back(std::make_shared<Sphere>(Vector3d(0.0, -100.5, -1.0), 100.0, "scene", groundMaterial));

//    auto ballMaterial = std::make_shared<Lambertian>(Vector3d(0.5, 0.5, 0.5));
//    auto ballMaterial = std::make_shared<Metal>(Vector3d(0.8, 0.8, 0.9), 0.0);
    auto ballMaterial = std::make_shared<Dielectric>(Vector3d(0.8, 0.8, 0.9), 1.5);

    shapeList.push_back(std::make_shared<Sphere>(Vector3d(0.0, 0.0, -1.0), 0.5, "ball", ballMaterial));

    return shapeList;
}

std::shared_ptr<Camera> randomBallsCamera(double aspectRatio) {
    Vector3d position = { 13.0, 2.0, 3.0 };
    Vector3d lookAt = { 0.0, 0.0, 0.0 };
    Vector3d up = { 0.0, 1.0, 0.0 };
    return std::make_shared<Camera>(aspectRatio, 0.1, 10.0, 20.0, position, lookAt, up);
}

std::vector<std::shared_ptr<Shape>> randomBallsScene() {
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
                    sphereMaterial = std::make_shared<Dielectric>(Vector3d(0.9, 0.9, 0.95), 1.5);
                    shapeList.push_back(std::make_shared<Sphere>(center, 0.2, "glass_ball", sphereMaterial));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(Vector3d(0.95, 0.95, 1.0), 1.5);
    shapeList.push_back(std::make_shared<Sphere>(Vector3d(0.0, 1.0, 0.0), 1.0, "ball1", material1));

    auto material2 = std::make_shared<Lambertian>(Vector3d(0.4, 0.2, 0.1));
    shapeList.push_back(std::make_shared<Sphere>(Vector3d(-4.0, 1.0, 0.0), 1.0, "ball2", material2));

    auto material3 = std::make_shared<Metal>(Vector3d(0.7, 0.6, 0.5), 0.0);
    shapeList.push_back(std::make_shared<Sphere>(Vector3d(4.0, 1.0, 0.0), 1.0, "ball3", material3));

    return shapeList;
}