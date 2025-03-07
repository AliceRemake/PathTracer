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
#include <Core/Camera.h>
#include <Core/Image.h>
#include <Core/Renderer.h>
#include <Core/Debug.h>
#include <Core/Mesh.h>
#include <Core/Primitive.h>
#include <Core/ThreadPool.h>


int main()
{
    // {
    //     fmt::print("Building BVH ...\n");
    //     const auto st = Debug::Now();
    //     scene->InitializeBVH();
    //     const auto ed = Debug::Now();
    //     fmt::print("Build BVH Done! Time Escape: {} ms\n", Debug::MicroSeconds(ed - st));
    // }

    const Ref<HittableList> scene = MakeRef<HittableList>();

    Image film;

    const RenderConfig config
    {
        .SPP = 128 * (Eigen::Index)THREAD_POOL.ThreadNumber(),
        .stop_prob = 0.02,
    };

    std::clog << "SPP: " << config.SPP << std::endl;

    // NOTE: Edit Scene And Camera Below.

    // Test Scene 0. Empty Cornell Box.
    const Camera camera(Camera::CAMERA_TYPE_PERSPECTIVE, 100, 100, 1.0, 1000.0, ToRadians(40.0),Eigen::Vector3d{278, 278, -800},Eigen::Vector3d{278, 278, 0},{},Eigen::Vector3d{0.0, 0.0, 0.0});

    // const auto red   = MakeRef<LambertMaterial>(MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.65, 0.05, 0.05 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.0, 0.0, 0.0}));
    // const auto white = MakeRef<LambertMaterial>(MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.73, 0.73, 0.73 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.0, 0.0, 0.0}));
    // const auto green = MakeRef<LambertMaterial>(MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.12, 0.45, 0.15 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.0, 0.0, 0.0}));
    // const auto light = MakeRef<LambertMaterial>(MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.00, 0.00, 0.00 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 15.0, 15.0, 15.0}));

    const auto red   = MakeRef<BlinnPhongMaterial>(2.0, MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.0, 0.0, 0.0 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.60, 0.00, 0.00 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.05, 0.05, 0.05 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.0, 0.0, 0.0 }));
    const auto white = MakeRef<BlinnPhongMaterial>(2.0, MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.0, 0.0, 0.0 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.23, 0.23, 0.23 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.50, 0.50, 0.50 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.0, 0.0, 0.0 }));
    const auto green = MakeRef<BlinnPhongMaterial>(2.0, MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.0, 0.0, 0.0 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.02, 0.35, 0.05 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.10, 0.10, 0.10 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.0, 0.0, 0.0 }));
    const auto light = MakeRef<BlinnPhongMaterial>(2.0, MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.0, 0.0, 0.0 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.50, 0.50, 0.50 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.00, 0.00, 0.00 }),MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 15.0, 15.0, 15.0 }));

    scene->PushBack(MakeRef<Quadrangle>(green,Eigen::Vector3d{555.00,   0.00,   0.00},Eigen::Vector3d{  0.00,   0.00, 555.00},Eigen::Vector3d{  0.00, 555.00,   0.00}));
    scene->PushBack(MakeRef<Quadrangle>(red,Eigen::Vector3d{0.00,   0.00,   0.00},Eigen::Vector3d{0.00, 555.00,   0.00},Eigen::Vector3d{0.00,   0.00, 555.00}));
    scene->PushBack(MakeRef<Quadrangle>(white,Eigen::Vector3d(0,0,0),Eigen::Vector3d(0,0,555),Eigen::Vector3d(555,0,0)));
    scene->PushBack(MakeRef<Quadrangle>(white,Eigen::Vector3d(555,555,555),Eigen::Vector3d(-555,0,0),Eigen::Vector3d(0,0,-555)));
    scene->PushBack(MakeRef<Quadrangle>(white,Eigen::Vector3d(0,0,555),Eigen::Vector3d(0,555,0),Eigen::Vector3d(555,0,0)));
    const auto t_light = MakeRef<Quadrangle>(light,Eigen::Vector3d(343, 554, 332),Eigen::Vector3d(-130,0,0),Eigen::Vector3d(0,0,-105));
    scene->PushBack(t_light);

    // Test Scene 1. Veach Mis.
    // const Camera camera = Camera::FromXML((FS::path(STR(CMAKE_SOURCE_DIR)) / "Input" / "veach-mis" / "veach-mis.xml").string().c_str());
    // const Ref<Mesh> mesh = MakeRef<Mesh>(Mesh::FromOBJ((FS::path(STR(CMAKE_SOURCE_DIR)) / "Input" / "veach-mis" / "veach-mis.obj").string().c_str(), camera.lights));
    // scene->PushBack(mesh);

    // Test Scene 2. Cornell Box.
    // const Camera camera = Camera::FromXML((FS::path(STR(CMAKE_SOURCE_DIR)) / "Input" / "cornell-box" / "cornell-box.xml").string().c_str());
    // const Ref<Mesh> mesh = MakeRef<Mesh>(Mesh::FromOBJ((FS::path(STR(CMAKE_SOURCE_DIR)) / "Input" / "cornell-box" / "cornell-box.obj").string().c_str(), camera.lights));
    // scene->PushBack(mesh);

    // Test Scene 3. Bathroom.
    // const Camera camera = Camera::FromXML((FS::path(STR(CMAKE_SOURCE_DIR)) / "Input" / "bathroom2" / "bathroom2.xml").string().c_str());
    // const Ref<Mesh> mesh = MakeRef<Mesh>(Mesh::FromOBJ((FS::path(STR(CMAKE_SOURCE_DIR)) / "Input" / "bathroom2" / "bathroom2.obj").string().c_str(), camera.lights));
    // scene->PushBack(mesh);

    {   // Render To Image
        fmt::print("Rendering...\n");
        const auto st = Debug::Now();
        Renderer::Render(camera, scene, {}, config, film);
        const auto ed = Debug::Now();
        fmt::print("Render Done! Time Escape: {} ms\n", Debug::MilliSeconds(ed - st));
    }

    {   // Write Image To Disk.
        fmt::print("Writing To Disk {}\n", (FS::path(STR(CMAKE_SOURCE_DIR)) / "Output" / "Output.png").string().c_str());
        const bool result = Image::ToPNG(film, (FS::path(STR(CMAKE_SOURCE_DIR)) / "Output" / "Output.png").string().c_str());
        ASSERT(result);
        fmt::print("Write To Disk Done!\n");
    }

    system("pause");

    return 0;
}
