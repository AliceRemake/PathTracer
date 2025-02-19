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
#include <Core/RNG.h>
#include <Core/Material.h>
#include <Core/Camera.h>
#include <Core/Image.h>
#include <Core/Renderer.h>
#include <Core/Debug.h>


int main()
{
    const Ref<HittableList> scene = MakeRef<HittableList>();

    const auto red   = MakeRef<LambertMaterial>(MakeRef<PureColorTexture2D>(Eigen::Vector3d{.65, .05, .05}));
    const auto white = MakeRef<LambertMaterial>(MakeRef<PureColorTexture2D>(Eigen::Vector3d{.73, .73, .73}));
    const auto green = MakeRef<LambertMaterial>(MakeRef<PureColorTexture2D>(Eigen::Vector3d{.12, .45, .15}));
    const auto light = MakeRef<EmissiveMaterial>(
        MakeRef<PureColorTexture2D>(Eigen::Vector3d{15, 15, 15}),
        MakeRef<DiffuseMaterial>(MakeRef<PureColorTexture2D>(Eigen::Vector3d{1, 1, 1}))
    );

    scene->PushBack(MakeRef<Quadrangle>(
        green,
        Eigen::Vector3d{555.00,   0.00,   0.00},
        Eigen::Vector3d{  0.00,   0.00, 555.00},
        Eigen::Vector3d{  0.00, 555.00,   0.00}
    ));
    scene->PushBack(MakeRef<Quadrangle>(
        red,
        Eigen::Vector3d{0.00,   0.00,   0.00},
        Eigen::Vector3d{0.00, 555.00,   0.00},
        Eigen::Vector3d{0.00,   0.00, 555.00}
    ));
    scene->PushBack(MakeRef<Quadrangle>(light, Eigen::Vector3d(343, 554, 332), Eigen::Vector3d(0,0,-105), Eigen::Vector3d(-130,0,0)));
    scene->PushBack(MakeRef<Quadrangle>(white, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0,0,555), Eigen::Vector3d(555,0,0)));
    scene->PushBack(MakeRef<Quadrangle>(white, Eigen::Vector3d(555,555,555), Eigen::Vector3d(-555,0,0), Eigen::Vector3d(0,0,-555)));
    scene->PushBack(MakeRef<Quadrangle>(white, Eigen::Vector3d(0,0,555), Eigen::Vector3d(0,555,0), Eigen::Vector3d(555,0,0)));

    // {
    //     fmt::print("Building BVH ...\n");
    //     const auto st = Debug::Now();
    //     scene->InitializeBVH();
    //     const auto ed = Debug::Now();
    //     fmt::print("Build BVH Done! Time Escape: {} ms\n", Debug::MicroSeconds(ed - st));
    // }

    const Camera camera(
        Camera::CAMERA_TYPE_PERSPECTIVE, 600, 600,
        1.0, 1000.0, ToRadians(40.0),
        Eigen::Vector3d{278, 278, -800},
        Eigen::Vector3d{278, 278, 0},
        Eigen::Vector3d{0.0, 0.0, 0.0}
    );

    CONSTEXPR RenderConfig config
    {
        .SPP = 4096,
        .stop_prob = 0.02,
    };

    Image film;

    {
        fmt::print("Rendering...\n");
        const auto st = Debug::Now();
        Renderer::Render(camera, scene, config, film);
        const auto ed = Debug::Now();
        fmt::print("Render Done! Time Escape: {} ms\n", Debug::MilliSeconds(ed - st));
    }

    fmt::print("Writing To Disk {}\n", (FS::path(STR(CMAKE_SOURCE_DIR)) / "Output" / "Output.png").string().c_str());
    const bool result = Image::ToPNG(film, (FS::path(STR(CMAKE_SOURCE_DIR)) / "Output" / "Output.png").string().c_str());
    ASSERT(result);
    fmt::print("Write To Disk Done!\n");

    return 0;
}

Camera FinalSceneSeries1(const Ref<HittableList>& scene) NOEXCEPT
{
    scene->PushBack(
        MakeRef<Sphere>(
            MakeRef<LambertMaterial>(MakeRef<PureColorTexture2D>(Eigen::Vector3d{0.5, 0.5, 0.5})),
            Eigen::Vector3d{0.0, -1000.0, 0.0}, 1000.0
        )
    );

    scene->PushBack(
        MakeRef<Sphere>(
            MakeRef<DielectricMaterial>(1.5),
            Eigen::Vector3d{0.0, 1.0, 0.0}, 1.0
        )
    );

    scene->PushBack(
        MakeRef<Sphere>(
            MakeRef<LambertMaterial>(MakeRef<PureColorTexture2D>(Eigen::Vector3d{0.4, 0.2, 0.1})),
            Eigen::Vector3d{-4.0, 1.0, 0.0}, 1.0
        )
    );

    scene->PushBack(
        MakeRef<Sphere>(
            MakeRef<MetalMaterial>(MakeRef<PureColorTexture2D>(Eigen::Vector3d{0.7, 0.6, 0.5}), 0.0),
            Eigen::Vector3d{4.0, 1.0, 0.0}, 1.0
        )
    );

    for (int i = -11; i < 11; ++i)
    {
        for (int j = -11; j <= 11; ++j)
        {
            auto dist1 = RNG::UniformDist<double>(0.0, 1.0);
            auto dist2 = RNG::UniformDist<double>(0.0, 0.5);
            auto dist3 = RNG::UniformDist<double>(0.5, 1.0);

            const double choose_mat = RNG::Rand(dist1);
            const Eigen::Vector3d center = {i + 0.9 * RNG::Rand(dist1), 0.2, j + 0.9 * RNG::Rand(dist1)};

            if (choose_mat < 0.8)
            {
                scene->PushBack(
                    MakeRef<Sphere>(
                        MakeRef<LambertMaterial>(
                            MakeRef<PureColorTexture2D>(Eigen::Vector3d{RNG::Rand(dist1) * RNG::Rand(dist1), RNG::Rand(dist1) * RNG::Rand(dist1), RNG::Rand(dist1) * RNG::Rand(dist1)})
                        ), center, 0.2
                    )
                );
            }
            else if (choose_mat < 0.95)
            {
                scene->PushBack(
                    MakeRef<Sphere>(
                        MakeRef<MetalMaterial>(
                            MakeRef<PureColorTexture2D>(Eigen::Vector3d{RNG::Rand(dist3) * RNG::Rand(dist3), RNG::Rand(dist3) * RNG::Rand(dist3), RNG::Rand(dist3) * RNG::Rand(dist3)}), RNG::Rand(dist2)
                        ), center, 0.2
                    )
                );
            }
            else
            {
                scene->PushBack(
                    MakeRef<Sphere>(
                        MakeRef<DielectricMaterial>(1.5),
                        center, 0.2
                    )
                );
            }
        }
    }

    return {
        Camera::CAMERA_TYPE_PERSPECTIVE, 960, 1200,
        1.0, 1000.0, ToRadians(20.0),
        Eigen::Vector3d{13.00, 2.00, 3.00},
        Eigen::Vector3d{ 0.00, 0.00, 0.00},
        Eigen::Vector3d{ 0.70, 0.80, 1.00}
    };
}
