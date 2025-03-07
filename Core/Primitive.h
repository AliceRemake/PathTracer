/**
  ******************************************************************************
  * @file           : Primitive.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-27
  ******************************************************************************
  */



#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <Core/Common.h>
#include <Core/Hittable.h>

struct Primitive : Hittable
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Hittable* ptr) NOEXCEPT
    { return HITTABLE_KIND_PRIMITIVE_START < ptr->Kind() && ptr->Kind() < HITTABLE_KIND_PRIMITIVE_END; }

protected:
    Ref<BoundingBox> bounding_box;
    NODISCARD const Ref<BoundingBox>& GetBoundingBox() const NOEXCEPT OVERRIDE { return bounding_box; }
    NODISCARD explicit Primitive(HittableKind hittable_type) NOEXCEPT : Hittable(hittable_type), bounding_box(nullptr) {}

    // Interface.
    NODISCARD virtual Ref<BoundingBox> CreateBoundingBox() const NOEXCEPT = 0;
};

// struct Plane final : Primitive3D
// {
//     NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Hittable* ptr) NOEXCEPT {return ptr->Kind() == HITTABLE_TYPE_PLANE;}
//
//     Ref<Material> material;
//     Eigen::Vector4d plane; // Ax + By + Cz + D = 0
//
//     NODISCARD Plane(const Ref<Material>& material, const Eigen::Vector3d& center, const Eigen::Vector3d& normal) NOEXCEPT
//     : Primitive3D(HITTABLE_TYPE_PLANE, MakeRef<AABB3D>(AABB3D::UNIVERSE)), material(material), plane(normal)
//     {
//         plane.w() = -normal.dot(center);
//         InitializeAABB();
//     }
//
//     void InitializeAABB() NOEXCEPT OVERRIDE
//     {
//         aabb = AABB3D::UNIVERSE;
//     }
//
//     NODISCARD Eigen::Vector2d UV(const Eigen::Vector3d& hit_point) const NOEXCEPT
//     {
//
//     }
//
//     NODISCARD bool Hit(const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT OVERRIDE;
// };
//
// struct Disk final : Primitive
// {
//     NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Hittable* ptr) NOEXCEPT {return ptr->Kind() == HITTABLE_TYPE_DISK;}
//
//     Ref<Material> material;
//     Eigen::Vector3d center;
//     Eigen::Vector3d normal;
//     double radius;
//
//     NODISCARD Disk(const Ref<Material>& material, Eigen::Vector3d center, Eigen::Vector3d normal, const double radius) NOEXCEPT
//     : Primitive(HITTABLE_TYPE_DISK), material(material), center(std::move(center)), normal(std::move(normal)), radius(radius)
//     {
//         InitializeAABB();
//     }
//
//     void InitializeAABB() NOEXCEPT OVERRIDE
//     {
//         const double delta_x = radius * normal.cross(Eigen::Vector3d(1.0, 0.0, 0.0)).norm();
//         const double delta_y = radius * normal.cross(Eigen::Vector3d(0.0, 1.0, 0.0)).norm();
//         const double delta_z = radius * normal.cross(Eigen::Vector3d(0.0, 0.0, 1.0)).norm();
//         aabb.vmin.x() = center.x() - delta_x;
//         aabb.vmax.x() = center.x() + delta_x;
//         aabb.vmin.y() = center.y() - delta_y;
//         aabb.vmax.y() = center.y() + delta_y;
//         aabb.vmin.z() = center.z() - delta_z;
//         aabb.vmax.z() = center.z() + delta_z;
//     }
//
//     NODISCARD Eigen::Vector2d UV(const Eigen::Vector3d& hit_point) const NOEXCEPT
//     {
//
//     }
//
//     NODISCARD bool Hit(const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT OVERRIDE;
// };
//
// struct Triangle final : Primitive
// {
//     NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Hittable* ptr) NOEXCEPT {return ptr->Kind() == HITTABLE_TYPE_TRIANGLE;}
//
//     Ref<Material> material;
//     Eigen::Vector3d v0, v1, v2;
//
//     NODISCARD Triangle(const Ref<Material>& material, Eigen::Vector3d v0, Eigen::Vector3d v1, Eigen::Vector3d v2) NOEXCEPT
//     : Primitive(HITTABLE_TYPE_TRIANGLE), material(material), v0(std::move(v0)), v1(std::move(v1)), v2(std::move(v2))
//     {
//         InitializeAABB();
//     }
//
//     void InitializeAABB() NOEXCEPT OVERRIDE
//     {
//         aabb.vmin = v0.array().min(v1.array().min(v2.array()));
//         aabb.vmax = v0.array().max(v1.array().max(v2.array()));
//     }
//
//     NODISCARD Eigen::Vector2d UV(const Eigen::Vector3d& hit_point) const NOEXCEPT
//     {
//
//     }
//
//     NODISCARD bool Hit(const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT OVERRIDE;
// };

