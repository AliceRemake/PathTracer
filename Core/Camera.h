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
        CAMERA_TYPE_PERSPECTIVE,
    };

    CameraType type;
    int width;
    int height;
    float fovy;
    Eigen::Vector3f eye;
    Eigen::Vector3f lookat;
    Eigen::Vector3f up;

    NODISCARD static Camera From(const char* filename) NOEXCEPT;
};

#endif //CAMERA_H
