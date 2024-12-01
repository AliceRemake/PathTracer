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

struct Scene;

struct Ray
{
    Eigen::Vector3d origin;
    Eigen::Vector3d direction; // Normalized.

    NODISCARD FORCE_INLINE Eigen::Vector3d At(const double t) const NOEXCEPT
    {
        return origin + t * direction;
    }

    NODISCARD static Eigen::Vector4d RayCast(const Ray& ray, const Scene& scene) NOEXCEPT;
};

#endif //RAY_H
