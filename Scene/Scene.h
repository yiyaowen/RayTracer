/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#ifndef SCENE_H
#define SCENE_H

#include "Camera/Camera.h"
#include "Shape/Shape.h"

std::shared_ptr<Camera> testCamera(double aspectRatio);
std::vector<std::shared_ptr<Shape>> testScene();

std::shared_ptr<Camera> randomBallsCamera(double aspectRatio);
std::vector<std::shared_ptr<Shape>> randomBallsScene();


#endif // SCENE_H