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

NODISCARD Ray DiffuseMaterial::Scatter(const Ray &ray UNUSED, HitRecord &record) const NOEXCEPT
{
    record.scatter_type = HitRecord::SCATTER_TYPE_REFLECT;
    return Ray
    {
        .origin = record.hit_point,
        .direction = RNG::RandUniformHemisphere(record.hit_normal),
    };
}

NODISCARD Eigen::Vector3d DiffuseMaterial::Radiance(const Ray& ray UNUSED, const HitRecord& record UNUSED, const Eigen::Vector3d& color) const NOEXCEPT
{
    if (color_tex == nullptr)
    {
        exit(EXIT_FAILURE);
    }
    else
    {
        return color_tex->Sample(record.texcoord2d).array() * color.array();
    }
}

NODISCARD Ray LambertMaterial::Scatter(const Ray &ray UNUSED, HitRecord &record) const NOEXCEPT
{
    record.scatter_type = HitRecord::SCATTER_TYPE_REFLECT;
    return Ray
    {
        .origin = record.hit_point,
        .direction = RNG::RandLambertianDirection(record.hit_normal),
    };
}

NODISCARD Eigen::Vector3d LambertMaterial::Radiance(const Ray& ray UNUSED, const HitRecord& record UNUSED, const Eigen::Vector3d& color) const NOEXCEPT
{
    if (color_tex == nullptr)
    {
        exit(EXIT_FAILURE);
    }
    else
    {
        return color_tex->Sample(record.texcoord2d).array() * color.array();
    }
}

NODISCARD Ray MetalMaterial::Scatter(const Ray &ray, HitRecord &record) const NOEXCEPT
{
    record.scatter_type = HitRecord::SCATTER_TYPE_REFLECT;
    return Ray
    {
        .origin = record.hit_point,
        .direction = (ray.direction - 2.0 * ray.direction.dot(record.hit_normal) * record.hit_normal + fuzziness * RNG::RandUniformSphere()).normalized(),
    };
}

NODISCARD Eigen::Vector3d MetalMaterial::Radiance(const Ray& ray UNUSED, const HitRecord& record UNUSED, const Eigen::Vector3d& color) const NOEXCEPT
{
    if (color_tex == nullptr)
    {
        exit(EXIT_FAILURE);
    }
    else
    {
        return color_tex->Sample(record.texcoord2d).array() * color.array();
    }
}

NODISCARD Ray DielectricMaterial::Scatter(const Ray &ray, HitRecord &record) const NOEXCEPT
{
    const double cos_theta = std::clamp(-ray.direction.dot(record.hit_normal), -1.0, 1.0);
    const double sin_theta = SSqrt(1.0 - cos_theta * cos_theta);
    const double rior = FIsPositive(cos_theta) ? 1.0 / ior : ior;

    if (rior * sin_theta > 1.0) // Total Internal Reflection. TIR.
    {
        record.scatter_type = HitRecord::SCATTER_TYPE_REFLECT;
        return Ray
        {
            .origin = record.hit_point,
            .direction = Reflect(-ray.direction, record.hit_normal),
        };
    }

    if (auto dist = RNG::UniformDist<double>(0, 1); RNG::Rand(dist) < SchlickReflectance(cos_theta, rior))
    {
        record.scatter_type = HitRecord::SCATTER_TYPE_REFLECT;
        return Ray
        {
            .origin = record.hit_point,
            .direction = Reflect(-ray.direction, record.hit_normal),
        };
    }
    else
    {
        record.scatter_type = HitRecord::SCATTER_TYPE_REFRACT;
        return Ray
        {
            .origin = record.hit_point,
            .direction = Refract(cos_theta, -ray.direction, record.hit_normal, rior),
        };
    }
}

NODISCARD Eigen::Vector3d DielectricMaterial::Radiance(const Ray& ray UNUSED, const HitRecord& record UNUSED, const Eigen::Vector3d& color) const NOEXCEPT
{
    return color.array();
    // if (record.scatter_type == HitRecord::SCATTER_TYPE_REFLECT)
    // {
    //     // return color_tex->Sample(record.texcoord2d).array() * color.array();
    //     return color.array();
    // }
    // else // if (record.scatter_type == HitRecord::SCATTER_TYPE_REFRACT)
    // {
    //     // return (1.0 - color_tex->Sample(record.texcoord2d).array()) * color.array();
    //     return color.array();
    // }
}

NODISCARD Ray EmissiveMaterial::Scatter(const Ray &ray UNUSED, HitRecord &record UNUSED) const NOEXCEPT
{
    return ref->Scatter(ray, record);
}

NODISCARD Eigen::Vector3d EmissiveMaterial::Radiance(const Ray& ray UNUSED, const HitRecord& record UNUSED, const Eigen::Vector3d& color UNUSED) const NOEXCEPT
{
    return ref->Radiance(ray, record, color);
}
