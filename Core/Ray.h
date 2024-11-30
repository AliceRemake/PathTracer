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
#include <Core/Scene.h>
#include <Core/Debug.h>

struct Ray
{
    Eigen::Vector3d origin;
    Eigen::Vector3d direction; // Normalized.

    NODISCARD FORCE_INLINE Eigen::Vector3d At(const double t) const NOEXCEPT
    {
        return origin + t * direction;
    }

    NODISCARD static Eigen::Vector4d RayCast(const Ray& ray, const Scene& scene) NOEXCEPT
    {
        Debug::Unuse(scene);
        const double u = std::acos(ray.direction.y()) / PI;
        return (1.0 - u) * Eigen::Vector4d{0.5, 0.7, 1.0, 1.0} + u * Eigen::Vector4d{1.0, 1.0, 1.0, 1.0};
    }
};

#endif //RAY_H
