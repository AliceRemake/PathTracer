/**
  ******************************************************************************
  * @file           : Material.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-27
  ******************************************************************************
  */



#ifndef MATERIAL_H
#define MATERIAL_H

#include <Core/Common.h>

// illum
// 0		Color on and Ambient off
// 1		Color on and Ambient on
// 2		Highlight on
// 3		Reflection on and Ray trace on
// 4		Transparency: Glass on
//          Reflection: Ray trace on
// 5		Reflection: Fresnel on and Ray trace on
// 6		Transparency: Refraction on
//          Reflection: Fresnel off and Ray trace on
// 7		Transparency: Refraction on
//          Reflection: Fresnel on and Ray trace on
// 8		Reflection on and Ray trace off
// 9		Transparency: Glass on
//          Reflection: Ray trace off
// 10		Casts shadows onto invisible surfaces
// Color
// Ambient
// Highlight
// Reflection
// Refraction
// Ray Trace
// Glass
// Fresnel
struct Material
{
	std::string name;
    unsigned illum;
    double ns; // Specular Highlights.
    double ni; // Index Of Refraction.
    Eigen::Vector3d ka; // Ambient.
    Eigen::Vector3d kd; // Diffuse.
    Eigen::Vector3d ks; // Specular.
    Eigen::Vector3d tf; // Transmission.
    Eigen::Vector3d ke; // Emission.

    // TODO Texture.
};

#endif //MATERIAL_H
