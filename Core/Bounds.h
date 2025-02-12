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
#include <Core/Interval.h>
#include <Core/Hittable.h>
#include <Core/Ray.h>

struct AABB final : Hittable
{
    Interval x_interval, y_interval, z_interval;

    NODISCARD AABB() NOEXCEPT : Hittable(HITTABLE_TYPE_AABB),
    x_interval(Interval::EMPTY), y_interval(Interval::EMPTY), z_interval(Interval::EMPTY) {}

    NODISCARD AABB(const Interval& x_interval, const Interval& y_interval, const Interval& z_interval) NOEXCEPT
    : Hittable(HITTABLE_TYPE_AABB), x_interval(x_interval), y_interval(y_interval), z_interval(z_interval) {}

    NODISCARD AABB(const Eigen::Vector3d& a, const Eigen::Vector3d& b) NOEXCEPT
    : Hittable(HITTABLE_TYPE_AABB), x_interval(), y_interval(), z_interval()
    {
        x_interval = (a.x() < b.x()) ? Interval{a.x(), b.x()} : Interval{b.x(), a.x()};
        y_interval = (a.y() < b.y()) ? Interval{a.y(), b.y()} : Interval{b.y(), a.y()};
        z_interval = (a.z() < b.z()) ? Interval{a.z(), b.z()} : Interval{b.z(), a.z()};
    }

    NODISCARD AABB(const AABB& a, const AABB& b) NOEXCEPT
    : Hittable(HITTABLE_TYPE_AABB),
      x_interval(Interval::Union(a.x_interval, b.x_interval)),
      y_interval(Interval::Union(a.y_interval, b.y_interval)),
      z_interval(Interval::Union(a.z_interval, b.z_interval))
    {}

    // TODO: Use Interval
    NODISCARD bool Hit(const Ray &ray, const Interval &interval UNUSED, HitRecord &record UNUSED) const NOEXCEPT OVERRIDE
    {
        // Acc.
        const double inv_dx = 1.0 / ray.direction.x();
        const double inv_dy = 1.0 / ray.direction.y();
        const double inv_dz = 1.0 / ray.direction.z();

        const double t0x = (x_interval.imin - ray.origin.x()) * inv_dx;
        const double t1x = (x_interval.imax - ray.origin.x()) * inv_dx;
        const double t0y = (y_interval.imin - ray.origin.y()) * inv_dy;
        const double t1y = (y_interval.imax - ray.origin.y()) * inv_dy;
        const double t0z = (z_interval.imin - ray.origin.z()) * inv_dz;
        const double t1z = (z_interval.imax - ray.origin.z()) * inv_dz;

        const Interval ix = Interval{std::min(t0x, t1x), std::max(t0x, t1x)};
        const Interval iy = Interval{std::min(t0y, t1y), std::max(t0y, t1y)};
        const Interval iz = Interval{std::min(t0z, t1z), std::max(t0z, t1z)};

        if (Interval::OverLap(ix, iy) && Interval::OverLap(iy, iz) && Interval::OverLap(iz, ix))
        {
            return true;
        }

        return false;
    }
};

// struct BVH : Hittable
// {
//     Ref<AABB> l_aabb, r_aabb;
//     Ref<AABB>
//     Ref<Hittable> aabb;
//
//     NODISCARD bool Hit(const Ray &ray, const Interval &interval, HitRecord &record) const NOEXCEPT OVERRIDE
//     {
//         // Acc.
//
//
//     }
// };

// struct BVH : Hittable
// {
//     Ref<Hittable> l, r;
//     AABB aabb;
//
//     NODISCARD bool Hit(const Ray &ray, const Interval& interval, HitRecord& record) const NOEXCEPT OVERRIDE
//     {
//         bool l_result = false, r_result = false;
//         if (l != nullptr) {l_result = l->Hit(ray, interval, record);}
//         if (r != nullptr) {r_result = r->Hit(ray, interval, record);}
//         return l_result || r_result;
//     }
// };

