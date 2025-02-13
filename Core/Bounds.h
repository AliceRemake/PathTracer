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
#include <Core/Ray.h>
#include <Core/Interval.h>

struct BoundingBox
{
protected:
    enum BoundingBoxKind
    {
        BOUNDING_BOX_KIND_AABB,
        BOUNDING_BOX_KIND_BVH,
    };

private:
    const BoundingBoxKind kind;

public:
    NODISCARD CONSTEXPR FORCE_INLINE BoundingBoxKind Kind() const NOEXCEPT { return kind; }

    NODISCARD virtual Eigen::Vector3d Center() const NOEXCEPT = 0;
    NODISCARD virtual bool Hit(const Ray& ray, const Interval& interval) const NOEXCEPT = 0;

    NODISCARD explicit BoundingBox(const BoundingBoxKind kind) NOEXCEPT : kind(kind) {}
    virtual ~BoundingBox() NOEXCEPT = default;
};

struct AABB final : BoundingBox
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const BoundingBox* ptr) NOEXCEPT {return ptr->Kind() == BOUNDING_BOX_KIND_AABB;}

    Interval xi, yi, zi;

    NODISCARD AABB() NOEXCEPT
    : BoundingBox(BOUNDING_BOX_KIND_AABB)
    {}

    NODISCARD AABB(const Interval& x_interval, const Interval& y_interval, const Interval& z_interval) NOEXCEPT
    : BoundingBox(BOUNDING_BOX_KIND_AABB), xi(x_interval), yi(y_interval), zi(z_interval)
    {}

    NODISCARD AABB(const Eigen::Vector3d& a, const Eigen::Vector3d& b) NOEXCEPT
    : BoundingBox(BOUNDING_BOX_KIND_AABB)
    {
        xi = (a.x() < b.x()) ? Interval{a.x(), b.x()} : Interval{b.x(), a.x()};
        yi = (a.y() < b.y()) ? Interval{a.y(), b.y()} : Interval{b.y(), a.y()};
        zi = (a.z() < b.z()) ? Interval{a.z(), b.z()} : Interval{b.z(), a.z()};
    }

    NODISCARD AABB(const AABB& a, const AABB& b) NOEXCEPT
    : BoundingBox(BOUNDING_BOX_KIND_AABB),
      xi(Interval::Union(a.xi, b.xi)),
      yi(Interval::Union(a.yi, b.yi)),
      zi(Interval::Union(a.zi, b.zi))
    {}

    NODISCARD Eigen::Vector3d Center() const NOEXCEPT OVERRIDE
    {
        return { xi.Center(), yi.Center(), zi.Center() };
    }

    NODISCARD bool Hit(const Ray &ray, const Interval &interval) const NOEXCEPT OVERRIDE
    {
        const double inv_dx = 1.0 / ray.direction.x();
        const double inv_dy = 1.0 / ray.direction.y();
        const double inv_dz = 1.0 / ray.direction.z();

        const Interval hxi = Interval((xi.imin - ray.origin.x()) * inv_dx, (xi.imax - ray.origin.x()) * inv_dx);
        const Interval hyi = Interval((yi.imin - ray.origin.y()) * inv_dy, (yi.imax - ray.origin.y()) * inv_dy);
        const Interval hzi = Interval((zi.imin - ray.origin.z()) * inv_dz, (zi.imax - ray.origin.z()) * inv_dz);
        const Interval hi = Interval::Intersection(Interval::Intersection(hxi, hyi), Interval::Intersection(hzi, interval));

        return !hi.IsEmpty();
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