struct Triangle final : Primitive
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Hittable* ptr) NOEXCEPT {return ptr->Kind() == HITTABLE_KIND_TRIANGLE;}

    Ref<Material> material;
    Eigen::Vector3d origin;
    Eigen::Vector3d u, v;

    NODISCARD Triangle(const Ref<Material>& material, Eigen::Vector3d origin, Eigen::Vector3d u, Eigen::Vector3d v) NOEXCEPT
    : Primitive(HITTABLE_KIND_QUADRANGLE), material(material), origin(std::move(origin)), u(std::move(u)), v(std::move(v))
    {
        // NOTE: No Need.
        // bounding_box = CreateBoundingBox();
    }

    NODISCARD Ref<BoundingBox> CreateBoundingBox() const NOEXCEPT OVERRIDE
    {
        Eigen::Vector3d p0 = origin + u;
        Eigen::Vector3d p1 = origin + v;
        return MakeRef<AABB>(
            Eigen::Vector3d {
                std::min(std::min(origin.x(), origin.y()), origin.z()),
                std::min(std::min(p0.x(), p0.y()), p0.z()),
                std::min(std::min(p1.x(), p1.y()), p1.z())
            },
            Eigen::Vector3d {
                std::max(std::max(origin.x(), origin.y()), origin.z()),
                std::max(std::max(p0.x(), p0.y()), p0.z()),
                std::max(std::max(p1.x(), p1.y()), p1.z())
            }
        );
    }

    NODISCARD Eigen::Vector2d Texcoord2D(const Eigen::Vector3d& hit_point) const NOEXCEPT
    {
        const Eigen::Vector3d p = hit_point - origin;
        const double u_length = std::sqrt(u.squaredNorm());
        const double v_length = std::sqrt(v.squaredNorm());
        const double a = p.dot(u) / u_length;
        const double b = p.dot(v) / v_length;
        const double cos_theta = u.dot(v) / (u_length * v_length);
        const double aa = (a + b) / (1 + cos_theta);
        const double bb = (a - b) / (1 - cos_theta);
        return { (aa + bb) / 2.0, (aa - bb) / 2.0 };
    }

    NODISCARD bool Hit(const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT OVERRIDE;
};

struct Quadrangle final : Primitive
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Hittable* ptr) NOEXCEPT {return ptr->Kind() == HITTABLE_KIND_QUADRANGLE;}

    Ref<Material> material;
    Eigen::Vector3d origin;
    Eigen::Vector3d u, v;

    NODISCARD Quadrangle(const Ref<Material>& material, Eigen::Vector3d origin, Eigen::Vector3d u, Eigen::Vector3d v) NOEXCEPT
    : Primitive(HITTABLE_KIND_QUADRANGLE), material(material), origin(std::move(origin)), u(std::move(u)), v(std::move(v))
    {
        // NOTE: No Need.
        // bounding_box = CreateBoundingBox();
    }

    NODISCARD Ref<BoundingBox> CreateBoundingBox() const NOEXCEPT OVERRIDE
    {
        return MakeRef<AABB>(
            AABB(origin, origin + u),
            AABB(origin + v, origin + u + v)
        );
    }

    NODISCARD Eigen::Vector2d Texcoord2D(const Eigen::Vector3d& hit_point) const NOEXCEPT
    {
        const Eigen::Vector3d p = hit_point - origin;
        const double u_length = std::sqrt(u.squaredNorm());
        const double v_length = std::sqrt(v.squaredNorm());
        const double a = p.dot(u) / u_length;
        const double b = p.dot(v) / v_length;
        const double cos_theta = u.dot(v) / (u_length * v_length);
        const double aa = (a + b) / (1 + cos_theta);
        const double bb = (a - b) / (1 - cos_theta);
        return { (aa + bb) / 2.0, (aa - bb) / 2.0 };
    }

    NODISCARD bool Hit(const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT OVERRIDE;
};

struct Sphere final : Primitive
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Hittable* ptr) NOEXCEPT {return ptr->Kind() == HITTABLE_KIND_SPHERE;}

    Ref<Material> material;
    Eigen::Vector3d center;
    double radius;

    NODISCARD Sphere(const Ref<Material>& material, Eigen::Vector3d center, const double radius) NOEXCEPT
    : Primitive(HITTABLE_KIND_SPHERE), material(material), center(std::move(center)), radius(radius)
    {
        // NOTE: No Need.
        // bounding_box = CreateBoundingBox();
    }

    NODISCARD Ref<BoundingBox> CreateBoundingBox() const NOEXCEPT OVERRIDE
    {
        return MakeRef<AABB>(center - Eigen::Vector3d{radius, radius, radius}, center + Eigen::Vector3d{radius, radius, radius});
    }

    NODISCARD Eigen::Vector2d Texcoord2D(const Eigen::Vector3d& hit_point) const NOEXCEPT
    {
        const Eigen::Vector3d p = (hit_point - center).normalized();
        const double theta = std::acos(p.y());
        const double phi = std::atan2(p.z(), p.x()) + PI;
        return { phi / (2.0 * PI), theta / PI };
    }

    NODISCARD bool Hit(const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT OVERRIDE;
};

#endif //PRIMITIVE_H
