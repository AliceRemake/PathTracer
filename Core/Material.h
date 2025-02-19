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

#include "Core/Texture.h"

struct Ray;

struct Material
{
protected:
    enum MaterialKind
    {
        MATERIAL_KIND_DIFFUSE,
        MATERIAL_KIND_LAMBERT,
        MATERIAL_KIND_METAL,
        MATERIAL_KIND_DIELECTRIC,
        MATERIAL_KIND_EMISSIVE,
    };

    const MaterialKind kind;

public:
    NODISCARD CONSTEXPR FORCE_INLINE MaterialKind Kind() const NOEXCEPT { return kind; }

    NODISCARD explicit Material(const MaterialKind kind) NOEXCEPT : kind(kind) {}
    virtual ~Material() NOEXCEPT = default;

    NODISCARD virtual Ray Scatter(const Ray& ray, HitRecord& record) const NOEXCEPT = 0;

    NODISCARD virtual Eigen::Vector3d Emission(const Eigen::Vector2d& texcoord2d UNUSED) const NOEXCEPT { return {0, 0, 0}; }
    NODISCARD virtual Eigen::Vector3d Radiance(const Ray& ray, const HitRecord& record, const Eigen::Vector3d& color) const NOEXCEPT = 0;

    // Helper Functions
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

struct DiffuseMaterial final : Material
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Material* ptr) NOEXCEPT {return ptr->Kind() == MATERIAL_KIND_DIFFUSE;}

    Ref<Texture2D<Eigen::Vector3d>> color_tex;

    NODISCARD DiffuseMaterial() NOEXCEPT
    : Material(MATERIAL_KIND_DIFFUSE), color_tex(nullptr)
    {}

    NODISCARD explicit DiffuseMaterial(const Ref<Texture2D<Eigen::Vector3d>>& color_tex) NOEXCEPT
    : Material(MATERIAL_KIND_DIFFUSE), color_tex(color_tex)
    {}

    NODISCARD Ray Scatter(const Ray &ray, HitRecord &record) const NOEXCEPT OVERRIDE;
    NODISCARD Eigen::Vector3d Radiance(const Ray& ray, const HitRecord& record, const Eigen::Vector3d& color) const NOEXCEPT OVERRIDE;
};

struct LambertMaterial final : Material
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Material* ptr) NOEXCEPT {return ptr->Kind() == MATERIAL_KIND_LAMBERT;}

    Ref<Texture2D<Eigen::Vector3d>> color_tex;

    NODISCARD LambertMaterial() NOEXCEPT
    : Material(MATERIAL_KIND_LAMBERT), color_tex(nullptr)
    {}

    NODISCARD explicit LambertMaterial(const Ref<Texture2D<Eigen::Vector3d>>& color_tex) NOEXCEPT
    : Material(MATERIAL_KIND_LAMBERT), color_tex(color_tex)
    {}

    NODISCARD Ray Scatter(const Ray &ray, HitRecord &record) const NOEXCEPT OVERRIDE;
    NODISCARD Eigen::Vector3d Radiance(const Ray& ray, const HitRecord& record, const Eigen::Vector3d& color) const NOEXCEPT OVERRIDE;
};

struct MetalMaterial final : Material
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Material* ptr) NOEXCEPT {return ptr->Kind() == MATERIAL_KIND_METAL;}

    Ref<Texture2D<Eigen::Vector3d>> color_tex;
    double fuzziness;

    NODISCARD MetalMaterial() NOEXCEPT
    : Material(MATERIAL_KIND_METAL), color_tex(nullptr), fuzziness(0)
    {}

    NODISCARD explicit MetalMaterial(const Ref<Texture2D<Eigen::Vector3d>>& color_tex, double fuzziness) NOEXCEPT
    : Material(MATERIAL_KIND_METAL), color_tex(color_tex), fuzziness(fuzziness)
    {}

    NODISCARD Ray Scatter(const Ray &ray, HitRecord &record) const NOEXCEPT OVERRIDE;
    NODISCARD Eigen::Vector3d Radiance(const Ray& ray, const HitRecord& record, const Eigen::Vector3d& color) const NOEXCEPT OVERRIDE;
};

struct DielectricMaterial final : Material
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Material* ptr) NOEXCEPT {return ptr->Kind() == MATERIAL_KIND_DIELECTRIC;}

    double ior;

    NODISCARD DielectricMaterial() NOEXCEPT
    : Material(MATERIAL_KIND_DIELECTRIC), ior(1.5)
    {}

    NODISCARD explicit DielectricMaterial(const double ior) NOEXCEPT
    : Material(MATERIAL_KIND_METAL), ior(ior)
    {}

    NODISCARD Ray Scatter(const Ray &ray, HitRecord &record) const NOEXCEPT OVERRIDE;
    NODISCARD Eigen::Vector3d Radiance(const Ray& ray, const HitRecord& record, const Eigen::Vector3d& color) const NOEXCEPT OVERRIDE;
};

struct EmissiveMaterial final : Material
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Material* ptr) NOEXCEPT {return ptr->Kind() == MATERIAL_KIND_EMISSIVE;}

    Ref<Texture2D<Eigen::Vector3d>> color_tex;
    Ref<Material> ref;

    NODISCARD EmissiveMaterial() NOEXCEPT
    : Material(MATERIAL_KIND_EMISSIVE), color_tex(nullptr)
    {}

    NODISCARD explicit EmissiveMaterial(const Ref<Texture2D<Eigen::Vector3d>>& color_tex, const Ref<Material>& ref) NOEXCEPT
    : Material(MATERIAL_KIND_METAL), color_tex(color_tex), ref(ref)
    {}

    NODISCARD Ray Scatter(const Ray &ray, HitRecord &record) const NOEXCEPT OVERRIDE;
    NODISCARD Eigen::Vector3d Radiance(const Ray& ray, const HitRecord& record, const Eigen::Vector3d& color) const NOEXCEPT OVERRIDE;
    NODISCARD Eigen::Vector3d Emission(const Eigen::Vector2d &texcoord2d) const NOEXCEPT OVERRIDE
    {
        return color_tex->Sample(texcoord2d);
    }
};

// struct PhongMaterial final : Material
// {
//     NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Material* ptr) NOEXCEPT {return ptr->Kind() == MATERIAL_TYPE_PHONG;}
//
//     Eigen::Vector3d kd;
//     Eigen::Vector3d ks;
//     Eigen::Vector3d tr;
//     double ns;
//     double ni;
//
//     NODISCARD PhongMaterial() NOEXCEPT : Material(MATERIAL_TYPE_PHONG), ns(1.0), ni(1.0) {}
//     NODISCARD explicit PhongMaterial(Eigen::Vector3d kd, Eigen::Vector3d ks, Eigen::Vector3d tr, const double ns, const double ni) NOEXCEPT
//     : Material(MATERIAL_TYPE_PHONG), kd(std::move(kd)), ks(std::move(ks)), tr(std::move(tr)), ns(ns), ni(ni) {}
//
//     NODISCARD Ray Scatter(const Ray &ray, HitRecord &record) const NOEXCEPT OVERRIDE;
//     NODISCARD Eigen::Vector3d Shading(const Ray& ray, const HitRecord& record, const Eigen::Vector3d& color) const NOEXCEPT OVERRIDE;
// };

#endif //MATERIAL_H
