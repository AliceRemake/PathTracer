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

NODISCARD Eigen::Vector3d LambertMaterial::BRDF(const Eigen::Vector2d& texcoord2d, const Eigen::Vector3d& normal UNUSED, const Eigen::Vector3d& omega_i UNUSED, const Eigen::Vector3d& omega_o UNUSED) const NOEXCEPT
{
    return albedo_tex->Sample(texcoord2d) / PI;
}

NODISCARD Eigen::Vector3d LambertMaterial::Emission(const Eigen::Vector2d& texcoord2d, const Eigen::Vector3d& normal UNUSED, const Eigen::Vector3d& omega_o UNUSED) const NOEXCEPT
{
    return emission_tex->Sample(texcoord2d);
}

NODISCARD Eigen::Vector3d BlinnPhongMaterial::BRDF(const Eigen::Vector2d& texcoord2d, const Eigen::Vector3d& normal, const Eigen::Vector3d& omega_i, const Eigen::Vector3d& omega_o) const NOEXCEPT
{
    const Eigen::Vector3d diffuse_term = diffuse_tex->Sample(texcoord2d) / PI;
    const Eigen::Vector3d half = (omega_i + omega_o).normalized();
    const Eigen::Vector3d specular_term = specular_tex->Sample(texcoord2d) * ((ns + 2.0) / (2.0 * PI)) * std::pow(std::max(normal.dot(half), 0.0), ns);
    return diffuse_term + specular_term;
}

NODISCARD Eigen::Vector3d BlinnPhongMaterial::Emission(const Eigen::Vector2d& texcoord2d, const Eigen::Vector3d& normal UNUSED, const Eigen::Vector3d& omega_o UNUSED) const NOEXCEPT
{
    return emission_tex->Sample(texcoord2d);
}
