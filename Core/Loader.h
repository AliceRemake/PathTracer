/**
  ******************************************************************************
  * @file           : Loader.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-27
  ******************************************************************************
  */



#ifndef LOADER_H
#define LOADER_H

#include <Core/Common.h>
#include <Core/Mesh.h>

struct Loader
{
    NODISCARD static Mesh LoadOBJ(const char* filename) NOEXCEPT;
};

#endif //LOADER_H
