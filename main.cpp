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
#include <Core/Loader.h>
#include <Core/Scene.h>
#include <Core/Camera.h>

int main()
{
    const Scene scene;
    // scene.objects.emplace_back(MakeRef<Mesh>(Loader::LoadOBJ((
    //     FS::path(STR(CMAKE_SOURCE_DIR)) / "Input" / "CornellBox" / "CornellBox-Original.obj"
    // ).string().c_str())));

    Camera camera
    {
        .type = Camera::CAMERA_TYPE_PERSPECTIVE,
        .near = 1.0,
        .far = 100.0,
        .fovy = ToRadians(75.0),
        .aspect = (double)800 / (double)600,
        .origin = Eigen::Vector3d{0.0, 0.0, 0.0},
        .direction = Eigen::Vector3d{0.0, 0.0, -1.0},
        .right = Eigen::Vector3d{1.0, 0.0, 0.0},
        .up = Eigen::Vector3d{0.0, 1.0, 0.0},
        .film = Image(600, 800),
    };

    camera.Render(scene);

    const bool result = Image::ToPNG
    (
        camera.film,
        (FS::path(STR(CMAKE_SOURCE_DIR)) / "Output" / "Output.png").string().c_str()
    );
    ASSERT(result);

    return 0;
}
