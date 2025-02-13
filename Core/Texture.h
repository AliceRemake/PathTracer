/**
  ******************************************************************************
  * @file           : Texture.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 25-1-9
  ******************************************************************************
  */



#ifndef TEXTURE_H
#define TEXTURE_H

#include <Core/Common.h>
#include <Core/Image.h>

template<typename T>
struct Texture2D
{
    virtual ~Texture2D() NOEXCEPT = default;

    NODISCARD virtual T Sample(const Eigen::Vector2d& texcoord) const NOEXCEPT = 0;
};

template<typename T>
struct Texture3D
{
    virtual ~Texture3D() NOEXCEPT = default;

    NODISCARD virtual T Sample(const Eigen::Vector3d& texcoord) const NOEXCEPT = 0;
};

struct PureColorTexture2D final : Texture2D<Eigen::Vector3d>
{
    Eigen::Vector3d color;

    NODISCARD explicit PureColorTexture2D(Eigen::Vector3d color) NOEXCEPT
    : color(std::move(color))
    {}

    NODISCARD Eigen::Vector3d Sample(const Eigen::Vector2d &texcoord UNUSED) const NOEXCEPT OVERRIDE
    {
        return color;
    }
};

struct CheckerTexture2D final : Texture2D<Eigen::Vector3d>
{
    Eigen::Vector2d scale;
    Eigen::Vector3d odd_color;
    Eigen::Vector3d even_color;

    NODISCARD explicit CheckerTexture2D(Eigen::Vector2d scale, Eigen::Vector3d odd_color, Eigen::Vector3d even_color) NOEXCEPT
    : scale(std::move(scale)), odd_color(std::move(odd_color)), even_color(std::move(even_color))
    {}

    NODISCARD Eigen::Vector3d Sample(const Eigen::Vector2d &texcoord) const NOEXCEPT OVERRIDE
    {
        Eigen::Vector2d t = texcoord.array() * scale.array();
        return (Eigen::Index)t.array().floor().sum() % 2 ? odd_color : even_color;
    }
};

struct ImageTexture2D final : Texture2D<Eigen::Vector3d>
{
    Ref<Image> image;

    NODISCARD explicit ImageTexture2D(const Ref<Image>& image) NOEXCEPT
    : image(image)
    {}

    NODISCARD Eigen::Vector3d Sample(const Eigen::Vector2d &texcoord) const NOEXCEPT OVERRIDE
    {
        const Eigen::Index col = (Eigen::Index)std::floor(texcoord.x() * (double)image->Width()) % image->Width();
        const Eigen::Index row = (Eigen::Index)std::floor(texcoord.y() * (double)image->Height()) % image->Height();
        return (*image)(row, col).head(3);
    }
};

#endif //TEXTURE_H
