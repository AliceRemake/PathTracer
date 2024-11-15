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
#include <tinyxml2.h>

NODISCARD Camera Camera::From(const char* filename) NOEXCEPT
{
    tinyxml2::XMLDocument doc;
    
    switch (doc.LoadFile(filename))
    {
    case tinyxml2::XML_SUCCESS:
        break;
    default:
        ASSERT(false && "Error Reading XML File.");
    }

    // Read XML Test
    // const tinyxml2::XMLElement* camera = doc.FirstChildElement("camera");
    // fmt::printf("%s\n", camera->Name());
    // fmt::printf("%s\n", camera->Attribute("type"));
    // fmt::printf("%s\n", camera->Attribute("width"));
    // fmt::printf("%s\n", camera->Attribute("height"));
    // fmt::printf("%s\n", camera->Attribute("fovy"));
    //
    // const tinyxml2::XMLElement* eye = camera->FirstChildElement("eye");
    // fmt::printf("%s\n", eye->Name());
    // fmt::printf("%s\n", eye->Attribute("x"));
    // fmt::printf("%s\n", eye->Attribute("y"));
    // fmt::printf("%s\n", eye->Attribute("z"));
    //
    // const tinyxml2::XMLElement* lookat = camera->FirstChildElement("lookat");
    // fmt::printf("%s\n", lookat->Name());
    // fmt::printf("%s\n", lookat->Attribute("x"));
    // fmt::printf("%s\n", lookat->Attribute("y"));
    // fmt::printf("%s\n", lookat->Attribute("z"));
    //
    // const tinyxml2::XMLElement* up = camera->FirstChildElement("up");
    // fmt::printf("%s\n", up->Name());
    // fmt::printf("%s\n", up->Attribute("x"));
    // fmt::printf("%s\n", up->Attribute("y"));
    // fmt::printf("%s\n", up->Attribute("z"));
    //
    // fflush(stdout);

    const tinyxml2::XMLElement* camera = doc.FirstChildElement("camera");
    const tinyxml2::XMLElement* eye = camera->FirstChildElement("eye");
    const tinyxml2::XMLElement* lookat = camera->FirstChildElement("lookat");
    const tinyxml2::XMLElement* up = camera->FirstChildElement("up");

    ASSERT(strcmp(camera->Attribute("type"), "perspective") == 0 && "Unsupported Camera Type");
    
    return Camera
    {
        .type = CAMERA_TYPE_PERSPECTIVE,
        .width = (int)std::strtoll(camera->Attribute("width"), nullptr, 10),
        .height = (int)std::strtoll(camera->Attribute("height"), nullptr, 10),
        .fovy = (float)std::strtold(camera->Attribute("fovy"), nullptr),
        .eye = Eigen::Vector3f
        {
            (float)std::strtold(eye->Attribute("x"), nullptr),
            (float)std::strtold(eye->Attribute("y"), nullptr),
            (float)std::strtold(eye->Attribute("z"), nullptr),
        },
        .lookat = Eigen::Vector3f
        {
            (float)std::strtold(lookat->Attribute("x"), nullptr),
            (float)std::strtold(lookat->Attribute("y"), nullptr),
            (float)std::strtold(lookat->Attribute("z"), nullptr),
        },
        .up = Eigen::Vector3f
        {
            (float)std::strtold(up->Attribute("x"), nullptr),
            (float)std::strtold(up->Attribute("y"), nullptr),
            (float)std::strtold(up->Attribute("z"), nullptr),
        },
    };
}
  
