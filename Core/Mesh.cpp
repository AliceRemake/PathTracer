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
#include <Core/Ray.h>

// Ref: https://iquilezles.org/articles/intersectors/
NODISCARD bool Mesh::Triangle::Hit(const Mesh& mesh, const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT
{
    Debug::Unuse(mesh); Debug::Unuse(ray); Debug::Unuse(interval); Debug::Unuse(record);
    return true;
    // const Eigen::Vector3d v1v0 = mesh.vertices[v1.vertex] - mesh.vertices[v0.vertex];
    // const Eigen::Vector3d v2v0 = mesh.vertices[v2.vertex] - mesh.vertices[v0.vertex];
    // const Eigen::Vector3d rov0 = ray.origin - mesh.vertices[v0.vertex];
    // const Eigen::Vector3d n = v1v0.cross(v2v0);
    // const Eigen::Vector3d q = rov0.cross(ray.direction);
    // const double d = 1.0 / ray.direction.dot(n);
    // const double u = -d * q.dot(v2v0);
    // const double v = d * q.dot(v1v0);
    // const double w = 1.0 - u - v;
    // const double t = -d * n.dot(rov0);
    // if (FIsNegative(u) || FIsNegative(v) || Fgt(u + v, 1.0))
    // {
    //     return false;
    // }
    // record.t = t;
    // record.hit_normal = u * mesh.normals[v0.normal] + v * mesh.normals[v1.normal] + (1.0 - u - v) * mesh.normals[v2.normal];
    // return true;
}

NODISCARD bool Mesh::Shape::Hit(const Mesh& mesh, const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT
{
    Debug::Unuse(mesh); Debug::Unuse(ray); Debug::Unuse(interval); Debug::Unuse(record);
    return true;
    // for (const auto& triangle : triangles)
    // {
    //     if (triangle.Hit(mesh, ray, interval, record))
    //     {
    //
    //     }
    // }
}

NODISCARD bool Mesh::Hit(const Ray &ray, const Interval& interval, HitRecord &record) NOEXCEPT
{
    Debug::Unuse(ray); Debug::Unuse(interval); Debug::Unuse(record);
    // for (const auto& shape : shapes)
    // {
    //     for (const auto& triangle : shape.triangles)
    //     {
    //
    //     }
    // }
    return true;
}
