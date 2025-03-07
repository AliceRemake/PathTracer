/**
  ******************************************************************************
  * @file           : Sampler.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 25-3-5
  ******************************************************************************
  */



#ifndef SAMPLER_H
#define SAMPLER_H

#include <Core/RNG.h>
#include <Core/ONB.h>
#include <Core/Hittable.h>

struct Sampler
{
    virtual ~Sampler() NOEXCEPT = default;

    NODISCARD virtual Eigen::Vector3d /* omega_i */ Sample(
        const Eigen::Vector3d& origin,
        const Eigen::Vector2d& texcoord,
        const Eigen::Vector3d& normal,
        const Eigen::Vector3d& omega_o
    ) const NOEXCEPT = 0;

    NODISCARD virtual double PDF(
        const Eigen::Vector3d& origin,
        const Eigen::Vector2d& texcoord,
        const Eigen::Vector3d& normal,
        const Eigen::Vector3d& omega_i,
        const Eigen::Vector3d& omega_o
    ) const NOEXCEPT = 0;
};

struct MixSampler final : Sampler
{
    std::vector<Ref<Sampler>> samplers;
    std::vector<double> cdf;

    NODISCARD MixSampler(std::vector<Ref<Sampler>> samplers, const std::vector<double>& weights) NOEXCEPT
    : samplers(std::move(samplers)), cdf(weights.size() + 1)
    {
        cdf[0] = 0.0;
        for (size_t i = 1; i <= weights.size(); ++i)
        {
            cdf[i] = cdf[i - 1] + weights[i - 1];
        }
        for (size_t i = 1; i <= weights.size(); ++i)
        {
            cdf[i] /= cdf.back();
        }
    }

    NODISCARD Eigen::Vector3d /* omega_i */ Sample(
        const Eigen::Vector3d& origin,
        const Eigen::Vector2d& texcoord,
        const Eigen::Vector3d& normal,
        const Eigen::Vector3d& omega_o
    ) const NOEXCEPT OVERRIDE
    {
        auto dist = RNG::UniformDist<double>(0.0, 1.0);
        const double rng = RNG::Rand(dist);
        for (size_t i = 1; i <= samplers.size(); ++i)
        {
            if (Fle(rng, cdf[i]))
            {
                return samplers[i - 1]->Sample(origin, texcoord, normal, omega_o);
            }
        }
        return samplers.back()->Sample(origin, texcoord, normal, omega_o);
    }

    NODISCARD double PDF(
        const Eigen::Vector3d& origin,
        const Eigen::Vector2d& texcoord,
        const Eigen::Vector3d& normal,
        const Eigen::Vector3d& omega_i,
        const Eigen::Vector3d& omega_o
    ) const NOEXCEPT OVERRIDE
    {
        double pdf = 0.0;
        for (size_t i = 1; i <= samplers.size(); ++i)
        {
            pdf += (cdf[i] - cdf[i - 1]) * samplers[i - 1]->PDF(origin, texcoord, normal, omega_i, omega_o);
        }
        return pdf;
    }
};

struct UniformHemiSphereSampler final : Sampler
{
    NODISCARD Eigen::Vector3d /* omega_i */ Sample(
        const Eigen::Vector3d& origin UNUSED,
        const Eigen::Vector2d& texcoord UNUSED,
        const Eigen::Vector3d& normal,
        const Eigen::Vector3d& omega_o UNUSED
    ) const NOEXCEPT OVERRIDE
    {
        auto dist = RNG::UniformDist<double>(0.0, 1.0);
        const double zeta_1 = RNG::Rand(dist);
        const double zeta_2 = RNG::Rand(dist);
        const double cos_theta = zeta_1 * 2.0 - 1.0;
        const double sin_theta = SSqrt(1.0 - cos_theta * cos_theta);
        const double phi = zeta_2 * 2.0 * PI;
        Eigen::Vector3d omega_i = { cos_theta, sin_theta * std::cos(phi), sin_theta * std::sin(phi) };
        return FIsNegative(omega_i.dot(normal)) ? -omega_i : omega_i;
    }

    NODISCARD double PDF(
        const Eigen::Vector3d& origin UNUSED,
        const Eigen::Vector2d& texcoord UNUSED,
        const Eigen::Vector3d& normal UNUSED,
        const Eigen::Vector3d& omega_i UNUSED,
        const Eigen::Vector3d& omega_o UNUSED
    ) const NOEXCEPT OVERRIDE
    {
        return 1.0 / (2.0 * PI);
    }
};

