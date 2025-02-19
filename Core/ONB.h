/**
  ******************************************************************************
  * @file           : ONB.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 25-2-7
  ******************************************************************************
  */



#ifndef ONB_H
#define ONB_H

#include <Core/Common.h>

struct ONB
{
    Eigen::Vector3d x, y, z;

    NODISCARD explicit ONB(const Eigen::Vector3d& direction) NOEXCEPT
    {
        x = direction;
        if (FIsZero((x - Eigen::Vector3d{0.0, 1.0, 0.0}).norm())) UNLIKELY
        {
            z = x.cross(Eigen::Vector3d{1.0, 0.0, 0.0}).normalized();
        }
        else
        {
            z = x.cross(Eigen::Vector3d{0.0, 1.0, 0.0}).normalized();
        }
        y = z.cross(x);
    }

    NODISCARD ONB(const Eigen::Vector3d& direction, const Eigen::Vector3d& up) NOEXCEPT
    {
        x = direction;
        ASSERT(!FIsZero((x - up).norm()));
        z = x.cross(up).normalized();
        y = z.cross(x);
    }

    NODISCARD Eigen::Vector3d Transform(const Eigen::Vector3d& vec) const NOEXCEPT
    {
        return vec.x() * x + vec.y() * y + vec.z() * z;
    }
};

#endif //ONB_H
