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
#include <Core/Texture.h>

struct Ray;

struct Material
{
protected:
    enum MaterialKind
    {
        MATERIAL_KIND_LAMBERT,
        MATERIAL_KIND_BLINNPHONG,
    };

    const MaterialKind kind;

public:
    NODISCARD CONSTEXPR FORCE_INLINE MaterialKind Kind() const NOEXCEPT { return kind; }

    NODISCARD explicit Material(const MaterialKind kind) NOEXCEPT : kind(kind) {}
    virtual ~Material() NOEXCEPT = default;

    NODISCARD virtual Eigen::Vector3d BRDF(
        const Eigen::Vector2d& texcoord2d,
        const Eigen::Vector3d& normal,
        const Eigen::Vector3d& omega_i,
        const Eigen::Vector3d& omega_o
    ) const NOEXCEPT = 0;
    NODISCARD virtual Eigen::Vector3d Emission(
        const Eigen::Vector2d& texcoord2d,
        const Eigen::Vector3d& normal,
        const Eigen::Vector3d& omega_o
    ) const NOEXCEPT = 0;
};

struct LambertMaterial final : Material
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Material* ptr) NOEXCEPT {return ptr->Kind() == MATERIAL_KIND_LAMBERT;}

    Ref<Texture2D<Eigen::Vector3d>> albedo_tex;
    Ref<Texture2D<Eigen::Vector3d>> emission_tex;

    NODISCARD LambertMaterial() NOEXCEPT
    : Material(MATERIAL_KIND_LAMBERT), albedo_tex(nullptr), emission_tex(nullptr)
    {}

    NODISCARD explicit LambertMaterial(
        const Ref<Texture2D<Eigen::Vector3d>>& albedo_tex,
        const Ref<Texture2D<Eigen::Vector3d>>& emission_tex
    ) NOEXCEPT
    : Material(MATERIAL_KIND_LAMBERT), albedo_tex(albedo_tex), emission_tex(emission_tex)
    {}

    NODISCARD Eigen::Vector3d BRDF(const Eigen::Vector2d& texcoord2d, const Eigen::Vector3d& normal, const Eigen::Vector3d& omega_i, const Eigen::Vector3d& omega_o) const NOEXCEPT OVERRIDE;
    NODISCARD Eigen::Vector3d Emission(const Eigen::Vector2d& texcoord2d, const Eigen::Vector3d& normal, const Eigen::Vector3d& omega_o) const NOEXCEPT OVERRIDE;
};

struct BlinnPhongMaterial final : Material
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Material* ptr) NOEXCEPT {return ptr->Kind() == MATERIAL_KIND_BLINNPHONG;}

    const double ns;
    Ref<Texture2D<Eigen::Vector3d>> ambient_tex;
    Ref<Texture2D<Eigen::Vector3d>> diffuse_tex;
    Ref<Texture2D<Eigen::Vector3d>> specular_tex;
    Ref<Texture2D<Eigen::Vector3d>> emission_tex;

    NODISCARD BlinnPhongMaterial() NOEXCEPT
    : Material(MATERIAL_KIND_BLINNPHONG), ns(2.0), ambient_tex(nullptr), diffuse_tex(nullptr), specular_tex(nullptr), emission_tex(nullptr)
    {}

    NODISCARD explicit BlinnPhongMaterial(
        const double ns,
        const Ref<Texture2D<Eigen::Vector3d>>& ambient_tex,
        const Ref<Texture2D<Eigen::Vector3d>>& diffuse_tex,
        const Ref<Texture2D<Eigen::Vector3d>>& specular_tex,
        const Ref<Texture2D<Eigen::Vector3d>>& emission_tex
    ) NOEXCEPT
    : Material(MATERIAL_KIND_BLINNPHONG), ns(ns), ambient_tex(ambient_tex), diffuse_tex(diffuse_tex), specular_tex(specular_tex), emission_tex(emission_tex)
    {}

    NODISCARD Eigen::Vector3d BRDF(const Eigen::Vector2d& texcoord2d, const Eigen::Vector3d& normal, const Eigen::Vector3d& omega_i, const Eigen::Vector3d& omega_o) const NOEXCEPT OVERRIDE;
    NODISCARD Eigen::Vector3d Emission(const Eigen::Vector2d& texcoord2d, const Eigen::Vector3d& normal, const Eigen::Vector3d& omega_o) const NOEXCEPT OVERRIDE;
};

#endif //MATERIAL_H
