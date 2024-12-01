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

NODISCARD Ray BasicMaterial::Scatter(const Ray &ray UNUSED, const HitRecord &record) const NOEXCEPT
{
    return Ray
    {
        .origin = record.hit_point,
        .direction = RNG::RandUniformHemisphere(record.hit_normal),
    };
}

NODISCARD Eigen::Vector3d BasicMaterial::Shading(const Ray& ray UNUSED, const HitRecord& record UNUSED, const Eigen::Vector3d& color) const NOEXCEPT
{
    return diffuse.array() * color.array();
}

NODISCARD Ray LambertMaterial::Scatter(const Ray &ray UNUSED, const HitRecord &record) const NOEXCEPT
{
    return Ray
    {
        .origin = record.hit_point,
        .direction = RNG::RandLambertianDirection(record.hit_normal),
    };
}

NODISCARD Eigen::Vector3d LambertMaterial::Shading(const Ray& ray UNUSED, const HitRecord& record UNUSED, const Eigen::Vector3d& color) const NOEXCEPT
{
    return diffuse.array() * color.array();
}
