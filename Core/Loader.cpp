/**
  ******************************************************************************
  * @file           : Loader.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-27
  ******************************************************************************
  */



#include <Core/Loader.h>
#include <Core/Parallel.h>
#include <tiny_obj_loader.h>

NODISCARD Mesh Loader::LoadOBJ(const char* filename) NOEXCEPT
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
    Parallel::For(0, mesh.materials.size(), THREAD_POOL.ThreadNumber(),
        [&mesh, &materials](size_t thread_begin, size_t thread_end)
        {
            for (size_t i = thread_begin; i < thread_end; ++i)
            {
                const auto& t_material = materials[i];
                mesh.materials[i] = std::make_unique<Material>();
                const auto& material = mesh.materials[i];
                material->name = t_material.name;
                material->illum = t_material.illum;
                material->ns = t_material.shininess;
                material->ni = t_material.ior;
                material->ka.x() = t_material.ambient[0];
                material->ka.y() = t_material.ambient[1];
                material->ka.z() = t_material.ambient[2];
                material->kd.x() = t_material.diffuse[0];
                material->kd.y() = t_material.diffuse[1];
                material->kd.z() = t_material.diffuse[2];
                material->ks.x() = t_material.specular[0];
                material->ks.y() = t_material.specular[1];
                material->ks.z() = t_material.specular[2];
                material->tf.x() = t_material.transmittance[0];
                material->tf.y() = t_material.transmittance[1];
                material->tf.z() = t_material.transmittance[2];
                material->ke.x() = t_material.emission[0];
                material->ke.y() = t_material.emission[1];
                material->ke.z() = t_material.emission[2];
            }
        }
    );

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
                        auto& triangle_v = t_shape.triangles[i].v[k];
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
