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
    double t;
    Ref<Material> material;
    Eigen::Vector3d hit_point;
    Eigen::Vector3d hit_normal;
};

struct Hittable
{
protected:
    enum HittableType
    {
        HITTABLE_TYPE_SPHERE,
        HITTABLE_TYPE_MESH,
        HITTABLE_TYPE_SCENE,
    };

    const HittableType kind;

public:
    NODISCARD CONSTEXPR FORCE_INLINE HittableType Kind() const NOEXCEPT {return kind;}

    NODISCARD explicit Hittable(HittableType kind) NOEXCEPT : kind(kind) {}
    virtual ~Hittable() NOEXCEPT = default;

    NODISCARD virtual bool Hit(const Ray& ray, const Interval& interval, HitRecord& record) const NOEXCEPT = 0;
};

#endif //HITTABLE_H
