/**
  ******************************************************************************
  * @file           : Hittable.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-27
  ******************************************************************************
  */



#ifndef HITTABLE_H
#define HITTABLE_H

#include <Core/Common.h>

struct Interval;
struct Material;
struct Ray;

struct HitRecord
{
    enum ScatterType
    {
        SCATTER_TYPE_REFLECT,
        SCATTER_TYPE_REFRACT,
    };

    double t;
    ScatterType scatter_type;
    Ref<Material> material;
    Eigen::Vector2d texcoord2d;
    Eigen::Vector3d hit_point;
    Eigen::Vector3d hit_normal;
};

// ReSharper disable once CppClassCanBeFinal
struct Hittable
{
protected:
    enum HittableType
    {
        HITTABLE_TYPE_LIST,
        HITTABLE_TYPE_PRIMITIVE_3D_START,
        HITTABLE_TYPE_PLANE,
        HITTABLE_TYPE_DISK,
        HITTABLE_TYPE_TRIANGLE,
        HITTABLE_TYPE_QUADRANGLE,
        HITTABLE_TYPE_SPHERE,
        HITTABLE_TYPE_PRIMITIVE_3D_END,
        HITTABLE_TYPE_MESH,
        HITTABLE_TYPE_SCENE,
        // Acc
        HITTABLE_TYPE_AABB,
    };

    const HittableType kind;

public:
    NODISCARD CONSTEXPR FORCE_INLINE HittableType Kind() const NOEXCEPT {return kind;}

    NODISCARD explicit Hittable(const HittableType kind) NOEXCEPT : kind(kind) {}
    virtual ~Hittable() NOEXCEPT = default;

    NODISCARD virtual bool Hit(const Ray& ray, const Interval& interval, HitRecord& record) const NOEXCEPT = 0;
};

struct HittableList final : Hittable
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Hittable* ptr) NOEXCEPT {return ptr->Kind() == HITTABLE_TYPE_LIST;}

    std::vector<Ref<Hittable>> hittable_list;

    NODISCARD HittableList() NOEXCEPT : Hittable(HITTABLE_TYPE_LIST) {}
    NODISCARD explicit HittableList(const std::vector<Ref<Hittable>>& hittable_list) NOEXCEPT
    : Hittable(HITTABLE_TYPE_LIST), hittable_list(hittable_list) {}

    void PushBack(const Ref<Hittable>& hittable) NOEXCEPT {hittable_list.push_back(hittable);}
    void PopBack() NOEXCEPT {hittable_list.pop_back();}

    NODISCARD bool Hit(const Ray& ray, const Interval& interval, HitRecord& record) const NOEXCEPT OVERRIDE;
};

#endif //HITTABLE_H
