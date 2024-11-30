/**
  ******************************************************************************
  * @file           : Bounds.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-17
  ******************************************************************************
  */



#ifndef BOUNDS_H
#define BOUNDS_H

#include <Core/Common.h>

struct AABB2D
{
    Eigen::Vector2d vmin;
    Eigen::Vector2d vmax;

    // Bound Two AABB2D lhs And rhs.
    NODISCARD CONSTEXPR FORCE_INLINE static AABB2D From(const AABB2D& lhs, const AABB2D& rhs) NOEXCEPT
    {
        return AABB2D
        {
            .vmin = lhs.vmin.array().min(rhs.vmin.array()),
            .vmax = lhs.vmax.array().max(rhs.vmax.array()),
        };
    }

    NODISCARD CONSTEXPR FORCE_INLINE Eigen::Vector2d Center() const NOEXCEPT
    {
        return (vmin + vmax) / 2.0;
    }

    NODISCARD CONSTEXPR FORCE_INLINE Eigen::Vector2d Radius() const NOEXCEPT
    {
        return (vmax - vmin) / 2.0;
    }

    NODISCARD CONSTEXPR FORCE_INLINE bool Contain(const Eigen::Vector2d& v) const NOEXCEPT
    {
        return ((vmin.array() <= v.array()) && (vmax.array() <= v.array())).all();
    }

    NODISCARD CONSTEXPR FORCE_INLINE bool Contain(const AABB2D& oth) const NOEXCEPT
    {
        return ((vmin.array() <= oth.vmin.array()) && (vmax.array() <= oth.vmax.array())).all();
    }

    NODISCARD CONSTEXPR FORCE_INLINE bool Surround(const Eigen::Vector2d& v) const NOEXCEPT
    {
        return ((vmin.array() < v.array()) && (vmax.array() < v.array())).all();
    }

    NODISCARD CONSTEXPR FORCE_INLINE bool Surround(const AABB2D& oth) const NOEXCEPT
    {
        return ((vmin.array() < oth.vmin.array()) && (vmax.array() < oth.vmax.array())).all();
    }

    NODISCARD CONSTEXPR FORCE_INLINE static bool OverLap(const AABB2D& lhs, const AABB2D& rhs) NOEXCEPT
    {
        // Ignore Single Point OverLap.
        return ((lhs.vmin.array() < rhs.vmax.array()) && (rhs.vmin.array() < lhs.vmax.array())).all();
    }

    static const AABB2D EMPTY;
    static const AABB2D UNIVERSE;
};

struct AABB3D
{
    Eigen::Vector3d vmin;
    Eigen::Vector3d vmax;

    // Bound Two AABB3D lhs And rhs.
    NODISCARD CONSTEXPR FORCE_INLINE static AABB3D From(const AABB3D& lhs, const AABB3D& rhs) NOEXCEPT
    {
        return AABB3D
        {
            .vmin = lhs.vmin.array().min(rhs.vmin.array()),
            .vmax = lhs.vmax.array().max(rhs.vmax.array()),
        };
    }

    NODISCARD CONSTEXPR FORCE_INLINE Eigen::Vector3d Center() const NOEXCEPT
    {
        return (vmin + vmax) / 2.0;
    }

    NODISCARD CONSTEXPR FORCE_INLINE Eigen::Vector3d Radius() const NOEXCEPT
    {
        return (vmax - vmin) / 2.0;
    }

    NODISCARD CONSTEXPR FORCE_INLINE bool Contain(const Eigen::Vector3d& v) const NOEXCEPT
    {
        return ((vmin.array() <= v.array()) && (v.array() <= vmax.array())).all();
    }

    NODISCARD CONSTEXPR FORCE_INLINE bool Contain(const AABB3D& oth) const NOEXCEPT
    {
        return ((vmin.array() <= oth.vmin.array()) && (oth.vmax.array() <= vmax.array())).all();
    }

    NODISCARD CONSTEXPR FORCE_INLINE bool Surround(const Eigen::Vector3d& v) const NOEXCEPT
    {
        return ((vmin.array() < v.array()) && (v.array() < vmax.array())).all();
    }

    NODISCARD CONSTEXPR FORCE_INLINE bool Surround(const AABB3D& oth) const NOEXCEPT
    {
        return ((vmin.array() < oth.vmin.array()) && (oth.vmax.array() < vmax.array())).all();
    }

    NODISCARD CONSTEXPR FORCE_INLINE static bool OverLap(const AABB3D& lhs, const AABB3D& rhs) NOEXCEPT
    {
        // Ignore Single Point OverLap.
        return ((lhs.vmin.array() < rhs.vmax.array()) && (rhs.vmin.array() < lhs.vmax.array())).all();
    }

    static const AABB3D EMPTY;
    static const AABB3D UNIVERSE;
};

#endif //BOUNDS_H
