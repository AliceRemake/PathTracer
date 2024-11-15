/**
  ******************************************************************************
  * @file           : Debug.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-16
  ******************************************************************************
  */



#include <Core/Debug.h>

void Debug::Dump(FILE* fp, const Eigen::Vector3f& v) NOEXCEPT
{
    #ifdef NDEBUG
    (void)fp; (void)v;
    #else
    fmt::fprintf(fp, "(%10.3f, %10.3f, %10.3f)\n", v.x(), v.y(), v.z());
    fflush(fp);
    #endif
}
