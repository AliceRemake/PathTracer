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

#include <utility>

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
    Eigen::Vector3d background;

public:
    NODISCARD Camera(
        CameraType type, const Eigen::Index height, const Eigen::Index width,
        const double near, const double far, const double fovy,
        const Eigen::Vector3d& origin, const Eigen::Vector3d& lookat,
        Eigen::Vector3d  background
    ) NOEXCEPT :
        type(type), height(height), width(width),
        near(near), far(far), fovy(fovy), aspect((double)width / (double)height),
        origin(origin), onb((lookat - origin).normalized()), background(std::move(background))
    {}

    NODISCARD Camera(
        CameraType type, const Eigen::Index height, const Eigen::Index width,
        const double near, const double far, const double fovy,
        const Eigen::Vector3d& origin, const Eigen::Vector3d& lookat,
        const Eigen::Vector3d& up,
        Eigen::Vector3d  background
    ) NOEXCEPT :
        type(type), height(height), width(width),
        near(near), far(far), fovy(fovy), aspect((double)width / (double)height),
        origin(origin), onb((lookat - origin).normalized(), up), background(std::move(background))
    {}

    NODISCARD static Camera FromXML(const char* filename) NOEXCEPT;

    NODISCARD Ray SampleRay(const Eigen::Index row, const Eigen::Index col) const NOEXCEPT
    {
        // TODO: add jitter
        const double near_height = near * std::tan(fovy / 2.0) * 2.0;
        const double near_width = near_height * aspect;
        const double pixel_size = near_height / (double)height;
        auto dist = RNG::UniformDist<double>(-pixel_size / 2.0, pixel_size / 2.0);
        const Eigen::Vector3d local_coord =
        {
            near,
            (near_height - pixel_size) / 2.0 - (double)row * pixel_size + RNG::Rand(dist),
            (pixel_size - near_width) / 2.0 + (double)col * pixel_size + RNG::Rand(dist),
        };
        return Ray
        {
            .origin = origin,
            .direction = onb.Transform(local_coord).normalized(),
        };
    }
};

#endif //CAMERA_H
