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

// Ignore tinyxml2 Warnings.
#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wtype-limits"
#endif

#include <tinyxml2.h>

#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif

// NODISCARD Camera Camera::FromXML(const char* filename) NOEXCEPT
// {
//     tinyxml2::XMLDocument doc;
//
//     switch (doc.LoadFile(filename))
//     {
//     case tinyxml2::XML_SUCCESS:
//         break;
//     default: UNLIKELY
//         FATAL("Error Parsing XML File.");
//     }
//
//     const tinyxml2::XMLElement* xml_camera = doc.FirstChildElement("camera");         ASSERT(xml_camera != nullptr);
//     const tinyxml2::XMLElement* xml_eye = xml_camera->FirstChildElement("eye");       ASSERT(xml_eye != nullptr);
//     const tinyxml2::XMLElement* xml_lookat = xml_camera->FirstChildElement("lookat"); ASSERT(xml_lookat != nullptr);
//     const tinyxml2::XMLElement* xml_up = xml_camera->FirstChildElement("up");         ASSERT(xml_up != nullptr);
//     const char* xml_height = xml_camera->Attribute("height");                         ASSERT(xml_height != nullptr);
//     const char* xml_width = xml_camera->Attribute("width");                           ASSERT(xml_width != nullptr);
//
//     Eigen::Vector3d eye = Eigen::Vector3d
//     {
//         (double)std::strtold(xml_eye->Attribute("x"), nullptr),
//         (double)std::strtold(xml_eye->Attribute("y"), nullptr),
//         (double)std::strtold(xml_eye->Attribute("z"), nullptr),
//     };
//
//     Eigen::Vector3d lookat = Eigen::Vector3d
//     {
//         (double)std::strtold(xml_lookat->Attribute("x"), nullptr),
//         (double)std::strtold(xml_lookat->Attribute("y"), nullptr),
//         (double)std::strtold(xml_lookat->Attribute("z"), nullptr),
//     };
//
//     Eigen::Vector3d up = Eigen::Vector3d
//     {
//         (double)std::strtold(xml_up->Attribute("x"), nullptr),
//         (double)std::strtold(xml_up->Attribute("y"), nullptr),
//         (double)std::strtold(xml_up->Attribute("z"), nullptr),
//     };
//
//     const Eigen::Index height = std::strtoll(xml_height, nullptr, 10);
//     const Eigen::Index width = std::strtoll(xml_width, nullptr, 10);
//
//     Camera camera = {};
//     if (strcmp(xml_camera->Attribute("type"), "perspective") == 0) LIKELY
//     {
//         camera.type = CAMERA_TYPE_PERSPECTIVE;
//     }
//     else if (strcmp(xml_camera->Attribute("type"), "orthographic") == 0) LIKELY
//     {
//         camera.type = CAMERA_TYPE_ORTHOGRAPHIC;
//     }
//     else UNLIKELY
//     {
//         FATAL("Unsupported Camera Type.");
//     }
//     camera.height = height;
//     camera.width = width;
//     camera.near = 1.0;
//     camera.far = 100.0;
//     camera.fovy = ToRadians((double)std::strtold(xml_camera->Attribute("fovy"), nullptr));
//     camera.aspect = (double)width / (double)height;
//     camera.origin = eye;
//     camera.direction = (lookat - eye).normalized();
//     camera.right = camera.direction.cross(up).normalized();
//     camera.up = camera.right.cross(camera.direction).normalized();
//
//     return camera;
// }
