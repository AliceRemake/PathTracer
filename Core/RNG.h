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

/// Usage:
///     RNG::Seed<int>(0);                        // Seed The Generator.
///     auto dist = RNG::UniformDist<int>(1, 10); // Choose A Distribution.
///     RNG::Rand(dist);                          // Generate Random Number.
struct RNG
{
    static std::mt19937 rng;

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
