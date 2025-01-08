/**
  ******************************************************************************
  * @file           : Renderer.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-12-1
  ******************************************************************************
  */



#include <Core/Renderer.h>
#include <Core/RNG.h>
#include <Core/Parallel.h>
#include <Core/Image.h>
#include <Core/Camera.h>
#include <Core/Ray.h>

void Renderer::Render(const Camera& camera, const Scene& scene, const RenderConfig& config, Image& film) NOEXCEPT
{
    film.data.resize(camera.height, camera.width);

    const double height  = camera.near * std::tan(camera.fovy / 2.0) * 2.0;
    const double width = height * camera.aspect;

    const double pixel_size = width / (double)camera.width;
    const double spp_norm_factor = 1.0 / (double)config.SPP;
    const double start_x = -(width + pixel_size) / 2.0;
    const double start_z = (height - pixel_size) / 2.0;

    #ifdef ENABLE_MULTI_THREAD
    // TODO: Add Progress Bar
    Parallel::For(0, camera.height, THREAD_POOL.ThreadNumber(),
        [&film, &camera, &scene, &config, pixel_size, spp_norm_factor, start_x, start_z]
        (size_t thread_begin, size_t thread_end)
        {
            auto dist = RNG::UniformDist<double>(-pixel_size / 2, pixel_size / 2);

            double z = start_z - (double)thread_begin * pixel_size;
            for (Eigen::Index row = (Eigen::Index)thread_begin; row < (Eigen::Index)thread_end; ++row, z -= pixel_size)
            {
                double x = start_x;
                for (Eigen::Index col = 0; col < camera.width; ++col, x += pixel_size)
                {
                    Eigen::Vector3d color = Eigen::Vector3d{0.0, 0.0, 0.0};
                    for (size_t i = 0; i < config.SPP; ++i)
                    {
                        double sample_x = x + RNG::Rand(dist);
                        double sample_z = z + RNG::Rand(dist);
                        Ray sample_ray
                        {
                            .origin = camera.origin,
                            .direction = (sample_x * camera.right + camera.near * camera.direction + sample_z * camera.up).normalized(),
                        };
                        color += Ray::RayCast(sample_ray, scene, config.bounces);
                    }
                    color *= spp_norm_factor;
                    auto& pixel = film(row, col);
                    pixel.x() = color.x();
                    pixel.y() = color.y();
                    pixel.z() = color.z();
                    pixel.w() = 1.0;
                }
            }
        }
    );
    #else
    // TODO: Add Progress Bar
    auto dist = RNG::UniformDist<double>(-pixel_size / 2, pixel_size / 2);

    double z = start_z;
    for (Eigen::Index row = 0; row < (Eigen::Index)camera.height; ++row, z -= pixel_size)
    {
        double x = start_x;
        for (Eigen::Index col = 0; col < camera.width; ++col, x += pixel_size)
        {
            Eigen::Vector3d color = Eigen::Vector3d{0.0, 0.0, 0.0};
            for (size_t i = 0; i < config.SPP; ++i)
            {
                double sample_x = x + RNG::Rand(dist);
                double sample_z = z + RNG::Rand(dist);
                Ray sample_ray
                {
                    .origin = camera.origin,
                    .direction = (sample_x * camera.right + camera.near * camera.direction + sample_z * camera.up).normalized(),
                };
                color += Ray::RayCast(sample_ray, scene, config.bounces);
            }
            color *= spp_norm_factor;
            auto& pixel = film(row, col);
            pixel.x() = color.x();
            pixel.y() = color.y();
            pixel.z() = color.z();
            pixel.w() = 1.0;
        }
    }
    #endif
}
