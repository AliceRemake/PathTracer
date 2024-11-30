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
#include <Core/Material.h>

struct Sphere final : Hittable
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Hittable* ptr) NOEXCEPT {return ptr->Kind() == HITTABLE_TYPE_SPHERE;}

    NODISCARD explicit Sphere() NOEXCEPT : Hittable(HITTABLE_TYPE_SPHERE), radius(), material() {}

    Eigen::Vector3d center;
    double radius;
    Material* material;

    NODISCARD bool Hit(const Ray &ray, const Interval& interval, HitRecord &record) NOEXCEPT OVERRIDE;
};

#endif //PRIMITIVE_H
