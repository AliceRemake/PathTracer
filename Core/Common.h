/**
  ******************************************************************************
  * @file           : Common.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-16
  ******************************************************************************
  */



// ReSharper disable CppUnusedIncludeDirective
// ReSharper disable CppDFAUnreachableFunctionCall

#ifndef COMMON_H
#define COMMON_H

#include <fmt/format.h>
#include <fmt/printf.h>

#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable:5054)
#endif

#include <Eigen/Core>
#include <Eigen/Geometry>

#if defined(_MSC_VER)
    #pragma warning(pop)
#endif

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <bitset>
#include <numeric>
#include <list>
#include <array>
#include <stack>
#include <queue>
#include <deque>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <functional>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <future>
#include <type_traits>
#include <random>

#define NODISCARD [[nodiscard]]
#define NOEXCEPT noexcept
#define OVERRIDE override
#define CONSTEXPR constexpr
#if defined(__GNUC__) || defined(__clang__)
    #define FORCE_INLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER)
    #define FORCE_INLINE __forceinline
#else
    #define FORCE_INLINE
#endif

#define __STR(s) #s // NOLINT(*-reserved-identifier)
#define STR(s) __STR(s)

#ifdef NDEBUG
  #define DEBUGBREAK()
#else
  #define DEBUGBREAK() __debugbreak()
#endif

#ifdef NDEBUG
  #define ASSERT(exp) (void)(exp)
#else
  #define ASSERT(exp) do { if(!(exp)) {DEBUGBREAK(); exit(EXIT_FAILURE);} } while (0)
#endif

#define FATAL(msg) do { fmt::fprintf(stderr, msg); exit(EXIT_FAILURE); } while(0)

template <typename TO, typename FROM>
NODISCARD CONSTEXPR FORCE_INLINE bool IsA(FROM* ptr) NOEXCEPT {
    if (ptr == nullptr) {return false;}
    return TO::ClassOf(ptr);
}
  
template <typename TO, typename FROM>
NODISCARD CONSTEXPR FORCE_INLINE bool IsA(const FROM* ptr) NOEXCEPT {
    if (ptr == nullptr) {return false;}
    return TO::ClassOf(ptr);
}
  
template <typename TO, typename FROM>
NODISCARD CONSTEXPR FORCE_INLINE TO* Cast(FROM* ptr) NOEXCEPT {
    ASSERT(IsA<TO>(ptr));
    return static_cast<TO*>(ptr);
}
  
template <typename TO, typename FROM>
NODISCARD CONSTEXPR FORCE_INLINE const TO* Cast(const FROM* ptr) NOEXCEPT {
    ASSERT(IsA<TO>(ptr));
    return static_cast<const TO*>(ptr);
}
  
template <typename TO, typename FROM>
NODISCARD CONSTEXPR FORCE_INLINE TO* DynCast(FROM* ptr) NOEXCEPT {
    return IsA<TO>(ptr) ? Cast<TO>(ptr) : nullptr;
}
  
template <typename TO, typename FROM>
NODISCARD CONSTEXPR FORCE_INLINE const TO* DynCast(const FROM* ptr) NOEXCEPT {
    return IsA<TO>(ptr) ? Cast<TO>(ptr) : nullptr;
}

namespace FS = std::filesystem;

template<typename T> using Uni = std::unique_ptr<T>;
template<typename T> using Ref = std::shared_ptr<T>;

template<typename T>
NODISCARD CONSTEXPR FORCE_INLINE Uni<T> MakeUni() NOEXCEPT
{
    return std::make_unique<T>();
}

template<typename T, typename... Args>
NODISCARD CONSTEXPR FORCE_INLINE Uni<T> MakeUni(Args&&... args) NOEXCEPT
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
NODISCARD CONSTEXPR FORCE_INLINE Ref<T> MakeRef() NOEXCEPT
{
    return std::make_shared<T>();
}

template<typename T, typename... Args>
NODISCARD CONSTEXPR FORCE_INLINE Ref<T> MakeRef(Args&&... args) NOEXCEPT
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

CONSTEXPR double EPS = std::numeric_limits<double>::epsilon();
CONSTEXPR double INF = std::numeric_limits<double>::infinity();
CONSTEXPR double PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;

NODISCARD FORCE_INLINE double ToRadians(const double degree) NOEXCEPT
{
    return degree * PI / 180.0;
}

NODISCARD FORCE_INLINE double ToDegree(const double radians) NOEXCEPT
{
    return radians * 180.0 / PI;
}

NODISCARD FORCE_INLINE bool Feq(const double x, const double y) NOEXCEPT
{
    return std::abs(x - y) <= EPS;
}

NODISCARD FORCE_INLINE bool Fne(const double x, const double y) NOEXCEPT
{
    return std::abs(x - y) > EPS;
}

NODISCARD FORCE_INLINE bool Fgt(const double x, const double y) NOEXCEPT
{
    return x > y + EPS;
}

NODISCARD FORCE_INLINE bool Fge(const double x, const double y) NOEXCEPT
{
    return x >= y - EPS;
}

NODISCARD FORCE_INLINE bool Flt(const double x, const double y) NOEXCEPT
{
    return x < y - EPS;
}

NODISCARD FORCE_INLINE bool Fle(const double x, const double y) NOEXCEPT
{
    return x <= y + EPS;
}

NODISCARD FORCE_INLINE bool FIsZero(const double x) NOEXCEPT
{
    return Feq(x, 0.0);
}

NODISCARD FORCE_INLINE bool FIsInfinity(const double x) NOEXCEPT
{
    return x == INF;
}

NODISCARD FORCE_INLINE bool FIsPositive(const double x) NOEXCEPT
{
    return Fgt(x, 0.0);
}

NODISCARD FORCE_INLINE bool FIsNegative(const double x) NOEXCEPT
{
    return Flt(x, 0.0);
}

#endif //COMMON_H
