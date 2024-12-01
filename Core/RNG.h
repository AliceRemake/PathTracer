/**
  ******************************************************************************
  * @file           : Rng.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-30
  ******************************************************************************
  */



#ifndef RNG_H
#define RNG_H

#include <Core/Common.h>

// ThreadSafe Random Number Generator.
struct RNG
{
private:
    THREAD_LOCAL static std::mt19937 rng;

public:
    template<typename T>
    FORCE_INLINE static void Seed(const T& seed) NOEXCEPT
    {
        rng.seed(seed);
    }

    template<typename T>
    NODISCARD FORCE_INLINE static typename T::result_type Rand(T& dist) NOEXCEPT
    {
        return dist(rng);
    }

    NODISCARD FORCE_INLINE static Eigen::Vector3d RandUniformSphere() NOEXCEPT
    {
        auto dist = UniformDist<double>(0.0, 1.0);
        const double cos_theta = Rand(dist) * 2.0 - 1.0; // U[-1.0, 1.0]
        const double phi = Rand(dist) * 2.0 * PI;        // U[0.0, 2.0 * PI]
        const double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
        return Eigen::Vector3d{sin_theta * std::cos(phi), sin_theta * std::sin(phi), cos_theta};
    }

    NODISCARD FORCE_INLINE static Eigen::Vector3d RandUniformHemisphere(const Eigen::Vector3d& normal) NOEXCEPT
    {
        const Eigen::Vector3d zeta = RandUniformSphere();
        return FIsNegative(zeta.dot(normal)) ? -zeta : zeta;
    }

    NODISCARD FORCE_INLINE static Eigen::Vector3d RandLambertianDirection(const Eigen::Vector3d& normal) NOEXCEPT
    {
        return (normal + RandUniformSphere()).normalized();
    }

    template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
    NODISCARD FORCE_INLINE static std::uniform_int_distribution<T> UniformDist(const T begin, const T end) NOEXCEPT
    {
        return std::uniform_int_distribution<T>(begin, end);
    }

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
    NODISCARD FORCE_INLINE static std::uniform_real_distribution<T> UniformDist(const T begin, const T end) NOEXCEPT
    {
        return std::uniform_real_distribution<T>(begin, end);
    }

    // TODO Add More Distribution.
};

#endif //RNG_H
