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
#include <Core/Debug.h>

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
    default: UNLIKELY
        FATAL("Error Parsing XML File.");
    }

    const tinyxml2::XMLElement* xml_camera = doc.FirstChildElement("camera");         ASSERT(xml_camera != nullptr);
    const tinyxml2::XMLElement* xml_eye = xml_camera->FirstChildElement("eye");       ASSERT(xml_eye != nullptr);
    const tinyxml2::XMLElement* xml_lookat = xml_camera->FirstChildElement("lookat"); ASSERT(xml_lookat != nullptr);
    const tinyxml2::XMLElement* xml_up = xml_camera->FirstChildElement("up");         ASSERT(xml_up != nullptr);
    const char* xml_height = xml_camera->Attribute("height");                         ASSERT(xml_height != nullptr);
    const char* xml_width = xml_camera->Attribute("width");                           ASSERT(xml_width != nullptr);
    const char* xml_fovy = xml_camera->Attribute("fovy");                             ASSERT(xml_fovy != nullptr);

    std::unordered_map<std::string, Ref<Texture2D<Eigen::Vector3d>>> lights;
    for (tinyxml2::XMLElement* e = doc.FirstChildElement("light"); e; e = e->NextSiblingElement("light"))
    {
        double x, y, z;
        const char* xml_radiance = e->Attribute("radiance");
        #ifdef _MSC_VER
        #pragma warning(push)
        #pragma warning(disable: 4996)
        #endif
        // ReSharper disable once CppDeprecatedEntity
        sscanf(xml_radiance, "%lf,%lf,%lf", &x, &y, &z);
        #ifdef _MSC_VER
        #pragma warning(pop)
        #endif
        lights[e->Attribute("mtlname")] = MakeRef<PureColorTexture2D>(Eigen::Vector3d{x, y, z});
    }

    // #ifndef NDEBUG
    // for (const auto &each: lights | std::views::values)
    // {
    //     Debug::Dump(stdout, each->Sample(Eigen::Vector2d{0.0, 0.0}));
    //     fflush(stdout);
    // }
    // #endif

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
    }.normalized();

    const Eigen::Index height = std::strtoll(xml_height, nullptr, 10);
    const Eigen::Index width = std::strtoll(xml_width, nullptr, 10);
    const double fovy = ToRadians((double)std::strtold(xml_fovy, nullptr));

    if (strcmp(xml_camera->Attribute("type"), "perspective") == 0) LIKELY
    {
        return {CAMERA_TYPE_PERSPECTIVE, height, width, 0.1, INF, fovy, eye, lookat, up, lights, Eigen::Vector3d{0.0, 0.0, 0.0}};
    }
    else if (strcmp(xml_camera->Attribute("type"), "orthographic") == 0)
    {
        return {CAMERA_TYPE_ORTHOGRAPHIC, height, width, 0.1, INF, fovy, eye, lookat, up, lights, Eigen::Vector3d{0.0, 0.0, 0.0}};
    }
    else UNLIKELY
    {
        FATAL("Unsupported Camera Type.");
    }
}
