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
#include <Core/Image.h>
#include <Core/Scene.h>

struct Camera
{
    enum CameraType
    {
        CAMERA_TYPE_PERSPECTIVE,
    };

    CameraType type;
    double near;
    double far;
    double fovy;
    double aspect;
    Eigen::Vector3d origin;
    Eigen::Vector3d direction; // Normalized.
    Eigen::Vector3d right;     // Normalized.
    Eigen::Vector3d up;        // Normalized.
    Image film;

    NODISCARD static Camera FromXML(const char* filename) NOEXCEPT;

    void Render(const Scene& scene) NOEXCEPT;
};

#endif //CAMERA_H
