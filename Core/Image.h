/**
  ******************************************************************************
  * @file           : Image.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-18
  ******************************************************************************
  */



#ifndef IMAGE_H
#define IMAGE_H

#include <Core/Common.h>

struct Image
{
    Eigen::Matrix<Eigen::Vector4d, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> data;

    NODISCARD Image() NOEXCEPT = default;

    NODISCARD Image(const Eigen::Index height, const Eigen::Index width) NOEXCEPT
    {
        data.resize(height, width);
    }

    NODISCARD CONSTEXPR FORCE_INLINE Eigen::Index Height() const NOEXCEPT
    {
        return data.rows();
    }
    
    NODISCARD CONSTEXPR FORCE_INLINE Eigen::Index Width() const NOEXCEPT
    {
        return data.cols();
    }

    NODISCARD FORCE_INLINE const Eigen::Vector4d* Data() const NOEXCEPT
    {
        return data.array().data();
    }

    NODISCARD FORCE_INLINE Eigen::Vector4d* Data() NOEXCEPT
    {
        return data.array().data();
    }
    
    NODISCARD CONSTEXPR FORCE_INLINE const Eigen::Vector4d& operator()(const Eigen::Index row, const Eigen::Index col) const NOEXCEPT
    {
        return data(row, col);
    }
    
    NODISCARD FORCE_INLINE Eigen::Vector4d& operator()(const Eigen::Index row, const Eigen::Index col) NOEXCEPT
    {
        return data(row, col);
    }
    
    NODISCARD static Image From(const char* filename) NOEXCEPT;

    NODISCARD static bool ToPNG(const Image& image, const char* filename) NOEXCEPT;
};

#endif //IMAGE_H
