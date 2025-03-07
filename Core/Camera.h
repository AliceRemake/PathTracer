/**
  ******************************************************************************
  * @file           : Camera.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-15
  ******************************************************************************
  */



#ifndef CAMERA_H
#define CAMERA_H

#include <Core/Common.h>
#include <Core/RNG.h>
#include <Core/ONB.h>
#include <Core/Ray.h>
#include <Core/Texture.h>

struct Camera
{
    enum CameraType
    {
        CAMERA_TYPE_ORTHOGRAPHIC,
        CAMERA_TYPE_PERSPECTIVE,
    };

    CameraType type;
    Eigen::Index height;
    Eigen::Index width;
    double near;
    double far;
    double fovy;
    double aspect;
    Eigen::Vector3d origin;
    ONB onb;
    std::unordered_map<std::string, Ref<Texture2D<Eigen::Vector3d>>> lights;
    Eigen::Vector3d background;

public:
    NODISCARD Camera(
        CameraType type, const Eigen::Index height, const Eigen::Index width,
        const double near, const double far, const double fovy,
        const Eigen::Vector3d& origin, const Eigen::Vector3d& lookat,
        std::unordered_map<std::string, Ref<Texture2D<Eigen::Vector3d>>> lights,
        Eigen::Vector3d  background
    ) NOEXCEPT :
        type(type), height(height), width(width),
        near(near), far(far), fovy(fovy), aspect((double)width / (double)height),
        origin(origin), onb((lookat - origin).normalized()), lights(std::move(lights)), background(std::move(background))
    {}

    NODISCARD Camera(
        CameraType type, const Eigen::Index height, const Eigen::Index width,
        const double near, const double far, const double fovy,
        const Eigen::Vector3d& origin, const Eigen::Vector3d& lookat,
        const Eigen::Vector3d& up,
        std::unordered_map<std::string, Ref<Texture2D<Eigen::Vector3d>>> lights,
        Eigen::Vector3d  background
    ) NOEXCEPT :
        type(type), height(height), width(width),
        near(near), far(far), fovy(fovy), aspect((double)width / (double)height),
        origin(origin), onb((lookat - origin).normalized(), up), lights(std::move(lights)), background(std::move(background))
    {}

    NODISCARD static Camera FromXML(const char* filename) NOEXCEPT;

    // NODISCARD Ray SampleRay(const Eigen::Index row, const Eigen::Index col) const NOEXCEPT
    NODISCARD Ray SampleRay(const Eigen::Index row, const Eigen::Index col, const Eigen::Index jitter_size, const Eigen::Index jitter_row, const Eigen::Index jitter_col) const NOEXCEPT
    {
        const double near_height = near * std::tan(fovy / 2.0) * 2.0;
        const double near_width = near_height * aspect;
        const double pixel_size = near_height / (double)height;
        const double sub_pixel_size = pixel_size / (double)jitter_size;
        auto dist = RNG::UniformDist<double>(-sub_pixel_size / 2.0, sub_pixel_size / 2.0);
        const Eigen::Vector3d local_coord =
        {
            near,
            ((near_height - sub_pixel_size) / 2.0 - (double)row * pixel_size - (double)jitter_row * sub_pixel_size) + RNG::Rand(dist),
            ((sub_pixel_size - near_width) / 2.0 + (double)col * pixel_size + (double)jitter_col * sub_pixel_size) + RNG::Rand(dist),
        };
        return { origin, onb.Transform(local_coord).normalized() };
    }
};

#endif //CAMERA_H
