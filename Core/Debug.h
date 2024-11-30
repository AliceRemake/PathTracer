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

    template<class T>
    FORCE_INLINE static void Unuse(T&& arg) NOEXCEPT
    {
        (void)arg;
    }

    template<class T, class... Args>
    FORCE_INLINE static void Unuse(T&& arg, Args&&... args) NOEXCEPT
    {
        (void)arg; Unuse(std::forward<Args>(args)...);
    }

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

    NODISCARD FORCE_INLINE static size_t TimeNanoS() NOEXCEPT
    {
        return std::chrono::system_clock::now().time_since_epoch().count();
    }

    NODISCARD FORCE_INLINE static size_t TimeMicroS() NOEXCEPT
    {
        return std::chrono::system_clock::now().time_since_epoch().count() / 1000;
    }

    NODISCARD FORCE_INLINE static size_t TimeMilliS() NOEXCEPT
    {
        return std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
    }
};

#endif //DEBUG_H
