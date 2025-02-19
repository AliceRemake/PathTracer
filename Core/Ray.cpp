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
#include <Core/RNG.h>
#include <Core/Interval.h>
#include <Core/Material.h>


NODISCARD Eigen::Vector3d Ray::RayCast(const Ray& ray, const Ref<Hittable>& hittable, const Eigen::Index bounce) NOEXCEPT // NOLINT(*-no-recursion)
{
    HitRecord record;

    // Background.
    if (!hittable->Hit(ray, Interval{ 10 * EPS, INF }, record))
    {
        // TODO: Skybox.
        return Eigen::Vector3d{0.0, 0.0, 0.0};
        // const double u = 0.5 * (ray.direction.y() + 1.0);
        // return (1.0 - u) * Eigen::Vector3d{1.0, 1.0, 1.0} + u * Eigen::Vector3d{0.5, 0.7, 1.0};
    }

    if (bounce <= 64)
    {
        // In General, Scatter The Ray, Trace The Scattered Ray, Shade The Color.
        const Ray scattered_ray = record.material->Scatter(ray, record);
        const Eigen::Vector3d color = RayCast(scattered_ray, hittable, bounce + 1);
        return record.material->Radiance(ray, record, color) + record.material->Emission(record.texcoord2d);
    }

    return {0.0, 0.0, 0.0};
    // else // Use Russian Roulette.
    // {
    //
    //     if (auto dist = RNG::UniformDist<double>(0, 1); RNG::Rand(dist) < stop_prob)
    //     {
    //         return Eigen::Vector3d{0.0, 0.0, 0.0};
    //     }
    //     else
    //     {
    //         const Ray scattered_ray = record.material->Scatter(ray, record);
    //         const Eigen::Vector3d color = RayCast(scattered_ray, hittable, bounce + 1);
    //         return record.material->Shading(ray, record, (1 - stop_prob) * color) + record.material->Emitted(record.texcoord2d);
    //     }
    // }
}
