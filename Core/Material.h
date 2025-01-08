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
        MATERIAL_TYPE_DIFFUSE_BEGIN,
        MATERIAL_TYPE_DIFFUSE_UNIFORM,
        MATERIAL_TYPE_DIFFUSE_LAMBERT,
        MATERIAL_TYPE_DIFFUSE_END,

        MATERIAL_TYPE_METAL,

        MATERIAL_TYPE_DIELECTRICS,
    };

    const MaterialType kind;

public:
    NODISCARD CONSTEXPR FORCE_INLINE MaterialType Kind() const NOEXCEPT {return kind;}

    NODISCARD explicit Material(const MaterialType kind) NOEXCEPT : kind(kind) {}
    virtual ~Material() NOEXCEPT = default;

    NODISCARD virtual Ray Scatter(const Ray& ray, const HitRecord& record) const NOEXCEPT = 0;
    NODISCARD virtual Eigen::Vector3d Shading(const Ray& ray, const HitRecord& record, const Eigen::Vector3d& color) const NOEXCEPT = 0;

    NODISCARD static Eigen::Vector3d Reflect(const Eigen::Vector3d& i, const Eigen::Vector3d& n) NOEXCEPT
    {
        return - (i - 2.0 * i.dot(n) * n).normalized();
    }

    NODISCARD static Eigen::Vector3d Refract(const double cos_theta, const Eigen::Vector3d& i, const Eigen::Vector3d& n, const double rior) NOEXCEPT
    {
        const Eigen::Vector3d perpendicular = (FIsNegative(cos_theta) ? 1.0 / rior : rior) * (- i + cos_theta * n);
        const Eigen::Vector3d parallel = - SSqrt(1.0 - perpendicular.squaredNorm()) * n;
        return (perpendicular + parallel).normalized();
    }

    NODISCARD static double SchlickReflectance(const double cos_theta, const double rior) NOEXCEPT
    {
        const double r0 = ((1 - rior) / (1 + rior)) * ((1 - rior) / (1 + rior));
        const double reflectance =  r0 + (1.0 - r0) * std::pow(1.0 - cos_theta, 5);
        return reflectance;
    }
};

struct DiffuseMaterial : Material
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Material* ptr) NOEXCEPT
    {
        return MATERIAL_TYPE_DIFFUSE_BEGIN < ptr->Kind() && ptr->Kind() < MATERIAL_TYPE_DIFFUSE_END;
    }

    NODISCARD explicit DiffuseMaterial(const MaterialType kind) NOEXCEPT : Material(kind) {}
};

struct DiffuseUniformMaterial final : DiffuseMaterial
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Material* ptr) NOEXCEPT {return ptr->Kind() == MATERIAL_TYPE_DIFFUSE_UNIFORM;}

    NODISCARD explicit DiffuseUniformMaterial() NOEXCEPT : DiffuseMaterial(MATERIAL_TYPE_DIFFUSE_UNIFORM) {}
    NODISCARD explicit DiffuseUniformMaterial(Eigen::Vector3d diffuse) NOEXCEPT
    : DiffuseMaterial(MATERIAL_TYPE_DIFFUSE_UNIFORM), reflection(std::move(diffuse)) {}

    Eigen::Vector3d reflection;

    NODISCARD Ray Scatter(const Ray &ray, const HitRecord &record) const NOEXCEPT OVERRIDE;
    NODISCARD Eigen::Vector3d Shading(const Ray& ray, const HitRecord& record, const Eigen::Vector3d& color) const NOEXCEPT OVERRIDE;
};

struct DiffuseLambertMaterial final : DiffuseMaterial
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Material* ptr) NOEXCEPT {return ptr->Kind() == MATERIAL_TYPE_DIFFUSE_LAMBERT;}

    NODISCARD explicit DiffuseLambertMaterial() NOEXCEPT : DiffuseMaterial(MATERIAL_TYPE_DIFFUSE_LAMBERT) {}
    NODISCARD explicit DiffuseLambertMaterial(Eigen::Vector3d diffuse) NOEXCEPT
    : DiffuseMaterial(MATERIAL_TYPE_DIFFUSE_LAMBERT), reflection(std::move(diffuse)) {}

    Eigen::Vector3d reflection;

    NODISCARD Ray Scatter(const Ray &ray, const HitRecord &record) const NOEXCEPT OVERRIDE;
    NODISCARD Eigen::Vector3d Shading(const Ray& ray, const HitRecord& record, const Eigen::Vector3d& color) const NOEXCEPT OVERRIDE;
};

struct MetalMaterial final : Material
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Material* ptr) NOEXCEPT {return ptr->Kind() == MATERIAL_TYPE_METAL;}

    NODISCARD explicit MetalMaterial() NOEXCEPT : Material(MATERIAL_TYPE_METAL), fuzziness() {}
    NODISCARD explicit MetalMaterial(Eigen::Vector3d reflection, double fuzziness) NOEXCEPT
    : Material(MATERIAL_TYPE_METAL), reflection(std::move(reflection)), fuzziness(fuzziness) {}

    Eigen::Vector3d reflection;
    double fuzziness;

    NODISCARD Ray Scatter(const Ray &ray, const HitRecord &record) const NOEXCEPT OVERRIDE;
    NODISCARD Eigen::Vector3d Shading(const Ray& ray, const HitRecord& record, const Eigen::Vector3d& color) const NOEXCEPT OVERRIDE;
};

struct DielectricMaterial final : Material
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Material* ptr) NOEXCEPT {return ptr->Kind() == MATERIAL_TYPE_METAL;}

    NODISCARD explicit DielectricMaterial() NOEXCEPT : Material(MATERIAL_TYPE_METAL), ior() {}
    NODISCARD explicit DielectricMaterial(double ior) NOEXCEPT
    : Material(MATERIAL_TYPE_METAL), ior(ior) {}

    double ior;

    NODISCARD Ray Scatter(const Ray &ray, const HitRecord &record) const NOEXCEPT OVERRIDE;
    NODISCARD Eigen::Vector3d Shading(const Ray& ray, const HitRecord& record, const Eigen::Vector3d& color) const NOEXCEPT OVERRIDE;
};

#endif //MATERIAL_H
