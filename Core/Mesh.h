/**
  ******************************************************************************
  * @file           : Mesh.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-30
  ******************************************************************************
  */



#ifndef MESH_H
#define MESH_H

#include <Core/Common.h>
#include <Core/Hittable.h>
#include <Core/Material.h>

// struct Mesh final : Hittable
// {
//     NODISCARD FORCE_INLINE static bool ClassOf(const Hittable* ptr) NOEXCEPT {return ptr->Kind() == HITTABLE_KIND_MESH;}
//
//     NODISCARD explicit Mesh() NOEXCEPT : Hittable(HITTABLE_KIND_MESH) {}
//
//     struct Index // Per Vertex.
//     {
//         Eigen::Index vertex;
//         Eigen::Index normal;
//         Eigen::Index texcoord;
//     };
//
//     struct Triangle // Three Vertex + One Material.
//     {
//         Index point[3];
//         Eigen::Index material;
//
//         NODISCARD bool Hit(const Mesh& mesh, const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT;
//     };
//
//     struct Shape
//     {
//         std::string name;
//         std::vector<Triangle> triangles;
//
//         NODISCARD bool Hit(const Mesh& mesh, const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT;
//     };
//
//     std::vector<Eigen::Vector3d> vertices;
//     std::vector<Eigen::Vector3d> normals;
//     std::vector<Eigen::Vector2d> texcoords;
//     std::vector<Ref<Material>> materials;
//
//     std::vector<Shape> shapes;
//
//     NODISCARD bool Hit(const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT OVERRIDE;
// };

#endif //MESH_H
