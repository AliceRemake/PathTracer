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
    Eigen::Vector3d direction;
    Eigen::Vector3d right;
    Eigen::Vector3d up;

    NODISCARD static Camera FromXML(const char* filename) NOEXCEPT;
};

#endif //CAMERA_H
