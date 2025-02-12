/**
  ******************************************************************************
  * @file           : Mesh.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-30
  ******************************************************************************
  */



#include <Core/Mesh.h>
#include <Core/Interval.h>
#include <Core/Ray.h>

// // Ref: https://iquilezles.org/articles/intersectors/
// NODISCARD bool Mesh::Triangle::Hit(const Mesh& mesh, const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT
// {
//     const Eigen::Vector3d v1v0 = mesh.vertices[point[1].vertex] - mesh.vertices[point[0].vertex];
//     const Eigen::Vector3d v2v0 = mesh.vertices[point[2].vertex] - mesh.vertices[point[0].vertex];
//     const Eigen::Vector3d rov0 = ray.origin - mesh.vertices[point[0].vertex];
//     const Eigen::Vector3d n = v1v0.cross(v2v0);
//     const Eigen::Vector3d q = rov0.cross(ray.direction);
//     const double d = 1.0 / ray.direction.dot(n);
//     const double u = -d * q.dot(v2v0);
//     const double v = d * q.dot(v1v0);
//     const double w = 1.0 - u - v;
//     const double t = -d * n.dot(rov0);
//     if (FIsNegative(u) || FIsNegative(v) || FIsNegative(w) || interval.Contain(t))
//     {
//         return false;
//     }
//     record.t = t;
//     record.material = mesh.materials[material];
//     record.hit_point = ray.At(t);
//     record.hit_normal = u * mesh.normals[point[0].normal] + v * mesh.normals[point[1].normal] + w * mesh.normals[point[2].normal];
//     return true;
// }
//
// NODISCARD bool Mesh::Shape::Hit(const Mesh& mesh, const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT
// {
//     record.t = INF;
//
//     for (const auto& triangle : triangles)
//     {
//         if (HitRecord t_record; triangle.Hit(mesh, ray, interval, t_record))
//         {
//             if (Fgt(record.t, t_record.t))
//             {
//                 record = t_record;
//             }
//         }
//     }
//
//     return !FIsInfinity(record.t);
// }
//
// NODISCARD bool Mesh::Hit(const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT
// {
//     record.t = INF;
//
//     for (const auto& shape : shapes)
//     {
//         if (HitRecord t_record; shape.Hit(*this, ray, interval, t_record))
//         {
//             if (Fgt(record.t, t_record.t))
//             {
//                 record = t_record;
//             }
//         }
//     }
//
//     return !FIsInfinity(record.t);
// }
