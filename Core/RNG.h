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
// NOTE: Seed Each Thread Independently.
struct RNG
{
private:
    THREAD_LOCAL static std::mt19937 rng;

public:
    template<typename T>
    FORCE_INLINE static void Seed(T&& seed) NOEXCEPT
    {
        rng.seed(std::forward<T>(seed));
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
};

#endif //RNG_H
