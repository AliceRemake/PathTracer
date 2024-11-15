/**
  ******************************************************************************
  * @file           : Debug.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-16
  ******************************************************************************
  */



#ifndef DEBUG_H
#define DEBUG_H

#include <Core/Common.h>

struct Debug {
    static void Dump(FILE* fp, const Eigen::Vector3f& v) NOEXCEPT;
};

#endif //DEBUG_H
