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

struct Hittable;
struct Camera;
struct Image;

struct RenderConfig
{
    const size_t SPP;
    const double stop_prob; // Continue Probability.
};

struct Renderer
{
    static void Render(const Camera& camera, const Ref<Hittable>& hittable, const RenderConfig& config, Image& film) NOEXCEPT;
};

#endif //RENDERER_H
