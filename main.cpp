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
#include <Core/Primitive.h>
#include <Core/Material.h>
#include <Core/Camera.h>
#include <Core/Image.h>
#include <Core/Renderer.h>
#include <Core/Debug.h>


int main()
{
    const Ref<HittableList> scene = MakeRef<HittableList>();

    Ref<Image> earth_map = MakeRef<Image>(Image::From((FS::path(STR(CMAKE_SOURCE_DIR)) / "Input" / "earthmap.jpg").string().c_str()));

    scene->PushBack
    (
        MakeRef<Sphere>(
            MakeRef<LambertMaterial>(MakeRef<ImageTexture2D>(earth_map)),
            Eigen::Vector3d{0.0, 0.0, 0.0}, 0.8
        )
    );

    fmt::print("Building BVH ...\n");
    auto st = Debug::Now();
    scene->InitializeBVH();
    auto ed = Debug::Now();
    fmt::print("Build BVH Done! Time Escape: {} ms\n", Debug::MicroSeconds(ed - st));

    const Camera camera(
        Camera::CAMERA_TYPE_PERSPECTIVE, 600, 800,
        1.0, 1000.0, ToRadians(75.0),
        Eigen::Vector3d{0.0, 0.0, 2.0},
        Eigen::Vector3d{0.0, 0.0, 0.0}
    );

    CONSTEXPR RenderConfig config
    {
        .SPP = 64,
        .stop_prob = 0.02,
    };

    Image film;

    fmt::print("Rendering...\n");
    st = Debug::Now();
    Renderer::Render(camera, scene, config, film);
    ed = Debug::Now();
    fmt::print("Rendering Done! Time Escape: {} ms\n", Debug::MilliSeconds(ed - st));

    fmt::print("Writing To Disk {}\n", (FS::path(STR(CMAKE_SOURCE_DIR)) / "Output" / "Output.png").string().c_str());
    const bool result = Image::ToPNG(film, (FS::path(STR(CMAKE_SOURCE_DIR)) / "Output" / "Output.png").string().c_str());
    ASSERT(result);
    fmt::print("Writing To Disk Done!\n");

    return 0;
}
