/**
  ******************************************************************************
  * @file           : Camera.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-15
  ******************************************************************************
  */



#include <Core/Camera.h>
#include <Core/RNG.h>
#include <Core/Parallel.h>
#include <Core/Ray.h>

// Ignore tinyxml2 Warnings.
#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wtype-limits"
#endif

#include <tinyxml2.h>

#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif

NODISCARD Camera Camera::FromXML(const char* filename) NOEXCEPT
{
    tinyxml2::XMLDocument doc;
    
    switch (doc.LoadFile(filename))
    {
    case tinyxml2::XML_SUCCESS:
        break;
    default:
        FATAL("Error Reading XML File.");
    }

    const tinyxml2::XMLElement* xml_camera = doc.FirstChildElement("camera");         ASSERT(xml_camera != nullptr);
    const tinyxml2::XMLElement* xml_eye = xml_camera->FirstChildElement("eye");       ASSERT(xml_eye != nullptr);
    const tinyxml2::XMLElement* xml_lookat = xml_camera->FirstChildElement("lookat"); ASSERT(xml_lookat != nullptr);
    const tinyxml2::XMLElement* xml_up = xml_camera->FirstChildElement("up");         ASSERT(xml_up != nullptr);
    const char* xml_height = xml_camera->Attribute("height");                         ASSERT(xml_height != nullptr);
    const char* xml_width = xml_camera->Attribute("width");                           ASSERT(xml_width != nullptr);
    ASSERT(strcmp(xml_camera->Attribute("type"), "perspective") == 0 && "Unsupported Camera Type");

    Eigen::Vector3d eye = Eigen::Vector3d
    {
        (double)std::strtold(xml_eye->Attribute("x"), nullptr),
        (double)std::strtold(xml_eye->Attribute("y"), nullptr),
        (double)std::strtold(xml_eye->Attribute("z"), nullptr),
    };

    Eigen::Vector3d lookat = Eigen::Vector3d
    {
        (double)std::strtold(xml_lookat->Attribute("x"), nullptr),
        (double)std::strtold(xml_lookat->Attribute("y"), nullptr),
        (double)std::strtold(xml_lookat->Attribute("z"), nullptr),
    };

    Eigen::Vector3d up = Eigen::Vector3d
    {
        (double)std::strtold(xml_up->Attribute("x"), nullptr),
        (double)std::strtold(xml_up->Attribute("y"), nullptr),
        (double)std::strtold(xml_up->Attribute("z"), nullptr),
    };

    const Eigen::Index height = std::strtoll(xml_height, nullptr, 10);
    const Eigen::Index width = std::strtoll(xml_width, nullptr, 10);

    Camera camera = {};
    camera.type = CAMERA_TYPE_PERSPECTIVE;
    camera.near = 1.0;
    camera.far = 100.0;
    camera.fovy = ToRadians((double)std::strtold(xml_camera->Attribute("fovy"), nullptr));
    camera.aspect = (double)width / (double)height;
    camera.origin = eye;
    camera.direction = (lookat - eye).normalized();
    camera.right = camera.direction.cross(up).normalized();
    camera.up = camera.right.cross(camera.direction).normalized();

    return camera;
}
  
void Camera::Render(const Scene& scene, const size_t spp) NOEXCEPT
{
    const double height  = near * std::tan(fovy / 2.0) * 2.0;
    const double width = height * aspect;

    const double pixel_width = width / (double)film.Width();
    const double normalization_factor = 1.0 / spp;
    const double start_x = -(width + pixel_width) / 2.0;
    const double start_z = (height - pixel_width) / 2.0;

    Parallel::For(0, film.Height(), THREAD_POOL.ThreadNumber(),
        [this, &scene, spp, pixel_width, normalization_factor, start_x, start_z](size_t thread_id, size_t thread_begin, size_t thread_end)
        {
            auto dist = RNG::UniformDist<double>(-pixel_width / 2, pixel_width / 2);
            RNG::Seed<double>(thread_id, 0.0);
            double z = start_z - (double)thread_begin * pixel_width;
            for (Eigen::Index row = (Eigen::Index)thread_begin; row < (Eigen::Index)thread_end; ++row, z -= pixel_width)
            {
                double x = start_x;
                for (Eigen::Index col = 0; col < film.Width(); ++col, x += pixel_width)
                {
                    Eigen::Vector4d color = Eigen::Vector4d{0.0, 0.0, 0.0, 0.0};
                    for (size_t i = 0; i < spp; ++i)
                    {
                        color += Ray::RayCast(Ray
                        {
                            .origin = origin,
                            .direction = ((x + RNG::Rand(thread_id, dist)) * right + near * direction + (z + RNG::Rand(thread_id, dist)) * up).normalized(),
                        }, scene);
                    }
                    film(row, col) = normalization_factor * color;
                }
            }
        }
    );
}
