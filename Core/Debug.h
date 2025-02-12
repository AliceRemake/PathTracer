/**
  ******************************************************************************
  * @file           : Debug.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-16
  ******************************************************************************
  */



#ifndef DEBUG_H
#define DEBUG_H

#include <Core/Common.h>
#include <chrono>

struct Debug
{
    static void Dump(FILE* fp, const Eigen::Vector3f& v) NOEXCEPT;

    template<class T> FORCE_INLINE static void Unuse(T&& arg) NOEXCEPT { (void)arg; }
    template<class T, class... Args> FORCE_INLINE static void Unuse(T&& arg, Args&&... args) NOEXCEPT { (void)arg; Unuse(std::forward<Args>(args)...); }

    template<class... Args>
    FORCE_INLINE static void Print(fmt::format_string<Args...>&& format, Args&&... args) NOEXCEPT
    {
        #ifdef NDEBUG
        Unuse(format); Unuse(std::forward<Args>(args)...);
        #else
        static std::mutex mutex;
        {
            std::unique_lock lock(mutex);
            fmt::print(stdout, std::forward<fmt::format_string<Args...>>(format), std::forward<Args>(args)...);
            fmt::fprintf(stdout, "\n");
            fflush(stdout);
        }
        #endif
    }

    NODISCARD FORCE_INLINE static std::chrono::high_resolution_clock::time_point Now() NOEXCEPT
    {
        return std::chrono::high_resolution_clock::now();
    }

    NODISCARD FORCE_INLINE static size_t NanoSeconds(const std::chrono::high_resolution_clock::duration& duration) NOEXCEPT
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    }

    NODISCARD FORCE_INLINE static size_t MicroSeconds(const std::chrono::high_resolution_clock::duration& duration) NOEXCEPT
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    }

    NODISCARD FORCE_INLINE static size_t MilliSeconds(const std::chrono::high_resolution_clock::duration& duration) NOEXCEPT
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }

    NODISCARD FORCE_INLINE static size_t Seconds(const std::chrono::high_resolution_clock::duration& duration) NOEXCEPT
    {
        return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    }
};

#endif //DEBUG_H
