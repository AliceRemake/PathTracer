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
    Eigen::Vector3d u, v, w;

    NODISCARD explicit ONB(const Eigen::Vector3d& n) NOEXCEPT
    {
        w = n.normalized();
        v = w.cross(Flt(w.x(), 1.0) ? Eigen::Vector3d{1, 0, 0} : Eigen::Vector3d{0, 1, 0}).normalized();
        u = w.cross(v);
    }

    NODISCARD Eigen::Vector3d Transform(const Eigen::Vector3d& vec) const NOEXCEPT
    {
        return vec.x() * u + vec.y() * v + vec.z() * w;
    }
};

#endif //ONB_H
