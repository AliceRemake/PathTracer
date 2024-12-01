/**
  ******************************************************************************
  * @file           : Scene.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-27
  ******************************************************************************
  */



#ifndef SCENE_H
#define SCENE_H

#include <Core/Common.h>
#include <Core/Hittable.h>

struct Scene final : Hittable
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Hittable* ptr) NOEXCEPT {return ptr->Kind() == HITTABLE_TYPE_SCENE;}

    NODISCARD explicit Scene() NOEXCEPT : Hittable(HITTABLE_TYPE_SCENE) {}

    std::vector<Ref<Hittable>> objects;

    NODISCARD bool Hit(const Ray &ray, const Interval &interval, HitRecord &record) const NOEXCEPT OVERRIDE;
};

#endif //SCENE_H
