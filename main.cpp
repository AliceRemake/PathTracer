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
#include <Core/Scene.h>
#include <Core/Camera.h>
#include <Core/Image.h>
#include <Core/Renderer.h>


int main()
{
    Scene scene;
    scene.objects.emplace_back( // A Sphere.
        MakeRef<Sphere>(
            Eigen::Vector3d{-0.5, 0.0, 0.0}, 0.5,
            MakeRef<BasicMaterial>(Eigen::Vector3d{0.5, 0.5, 0.5})
        )
    );
    scene.objects.emplace_back( // A Sphere.
        MakeRef<Sphere>(
            Eigen::Vector3d{0.5, 0.0, 0.0}, 0.5,
            MakeRef<LambertMaterial>(Eigen::Vector3d{0.5, 0.5, 0.5})
        )
    );
    scene.objects.emplace_back( // The Planet.
        MakeRef<Sphere>(
            Eigen::Vector3d{0.0, -1000.5, 0.0}, 1000.0,
            MakeRef<LambertMaterial>(Eigen::Vector3d{0.5, 0.5, 0.5})
        )
    );

    const Camera camera
    {
        .type = Camera::CAMERA_TYPE_PERSPECTIVE,
        .height = 600,
        .width = 800,
        .near = 1.0,
        .far = 100.0,
        .fovy = ToRadians(75.0),
        .aspect = (double)800 / (double)600,
        .origin = Eigen::Vector3d{0.0, 0.0, 2.0},
        .direction = Eigen::Vector3d{0.0, 0.0, -1.0},
        .right = Eigen::Vector3d{1.0, 0.0, 0.0},
        .up = Eigen::Vector3d{0.0, 1.0, 0.0},
    };

    CONSTEXPR RenderConfig config
    {
        .SPP = 64,
        .bounces = 10,
    };

    Image film;

    Renderer::Render(camera, scene, config, film);

    const bool result = Image::ToPNG(film, (FS::path(STR(CMAKE_SOURCE_DIR)) / "Output" / "Output.png").string().c_str());
    ASSERT(result);

    return 0;
}
