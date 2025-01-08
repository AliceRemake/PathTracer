/**
  ******************************************************************************
  * @file           : Material.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-27
  ******************************************************************************
  */



#include <Core/Material.h>
#include <Core/RNG.h>
#include <Core/Ray.h>

NODISCARD Ray DiffuseUniformMaterial::Scatter(const Ray &ray UNUSED, const HitRecord &record) const NOEXCEPT
{
    return Ray
    {
        .origin = record.hit_point,
        .direction = RNG::RandUniformHemisphere(record.hit_normal),
    };
}

NODISCARD Eigen::Vector3d DiffuseUniformMaterial::Shading(const Ray& ray UNUSED, const HitRecord& record UNUSED, const Eigen::Vector3d& color) const NOEXCEPT
{
    return reflection.array() * color.array();
}

NODISCARD Ray DiffuseLambertMaterial::Scatter(const Ray &ray UNUSED, const HitRecord &record) const NOEXCEPT
{
    return Ray
    {
        .origin = record.hit_point,
        .direction = RNG::RandLambertianDirection(record.hit_normal),
    };
}

NODISCARD Eigen::Vector3d DiffuseLambertMaterial::Shading(const Ray& ray UNUSED, const HitRecord& record UNUSED, const Eigen::Vector3d& color) const NOEXCEPT
{
    return reflection.array() * color.array();
}

NODISCARD Ray MetalMaterial::Scatter(const Ray &ray, const HitRecord &record) const NOEXCEPT
{
    return Ray
    {
        .origin = record.hit_point,
        .direction = (ray.direction - 2.0 * ray.direction.dot(record.hit_normal) * record.hit_normal + fuzziness * RNG::RandUniformSphere()).normalized(),
    };
}

NODISCARD Eigen::Vector3d MetalMaterial::Shading(const Ray& ray UNUSED, const HitRecord& record UNUSED, const Eigen::Vector3d& color) const NOEXCEPT
{
    return reflection.array() * color.array();
}

NODISCARD Ray DielectricMaterial::Scatter(const Ray &ray, const HitRecord &record) const NOEXCEPT
{
    const double cos_theta = std::clamp(-ray.direction.dot(record.hit_normal), -1.0, 1.0);
    const double sin_theta = SSqrt(1.0 - cos_theta * cos_theta);
    const double rior = FIsPositive(cos_theta) ? 1.0 / ior : ior;

    if (rior * sin_theta > 1.0) // Total Internal Reflection. TIR.
    {
        return Ray
        {
            .origin = record.hit_point,
            .direction = Reflect(-ray.direction, record.hit_normal),
        };
    }

    if (auto dist = RNG::UniformDist(0.0, 1.0); RNG::Rand(dist) < SchlickReflectance(cos_theta, rior))
    {
        return Ray
        {
            .origin = record.hit_point,
            .direction = Reflect(-ray.direction, record.hit_normal),
        };
    }

    return Ray
    {
        .origin = record.hit_point,
        .direction = Refract(cos_theta, -ray.direction, record.hit_normal, rior),
    };
}

NODISCARD Eigen::Vector3d DielectricMaterial::Shading(const Ray& ray UNUSED, const HitRecord& record UNUSED, const Eigen::Vector3d& color) const NOEXCEPT
{
    return color.array();
}
