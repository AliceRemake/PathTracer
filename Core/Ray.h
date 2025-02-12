/**
  ******************************************************************************
  * @file           : Ray.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-27
  ******************************************************************************
  */



#ifndef RAY_H
#define RAY_H

#include <Core/Common.h>

struct Hittable;

struct Ray
{
    Eigen::Vector3d origin;
    Eigen::Vector3d direction;

    NODISCARD Eigen::Vector3d At(const double t) const NOEXCEPT { return origin + t * direction; }

    NODISCARD static Eigen::Vector3d RayCast(const Ray& ray, const Ref<Hittable>& hittable, Eigen::Index bounce, double stop_prob) NOEXCEPT;
};

#endif //RAY_H
