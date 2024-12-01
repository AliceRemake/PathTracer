/**
  ******************************************************************************
  * @file           : Ray.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-27
  ******************************************************************************
  */



#include <Core/Ray.h>
#include <Core/Interval.h>
#include <Core/Scene.h>

NODISCARD Eigen::Vector4d Ray::RayCast(const Ray& ray, const Scene& scene) NOEXCEPT
{
    HitRecord record;

    if (!scene.Hit(ray, Interval{ EPS, INF }, record))
    {
        // Background Color.
        const double u = std::acos(ray.direction.y()) / PI;
        return (1.0 - u) * Eigen::Vector4d{0.3, 0.5, 1.0, 1.0} + u * Eigen::Vector4d{1.0, 1.0, 1.0, 1.0};
    }

    Eigen::Vector3d RGB = 0.5 * (record.hit_normal + Eigen::Vector3d{1.0, 1.0, 1.0});
    return Eigen::Vector4d{RGB.x(), RGB.y(), RGB.z(), 1.0};
}
