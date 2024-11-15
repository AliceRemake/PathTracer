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

#ifndef COMMON_H
#define COMMON_H

#include <fmt/format.h>
#include <fmt/printf.h>
#include <Eigen/Core>

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

namespace FS = std::filesystem;

#define NODISCARD [[nodiscard]]
#define NOEXCEPT noexcept
#define CONSTEXPR constexpr
#define FORCE_INLINE inline __attribute__((always_inline))

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
  #define ASSERT(exp) do { if(!(exp)) DEBUGBREAK(); } while (0)
#endif

#endif //COMMON_H
