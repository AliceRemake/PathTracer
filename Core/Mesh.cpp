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
#include <Core/Parallel.h>
#include <Core/Interval.h>
#include <Core/Ray.h>
#include <tiny_obj_loader.h>

// Ref: https://iquilezles.org/articles/intersectors/
NODISCARD bool Mesh::Triangle::Hit(const Mesh& mesh, const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT
{
    const Eigen::Vector3d& v1v0 = mesh.vertices[point[1].vertex] - mesh.vertices[point[0].vertex];
    const Eigen::Vector3d& v2v0 = mesh.vertices[point[2].vertex] - mesh.vertices[point[0].vertex];
    const Eigen::Vector3d rov0 = ray.origin - mesh.vertices[point[0].vertex];
    const Eigen::Vector3d n = v1v0.cross(v2v0);

    const double rdn = ray.direction.dot(n);

    if (FIsZero(rdn)) { return false; }

    const Eigen::Vector3d q = rov0.cross(ray.direction);
    const double d = 1.0 / rdn;
    const double u = d * -q.dot(v2v0);
    const double v = d * q.dot(v1v0);
    const double w = 1.0 - u - v;
    const double t = d * -n.dot(rov0);

    if (!interval.Contain(t)) { return false; }

    if (FIsNegative(u) || FIsNegative(v) || Fgt(u + v, 1.0)) { return false; }

    record.t = t;
    record.hit_point = ray.At(t);
    record.hit_normal = (u * mesh.normals[point[0].normal] + v * mesh.normals[point[1].normal] + w * mesh.normals[point[2].normal]).normalized();
    record.texcoord = Eigen::Vector2d{u, v};
    record.material = mesh.materials[material];

    return true;
}

NODISCARD bool Mesh::Shape::Hit(const Mesh& mesh, const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT
{
    record.t = INF;

    for (const auto& triangle : triangles)
    {
        if (HitRecord t_record; triangle.Hit(mesh, ray, interval, t_record))
        {
            if (Fgt(record.t, t_record.t))
            {
                record = t_record;
            }
        }
    }

    return !FIsInfinity(record.t);
}

NODISCARD bool Mesh::Hit(const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT
{
    record.t = INF;

    for (const auto& shape : shapes)
    {
        if (HitRecord t_record; shape.Hit(*this, ray, interval, t_record))
        {
            if (Fgt(record.t, t_record.t))
            {
                record = t_record;
            }
        }
    }

    return !FIsInfinity(record.t);
}

NODISCARD Mesh Mesh::FromOBJ(const char* filename, const std::unordered_map<std::string, Ref<Texture2D<Eigen::Vector3d>>>& lights) NOEXCEPT
{
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filename))
    {
        if (!reader.Error().empty())
        {
            fmt::fprintf(stderr, "%s\n", reader.Error().c_str()); fflush(stderr);
            exit(1);
        }
    }

    if (!reader.Warning().empty())
    {
        fmt::fprintf(stderr, "%s\n", reader.Warning().c_str()); fflush(stderr);
        exit(1);
    }

    Mesh mesh;

    const auto& attrib = reader.GetAttrib();

    ASSERT(attrib.vertices.size() % 3 == 0);
    mesh.vertices.resize(attrib.vertices.size() / 3);
    Parallel::For(0, mesh.vertices.size(), THREAD_POOL.ThreadNumber(),
        [&mesh, &attrib](size_t thread_begin, size_t thread_end)
        {
            for (size_t i = thread_begin, j = 3 * thread_begin; i < thread_end; ++i, j += 3)
            {
                auto& vertex = mesh.vertices[i];
                vertex.x() = attrib.vertices[j];
                vertex.y() = attrib.vertices[j + 1];
                vertex.z() = attrib.vertices[j + 2];
            }
        }
    );

    ASSERT(attrib.normals.size() % 3 == 0);
    mesh.normals.resize(attrib.normals.size() / 3);
    Parallel::For(0, mesh.normals.size(), THREAD_POOL.ThreadNumber(),
        [&mesh, &attrib](size_t thread_begin, size_t thread_end)
        {
            for (size_t i = thread_begin, j = 3 * thread_begin; i < thread_end; ++i, j += 3)
            {
                auto& normal = mesh.normals[i];
                normal.x() = attrib.normals[j];
                normal.y() = attrib.normals[j + 1];
                normal.z() = attrib.normals[j + 2];
            }
        }
    );

    ASSERT(attrib.texcoords.size() % 2 == 0);
    mesh.texcoords.resize(attrib.texcoords.size() / 2);
    Parallel::For(0, mesh.texcoords.size(), THREAD_POOL.ThreadNumber(),
        [&mesh, &attrib](size_t thread_begin, size_t thread_end)
        {
            for (size_t i = thread_begin, j = (thread_begin << 1); i < thread_end; ++i, j += 2)
            {
                auto& texcoord = mesh.texcoords[i];
                texcoord.x() = attrib.texcoords[j];
                texcoord.y() = attrib.texcoords[j + 1];
            }
        }
    );

    const auto& materials = reader.GetMaterials();
    mesh.materials.resize(materials.size());
    #ifdef NDEBUG
    Parallel::For(0, mesh.materials.size(), THREAD_POOL.ThreadNumber(), [&mesh, &materials, &lights](size_t thread_begin, size_t thread_end) {
        for (size_t i = thread_begin; i < thread_end; ++i)
    #else
        for (size_t i = 0; i < mesh.materials.size(); ++i)
    #endif
        {
            const auto& t_material = materials[i];
            Ref<Texture2D<Eigen::Vector3d>> ambient_tex = nullptr;
            Ref<Texture2D<Eigen::Vector3d>> diffuse_tex = nullptr;
            Ref<Texture2D<Eigen::Vector3d>> specular_tex = nullptr;
            Ref<Texture2D<Eigen::Vector3d>> emission_tex = nullptr;

            if (t_material.ambient_texname.empty())
            {
                ambient_tex = MakeRef<PureColorTexture2D>(Eigen::Vector3d{
                    t_material.ambient[0],
                    t_material.ambient[1],
                    t_material.ambient[2]
                });
            }
            else
            {
                ambient_tex = MakeRef<ImageTexture2D>(
                    MakeRef<Image>(Image::From(t_material.ambient_texname.c_str()))
                );
            }

            if (t_material.diffuse_texname.empty())
            {
                diffuse_tex = MakeRef<PureColorTexture2D>(Eigen::Vector3d{
                    t_material.diffuse[0],
                    t_material.diffuse[1],
                    t_material.diffuse[2]
                });
            }
            else
            {
                diffuse_tex = MakeRef<ImageTexture2D>(
                    MakeRef<Image>(Image::From(t_material.diffuse_texname.c_str()))
                );
            }

            if (t_material.specular_texname.empty())
            {
                specular_tex = MakeRef<PureColorTexture2D>(Eigen::Vector3d{
                    t_material.specular[0],
                    t_material.specular[1],
                    t_material.specular[2]
                });
            }
            else
            {
                specular_tex = MakeRef<ImageTexture2D>(
                    MakeRef<Image>(Image::From(t_material.specular_texname.c_str()))
                );
            }

            if (t_material.emissive_texname.empty())
            {
                emission_tex = MakeRef<PureColorTexture2D>(Eigen::Vector3d{
                    t_material.emission[0],
                    t_material.emission[1],
                    t_material.emission[2]
                });
            }
            else
            {
                emission_tex = MakeRef<ImageTexture2D>(
                    MakeRef<Image>(Image::From(t_material.emissive_texname.c_str()))
                );
            }

            if (t_material.name.starts_with("light"))
            {
                ambient_tex = MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.00, 0.00, 0.00 });
                diffuse_tex = MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.50, 0.50, 0.50 });
                specular_tex = MakeRef<PureColorTexture2D>(Eigen::Vector3d{ 0.00, 0.00, 0.00 });
                emission_tex = lights.find(t_material.name)->second;
            }

            mesh.materials[i] = MakeRef<BlinnPhongMaterial>(
                2.0, ambient_tex, diffuse_tex, specular_tex, emission_tex
            );
        }
    #ifdef NDEBUG
    });
    #endif

    const auto& shapes = reader.GetShapes();
    mesh.shapes.reserve(shapes.size());

    for (const auto& shape : shapes)
    {
        // Triangulation Enabled. Ignore Points. Ignore Lines.
        Mesh::Shape t_shape;
        t_shape.name = shape.name;

        ASSERT(shape.mesh.indices.size() % 3 == 0);
        ASSERT(shape.mesh.material_ids.size() == shape.mesh.indices.size() / 3);

        t_shape.triangles.resize(shape.mesh.indices.size() / 3);
        Parallel::For(0, t_shape.triangles.size(), THREAD_POOL.ThreadNumber(),
            [&t_shape, &shape](size_t thread_begin, size_t thread_end)
            {
                for (size_t i = thread_begin, j = 3 * thread_begin; i < thread_end; ++i, j+=3)
                {
                    for (size_t k = 0; k < 3; ++k)
                    {
                        auto& triangle_v = t_shape.triangles[i].point[k];
                        const auto& mesh_index = shape.mesh.indices[j + k];
                        triangle_v.vertex = mesh_index.vertex_index;
                        triangle_v.normal = mesh_index.normal_index;
                        triangle_v.texcoord = mesh_index.texcoord_index;
                    }
                    t_shape.triangles[i].material = shape.mesh.material_ids[i];
                }
            }
        );

        mesh.shapes.emplace_back(t_shape);
    }

    return mesh;
}