// struct AABB2D
// {
//     Eigen::Vector2d vmin;
//     Eigen::Vector2d vmax;
//
//     // Bound Two AABB2D lhs And rhs.
//     NODISCARD FORCE_INLINE static AABB2D From(const AABB2D& lhs, const AABB2D& rhs) NOEXCEPT
//     {
//         return AABB2D
//         {
//             .vmin = lhs.vmin.array().min(rhs.vmin.array()),
//             .vmax = lhs.vmax.array().max(rhs.vmax.array()),
//         };
//     }
//
//     NODISCARD FORCE_INLINE Eigen::Vector2d Center() const NOEXCEPT
//     {
//         return (vmin + vmax) / 2.0;
//     }
//
//     NODISCARD FORCE_INLINE Eigen::Vector2d Radius() const NOEXCEPT
//     {
//         return (vmax - vmin) / 2.0;
//     }
//
//     NODISCARD FORCE_INLINE bool Contain(const Eigen::Vector2d& v) const NOEXCEPT
//     {
//         return ((vmin.array() <= v.array()) && (vmax.array() <= v.array())).all();
//     }
//
//     NODISCARD FORCE_INLINE bool Contain(const AABB2D& oth) const NOEXCEPT
//     {
//         return ((vmin.array() <= oth.vmin.array()) && (vmax.array() <= oth.vmax.array())).all();
//     }
//
//     NODISCARD FORCE_INLINE bool Surround(const Eigen::Vector2d& v) const NOEXCEPT
//     {
//         return ((vmin.array() < v.array()) && (vmax.array() < v.array())).all();
//     }
//
//     NODISCARD FORCE_INLINE bool Surround(const AABB2D& oth) const NOEXCEPT
//     {
//         return ((vmin.array() < oth.vmin.array()) && (vmax.array() < oth.vmax.array())).all();
//     }
//
//     NODISCARD FORCE_INLINE static bool OverLap(const AABB2D& lhs, const AABB2D& rhs) NOEXCEPT
//     {
//         // Ignore Single Point OverLap.
//         return ((lhs.vmin.array() < rhs.vmax.array()) && (rhs.vmin.array() < lhs.vmax.array())).all();
//     }
//
//     static const AABB2D EMPTY;
//     static const AABB2D UNIVERSE;
// };
//
// struct AABB3D
// {
//     Eigen::Vector3d vmin;
//     Eigen::Vector3d vmax;
//
//     // Bound Two AABB3D lhs And rhs.
//     NODISCARD FORCE_INLINE static AABB3D From(const AABB3D& lhs, const AABB3D& rhs) NOEXCEPT
//     {
//         return AABB3D
//         {
//             .vmin = lhs.vmin.array().min(rhs.vmin.array()),
//             .vmax = lhs.vmax.array().max(rhs.vmax.array()),
//         };
//     }
//
//     NODISCARD FORCE_INLINE Eigen::Vector3d Center() const NOEXCEPT
//     {
//         return (vmin + vmax) / 2.0;
//     }
//
//     NODISCARD FORCE_INLINE Eigen::Vector3d Radius() const NOEXCEPT
//     {
//         return (vmax - vmin) / 2.0;
//     }
//
//     NODISCARD FORCE_INLINE bool Contain(const Eigen::Vector3d& v) const NOEXCEPT
//     {
//         return ((vmin.array() <= v.array()) && (v.array() <= vmax.array())).all();
//     }
//
//     NODISCARD FORCE_INLINE bool Contain(const AABB3D& oth) const NOEXCEPT
//     {
//         return ((vmin.array() <= oth.vmin.array()) && (oth.vmax.array() <= vmax.array())).all();
//     }
//
//     NODISCARD FORCE_INLINE bool Surround(const Eigen::Vector3d& v) const NOEXCEPT
//     {
//         return ((vmin.array() < v.array()) && (v.array() < vmax.array())).all();
//     }
//
//     NODISCARD FORCE_INLINE bool Surround(const AABB3D& oth) const NOEXCEPT
//     {
//         return ((vmin.array() < oth.vmin.array()) && (oth.vmax.array() < vmax.array())).all();
//     }
//
//     NODISCARD FORCE_INLINE static bool OverLap(const AABB3D& lhs, const AABB3D& rhs) NOEXCEPT
//     {
//         // Ignore Single Point OverLap.
//         return ((lhs.vmin.array() < rhs.vmax.array()) && (rhs.vmin.array() < lhs.vmax.array())).all();
//     }
//
//     static const AABB3D EMPTY;
//     static const AABB3D UNIVERSE;
// };

#endif //BOUNDS_H
