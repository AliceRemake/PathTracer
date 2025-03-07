/**
  ******************************************************************************
  * @file           : Renderer.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-12-1
  ******************************************************************************
  */



#ifndef RENDERER_H
#define RENDERER_H

#include <Core/Common.h>

struct Ray;
struct Hittable;
struct Camera;
struct Image;

struct RenderConfig
{
    const Eigen::Index SPP;
    const double stop_prob;
};

struct Renderer
{
    NODISCARD static Eigen::Vector3d RayCast(const Ray& ray, const Ref<Hittable>& hittable, const std::vector<Ref<Hittable>>& lights, Eigen::Index bounce, double stop_prob) NOEXCEPT;

    static void Render(const Camera& camera, const Ref<Hittable>& scene, const std::vector<Ref<Hittable>>& lights, const RenderConfig& config, Image& film) NOEXCEPT;
};

#endif //RENDERER_H
