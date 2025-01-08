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

struct Sphere final : Hittable
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Hittable* ptr) NOEXCEPT {return ptr->Kind() == HITTABLE_TYPE_SPHERE;}

    NODISCARD explicit Sphere() NOEXCEPT : Hittable(HITTABLE_TYPE_SPHERE), radius() {}
    NODISCARD Sphere(Eigen::Vector3d  center, const double radius, const Ref<Material>& material) NOEXCEPT
    : Hittable(HITTABLE_TYPE_SPHERE), center(std::move(center)), radius(radius), material(material) {}

    Eigen::Vector3d center;
    double radius;
    Ref<Material> material;

    NODISCARD bool Hit(const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT OVERRIDE;
};

#endif //PRIMITIVE_H
