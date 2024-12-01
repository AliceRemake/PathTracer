/**
  ******************************************************************************
  * @file           : Material.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-27
  ******************************************************************************
  */



#ifndef MATERIAL_H
#define MATERIAL_H

#include <Core/Common.h>
#include <Core/Hittable.h>

struct Ray;

struct Material
{
protected:
    enum MaterialType
    {
        MATERIAL_TYPE_BASIC,
        MATERIAL_TYPE_LAMBERT,
        MATERIAL_TYPE_METAL,
        MATERIAL_TYPE_DIELECTRIC,
    };

    const MaterialType kind;

public:
    NODISCARD CONSTEXPR FORCE_INLINE MaterialType Kind() const NOEXCEPT {return kind;}

    NODISCARD explicit Material(const MaterialType kind) NOEXCEPT : kind(kind) {}
    virtual ~Material() NOEXCEPT = default;

    NODISCARD virtual Ray Scatter(const Ray& ray, const HitRecord& record) const NOEXCEPT = 0;
    NODISCARD virtual Eigen::Vector3d Shading(const Ray& ray, const HitRecord& record, const Eigen::Vector3d& color) const NOEXCEPT = 0;
};

struct BasicMaterial final : Material
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Material* ptr) NOEXCEPT {return ptr->Kind() == MATERIAL_TYPE_BASIC;}

    NODISCARD explicit BasicMaterial() NOEXCEPT : Material(MATERIAL_TYPE_BASIC) {}
    NODISCARD explicit BasicMaterial(const Eigen::Vector3d& diffuse) NOEXCEPT
    : Material(MATERIAL_TYPE_BASIC), diffuse(diffuse) {}

    Eigen::Vector3d diffuse;

    NODISCARD Ray Scatter(const Ray &ray, const HitRecord &record) const NOEXCEPT OVERRIDE;
    NODISCARD Eigen::Vector3d Shading(const Ray& ray, const HitRecord& record, const Eigen::Vector3d& color) const NOEXCEPT OVERRIDE;
};

struct LambertMaterial final : Material
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Material* ptr) NOEXCEPT {return ptr->Kind() == MATERIAL_TYPE_LAMBERT;}

    NODISCARD explicit LambertMaterial() NOEXCEPT : Material(MATERIAL_TYPE_LAMBERT) {}
    NODISCARD explicit LambertMaterial(const Eigen::Vector3d& diffuse) NOEXCEPT
    : Material(MATERIAL_TYPE_LAMBERT), diffuse(diffuse) {}

    Eigen::Vector3d diffuse;

    NODISCARD Ray Scatter(const Ray &ray, const HitRecord &record) const NOEXCEPT OVERRIDE;
    NODISCARD Eigen::Vector3d Shading(const Ray& ray, const HitRecord& record, const Eigen::Vector3d& color) const NOEXCEPT OVERRIDE;
};

#endif //MATERIAL_H
