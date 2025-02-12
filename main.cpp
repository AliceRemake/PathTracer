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
    Ref<HittableList> scene = MakeRef<HittableList>();
    // scene.objects.emplace_back( // The Planet.
    //     MakeRef<Sphere>(
    //         MakeRef<LambertMaterial>(
    //             MakeRef<CheckerTexture2D>(
    //                 Eigen::Vector2d{8000, 2000},
    //                 Eigen::Vector3d{0.5, 0.5, 0.5},
    //                 Eigen::Vector3d{1.0, 1.0, 1.0}
    //             )
    //         ),
    //         Eigen::Vector3d{0.0, -1000, 0.0}, 1000.0
    //     )
    // );
    // for (Eigen::Index a = -11; a < 11; a++)
    // {
    //     for (Eigen::Index b = -11; b < 11; b++)
    //     {
    //         auto dist = RNG::UniformDist<double>(0, 1);
    //         auto dist1 = RNG::UniformDist<double>(0, 0.5);
    //         auto dist2 = RNG::UniformDist<double>(0.5, 1);
    //         Eigen::Vector3d center = Eigen::Vector3d{(double)a + 0.9 * RNG::Rand(dist), 0.2, (double)b + 0.9 * RNG::Rand(dist)};
    //
    //         if ((center - Eigen::Vector3d{4.0, 0.2, 0.0}).norm() > 0.9)
    //         {
    //             const double choose_mat = RNG::Rand(dist);
    //             Ref<Material> material = nullptr;
    //
    //             if (choose_mat < 0.8)
    //             {
    //                 // Diffuse
    //                 material = MakeRef<LambertMaterial>(
    //                     Eigen::Vector3d{RNG::Rand(dist), RNG::Rand(dist), RNG::Rand(dist)}.array() * Eigen::Vector3d{RNG::Rand(dist), RNG::Rand(dist), RNG::Rand(dist)}.array()
    //                 );
    //             }
    //             else if (choose_mat < 0.95)
    //             {
    //                 // Metal
    //                 material = MakeRef<MetalMaterial>(
    //                     Eigen::Vector3d{RNG::Rand(dist2), RNG::Rand(dist2), RNG::Rand(dist2)}, RNG::Rand(dist1)
    //                 );
    //             }
    //             else
    //             {
    //                 // Glass
    //                 material = MakeRef<DielectricMaterial>(1.5);
    //             }
    //
    //             scene.objects.emplace_back(MakeRef<Sphere>(material, center, 0.2));
    //         }
    //     }
    // }
    // scene.objects.emplace_back(
    //     MakeRef<Sphere>(
    //         MakeRef<DielectricMaterial>(1.5),
    //         Eigen::Vector3d{0.0, 1.0, 0.0}, 1.0
    //     )
    // );
    // scene.objects.emplace_back(
    //     MakeRef<Sphere>(
    //         MakeRef<LambertMaterial>(MakeRef<PureColorTexture2D>(Eigen::Vector3d{0.4, 0.2, 0.1})),
    //         Eigen::Vector3d{-4.0, 1.0, 0.0}, 1.0
    //     )
    // );
    // scene.objects.emplace_back(
    //     MakeRef<Sphere>(
    //         MakeRef<MetalMaterial>(MakeRef<PureColorTexture2D>(Eigen::Vector3d{0.7, 0.6, 0.5}), 0.0),
    //         Eigen::Vector3d{4.0, 1.0, 0.0}, 1.0
    //     )
    // );

    Ref<Image> earth_map = MakeRef<Image>(Image::From((FS::path(STR(CMAKE_SOURCE_DIR)) / "Input" / "earthmap.jpg").string().c_str()));

    scene->PushBack
    (
        MakeRef<Sphere>(
            MakeRef<LambertMaterial>(MakeRef<ImageTexture2D>(earth_map)),
            Eigen::Vector3d{0.0, 0.0, 0.0}, 0.8
        )
    );

    scene->InitializeBVH();
    // scene.objects.emplace_back(
    //     MakeRef<Sphere>(
    //         MakeRef<LambertMaterial>(MakeRef<CheckerTexture2D>(
    //             Eigen::Vector2d{20, 10},
    //             Eigen::Vector3d{0.5, 0.5, 0.5},
    //             Eigen::Vector3d{1.0, 1.0, 1.0}
    //         )),
    //         Eigen::Vector3d{0.0, 0.0, 0.0}, 0.8
    //     )
    // );

    // Camera camera
    // {
    //     .type = Camera::CAMERA_TYPE_PERSPECTIVE,
    //     .height = 900,
    //     .width = 1600,
    //     .near = 1.0,
    //     .far = 100.0,
    //     .fovy = ToRadians(75.0),
    //     .aspect = (double)800 / (double)600,
    //     .origin = Eigen::Vector3d{13.0, 2.0, 3.0},
    //     .direction = (Eigen::Vector3d{0.0, 0.0, 0.0} - Eigen::Vector3d{13.0, 2.0, 3.0}).normalized(),
    //     .right = Eigen::Vector3d{0.0, 0.0, 0.0},
    //     .up = Eigen::Vector3d{0.0, 0.0, 0.0},
    // };
    Camera camera
    {
        .type = Camera::CAMERA_TYPE_PERSPECTIVE,
        .height = 600,
        .width = 800,
        .near = 1.0,
        .far = 100.0,
        .fovy = ToRadians(75.0),
        .aspect = {},
        .origin = Eigen::Vector3d{0.0, 0.0, 2.0},
        .direction = (Eigen::Vector3d{0.0, 0.0, 0.0} - Eigen::Vector3d{0.0, 0.0, 2.0}).normalized(),
        .right = Eigen::Vector3d{0.0, 0.0, 0.0},
        .up = Eigen::Vector3d{0.0, 0.0, 0.0},
    };
    camera.aspect = (double)camera.width / (double)camera.height;
    camera.right = (camera.direction.cross(Eigen::Vector3d{0.0, 1.0, 0.0})).normalized();
    camera.up = camera.right.cross(camera.direction);

    CONSTEXPR RenderConfig config
    {
        .SPP = 64,
        .stop_prob = 0.02,
    };

    Image film;

    fmt::print("Rendering...\n");
    const auto st = Debug::Now();
    Renderer::Render(camera, scene, config, film);
    const auto ed = Debug::Now();
    fmt::print("Rendering Done! Time Escape: {} ms\n", Debug::MilliSeconds(ed - st));

    fmt::print("Writing To Disk {}\n", (FS::path(STR(CMAKE_SOURCE_DIR)) / "Output" / "Output.png").string().c_str());
    const bool result = Image::ToPNG(film, (FS::path(STR(CMAKE_SOURCE_DIR)) / "Output" / "Output.png").string().c_str());
    ASSERT(result);
    fmt::print("Writing To Disk Done!\n");

    return 0;
}
