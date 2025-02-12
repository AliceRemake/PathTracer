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
#include <Core/Hittable.h>

void Renderer::Render(const Camera& camera, const Ref<Hittable>& hittable, const RenderConfig& config, Image& film) NOEXCEPT
{
    film.data.resize(camera.height, camera.width);

    const double height  = camera.near * std::tan(camera.fovy / 2.0) * 2.0;
    const double width = height * camera.aspect;

    const double pixel_size = width / (double)camera.width;
    const double spp_norm_factor = 1.0 / (double)config.SPP;
    const double start_x = -(width + pixel_size) / 2.0;
    const double start_z = (height - pixel_size) / 2.0;

    static std::vector<std::future<void>> futures(THREAD_POOL.ThreadNumber());

    for (Eigen::Index index = 0; index < film.Height() * film.Width();)
    {
        Eigen::Index prev_index = index;
        for (Eigen::Index j = 0; index < film.Height() * film.Width() && j < (Eigen::Index)THREAD_POOL.ThreadNumber(); ++index, ++j)
        {
            futures[j] = THREAD_POOL.Submit([&film, &config, &camera, &hittable, index, start_z, start_x, pixel_size, spp_norm_factor]
            {
                auto dist = RNG::UniformDist<double>(-pixel_size / 2, pixel_size / 2);
                const Eigen::Index row = index / film.Width();
                const Eigen::Index col = index % film.Width();
                const double z = start_z - (double)row * pixel_size;
                const double x = start_x + (double)col * pixel_size;
                Eigen::Vector3d color = Eigen::Vector3d{ 0.0, 0.0, 0.0 };
                for (size_t i = 0; i < config.SPP; ++i)
                {
                    double sample_x = x + RNG::Rand(dist);
                    double sample_z = z + RNG::Rand(dist);
                    Ray sample_ray
                    {
                        .origin = camera.origin,
                        .direction = (sample_x * camera.right + camera.near * camera.direction + sample_z * camera.up).normalized(),
                    };
                    color += Ray::RayCast(sample_ray, hittable, 0, config.stop_prob);
                }
                color *= spp_norm_factor;
                auto& pixel = film(row, col);
                // pixel.head(3) = color; GNUC Bug, See: https://gitlab.com/libeigen/eigen/-/merge_requests/1820
                pixel.x() = color.x();
                pixel.y() = color.y();
                pixel.z() = color.z();
                pixel.w() = 1.0;
            });
        }
        for (Eigen::Index j = 0; prev_index < film.Height() * film.Width() && j < (Eigen::Index)THREAD_POOL.ThreadNumber(); ++prev_index, ++j)
        {
            futures[j].wait();
        }
    }
    // for (Eigen::Index i = 0; i < film.Height() * film.Width(); ++i)
    // {
    //     futures[i].wait();
    // }

    // TODO: Add Progress Bar
    // Parallel::For(0, camera.height, THREAD_POOL.ThreadNumber(),
    //     [&film, &camera, &scene, &config, pixel_size, spp_norm_factor, start_x, start_z]
    //     (size_t thread_begin, size_t thread_end)
    //     {
    //         auto dist = RNG::UniformDist<double>(-pixel_size / 2, pixel_size / 2);
    //
    //         double z = start_z - (double)thread_begin * pixel_size;
    //         for (Eigen::Index row = (Eigen::Index)thread_begin; row < (Eigen::Index)thread_end; ++row, z -= pixel_size)
    //         {
    //             double x = start_x;
    //             for (Eigen::Index col = 0; col < camera.width; ++col, x += pixel_size)
    //             {
    //                 Eigen::Vector3d color = Eigen::Vector3d{0.0, 0.0, 0.0};
    //                 for (size_t i = 0; i < config.SPP; ++i)
    //                 {
    //                     double sample_x = x + RNG::Rand(dist);
    //                     double sample_z = z + RNG::Rand(dist);
    //                     Ray sample_ray
    //                     {
    //                         .origin = camera.origin,
    //                         .direction = (sample_x * camera.right + camera.near * camera.direction + sample_z * camera.up).normalized(),
    //                     };
    //                     color += Ray::RayCast(sample_ray, scene, 0, config.stop_prob);
    //                 }
    //                 color *= spp_norm_factor;
    //                 auto& pixel = film(row, col);
    //                 pixel.x() = color.x();
    //                 pixel.y() = color.y();
    //                 pixel.z() = color.z();
    //                 pixel.w() = 1.0;
    //             }
    //         }
    //     }
    // );
}
