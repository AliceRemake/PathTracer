/**
  ******************************************************************************
  * @file           : main.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-15
  ******************************************************************************
  */



#include <Core/Common.h>
#include <Core/Debug.h>
#include <Core/Camera.h>

int main()
{
    #ifndef NDEBUG
    Camera camera = Camera::From((FS::path(STR(CMAKE_SOURCE_DIR)) / "Scene" / "cornell-box" / "cornell-box.xml").string().c_str());
    fmt::printf("Camera Width : %d\n", camera.width);
    fmt::printf("Camera Height: %d\n", camera.height);
    fmt::printf("Camera Fovy  : %.3f\n", camera.fovy);
    fmt::printf("Camera Eye   : "); Debug::Dump(stdout, camera.eye);
    fmt::printf("Camera LookAt: "); Debug::Dump(stdout, camera.lookat);
    fmt::printf("Camera Up    : "); Debug::Dump(stdout, camera.up);
    #endif
    return 0;
}
