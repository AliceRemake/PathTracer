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

        CONSTEXPR double delta = 0.0001;
        if (xi.IsEmpty()) { xi = xi.Expand(delta); }
        if (yi.IsEmpty()) { yi = yi.Expand(delta); }
        if (zi.IsEmpty()) { zi = zi.Expand(delta); }
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

#endif //BOUNDS_H
