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
#include <Core/Material.h>

NODISCARD Eigen::Vector3d Ray::RayCast(const Ray& ray, const Scene& scene, const size_t bounces) NOEXCEPT
{
    HitRecord record;

    if (!scene.Hit(ray, Interval{ EPS, INF }, record))
    {
        // Background Color.
        const double u = 0.5 * (ray.direction.y() + 1.0);
        return (1.0 - u) * Eigen::Vector3d{1.0, 1.0, 1.0} + u * Eigen::Vector3d{0.5, 0.7, 1.0};
    }

    // This Visualize The Normals.
    // Eigen::Vector3d RGB = 0.5 * (record.hit_normal + Eigen::Vector3d{1.0, 1.0, 1.0});
    // return Eigen::Vector4d{RGB.x(), RGB.y(), RGB.z(), 1.0};

    // This Use Uniform Hemisphere Distribution.
    // return 0.5 * RayCast(Ray{
    //     .origin = record.hit_point,
    //     .direction = RNG::RandUniformHemisphere(record.hit_normal),
    // }, scene, bounces - 1);

    // This Use Lambertian Distribution.
    // return 0.5 * RayCast(Ray{
    //     .origin = record.hit_point,
    //     .direction = RNG::RandLambertianDirection(record.hit_normal),
    // }, scene, bounces - 1);

    // No More Bounce. We Can Not See.
    if (bounces == 0)
    {
        return Eigen::Vector3d{0.0, 0.0, 0.0};
    }

    // In General, Scatter The Ray, Trace The Scattered Ray, Shade The Color.
    const Ray scattered_ray = record.material->Scatter(ray, record);
    const Eigen::Vector3d color = RayCast(scattered_ray, scene, bounces - 1);
    return record.material->Shading(ray, record, color);
}