struct CosineHemiSphereSampler final : Sampler
{
    NODISCARD Eigen::Vector3d /* omega_i */ Sample(
        const Eigen::Vector3d& origin UNUSED,
        const Eigen::Vector2d& texcoord UNUSED,
        const Eigen::Vector3d& normal,
        const Eigen::Vector3d& omega_o UNUSED
    ) const NOEXCEPT OVERRIDE
    {
        auto dist = RNG::UniformDist<double>(0.0, 1.0);
        const double zeta_1 = RNG::Rand(dist);
        const double zeta_2 = RNG::Rand(dist);
        const double phi = 2.0 * PI * zeta_2;
        const double cos_theta = SSqrt(zeta_1);
        const double sin_theta = SSqrt(1.0 - zeta_1);
        return ONB(normal).Transform(Eigen::Vector3d{ cos_theta, sin_theta * std::cos(phi), sin_theta * std::sin(phi) });
    }

    NODISCARD double PDF(
        const Eigen::Vector3d& origin UNUSED,
        const Eigen::Vector2d& texcoord UNUSED,
        const Eigen::Vector3d& normal,
        const Eigen::Vector3d& omega_i,
        const Eigen::Vector3d& omega_o UNUSED
    ) const NOEXCEPT OVERRIDE
    {
        return normal.dot(omega_i) / PI;
    }
};

struct BlinnPhongSpecularSampler final : Sampler
{
    double ns;

    NODISCARD explicit BlinnPhongSpecularSampler(const double ns) NOEXCEPT : ns(ns) {}

    NODISCARD Eigen::Vector3d /* omega_i */ Sample(
        const Eigen::Vector3d& origin UNUSED,
        const Eigen::Vector2d& texcoord UNUSED,
        const Eigen::Vector3d& normal,
        const Eigen::Vector3d& omega_o
    ) const NOEXCEPT OVERRIDE
    {
        auto dist = RNG::UniformDist<double>(0.0, 1.0);
        const double zeta_1 = RNG::Rand(dist);
        const double zeta_2 = RNG::Rand(dist);
        const double phi = 2.0 * PI * zeta_2;
        const double cos_theta = std::pow(zeta_1, 1.0 / (ns + 1.0));
        const double sin_theta = SSqrt(1.0 - cos_theta * cos_theta);
        const Eigen::Vector3d half = ONB(normal).Transform(Eigen::Vector3d{ cos_theta, sin_theta * std::cos(phi), sin_theta * std::sin(phi) });
        return 2.0 * omega_o.dot(half) * half - omega_o;
    }

    NODISCARD double PDF(
        const Eigen::Vector3d& origin UNUSED,
        const Eigen::Vector2d& texcoord UNUSED,
        const Eigen::Vector3d& normal,
        const Eigen::Vector3d& omega_i,
        const Eigen::Vector3d& omega_o
    ) const NOEXCEPT OVERRIDE
    {
        const Eigen::Vector3d half = (omega_i + omega_o).normalized();
        return ((ns + 1.0) * std::pow(half.dot(normal), ns)) / (8.0 * PI * omega_o.dot(half));
    }
};

// struct LightImportanceSampler final : Sampler
// {
//     const std::vector<Ref<Hittable>>& lights;
//
//     NODISCARD explicit LightImportanceSampler(const std::vector<Ref<Hittable>>& lights) NOEXCEPT : lights(lights) {}
//
//     NODISCARD Eigen::Vector3d /* omega_i */ Sample(
//         const Eigen::Vector3d& origin UNUSED,
//         const Eigen::Vector2d& texcoord UNUSED,
//         const Eigen::Vector3d& normal,
//         const Eigen::Vector3d& omega_o UNUSED
//     ) const NOEXCEPT OVERRIDE
//     {
//         Eigen::Vector3d omega_i = {};
//         double pdf = 0.0;
//
//         auto dist1 = RNG::UniformDist<double>(213.0, 343.0);
//         auto dist2 = RNG::UniformDist<double>(227.0, 332.0);
//         auto on_light = Eigen::Vector3d(RNG::Rand(dist1), 554, RNG::Rand(dist2));
//         Eigen::Vector3d to_light = on_light - origin;
//         auto distance_squared = to_light.squaredNorm();
//         to_light = to_light.normalized();
//
//         double light_area = (343-213)*(332-227);
//         auto light_cosine = std::fabs(to_light.y());
//
//         pdf = distance_squared / (light_cosine * light_area);
//         omega_i = to_light;
//
//         ASSERT(!FIsInfinity(pdf));
//
//         return omega_i;
//     }
//
//     NODISCARD double PDF(
//         const Eigen::Vector3d& origin UNUSED,
//         const Eigen::Vector2d& texcoord UNUSED,
//         const Eigen::Vector3d& normal,
//         const Eigen::Vector3d& omega_i,
//         const Eigen::Vector3d& omega_o UNUSED
//     ) const NOEXCEPT OVERRIDE
//     {
//         auto distance_squared = to_light.squaredNorm();
//         return distance_squared / (light_cosine * light_area);
//     }
// };

#endif //SAMPLER_H
