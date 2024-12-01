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
#include <Core/Primitive.h>
#include <Core/Scene.h>
#include <Core/Camera.h>


int main()
{
    Scene scene;
    scene.objects.emplace_back(
        MakeRef<Sphere>(
            Eigen::Vector3d{0.0, 0.0, 0.0}, 1.0,
            MakeRef<Material>(Material
            {
                .name = "Sphere Material",
                .illum = 2,
                .ns = 96.078431,
                .ni = 1.0,
                .ka = Eigen::Vector3d{1.0, 1.0, 1.0},
                .kd = Eigen::Vector3d{0.64, 0.64, 0.64},
                .ks = Eigen::Vector3d{0.5, 0.5, 0.5},
                .tf = Eigen::Vector3d{0.0, 0.0, 0.0},
                .ke = Eigen::Vector3d{0.0, 0.0, 0.0},
            })
        )
    );
    scene.objects.emplace_back(
        MakeRef<Sphere>(
            Eigen::Vector3d{0.0, -1001.0, 0.0}, 1000.0,
            MakeRef<Material>(Material
            {
                .name = "World Material",
                .illum = 2,
                .ns = 96.078431,
                .ni = 1.0,
                .ka = Eigen::Vector3d{1.0, 1.0, 1.0},
                .kd = Eigen::Vector3d{0.64, 0.64, 0.64},
                .ks = Eigen::Vector3d{0.5, 0.5, 0.5},
                .tf = Eigen::Vector3d{0.0, 0.0, 0.0},
                .ke = Eigen::Vector3d{0.0, 0.0, 0.0},
            })
        )
    );
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
        .origin = Eigen::Vector3d{0.0, 0.0, 2.0},
        .direction = Eigen::Vector3d{0.0, 0.0, -1.0},
        .right = Eigen::Vector3d{1.0, 0.0, 0.0},
        .up = Eigen::Vector3d{0.0, 1.0, 0.0},
        .film = Image(600, 800),
    };

    camera.Render(scene, 32);

    const bool result = Image::ToPNG
    (
        camera.film,
        (FS::path(STR(CMAKE_SOURCE_DIR)) / "Output" / "Output.png").string().c_str()
    );
    ASSERT(result);

    return 0;
}
