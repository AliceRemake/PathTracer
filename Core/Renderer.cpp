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

#include <Core/Parallel.h>
#include <Core/Image.h>
#include <Core/Camera.h>

void Renderer::Render(const Camera& camera, const Ref<Hittable>& hittable, const RenderConfig& config, Image& film) NOEXCEPT
{
    film.data.resize(camera.height, camera.width);

    const double spp_norm_factor = 1.0 / (double)config.SPP;

    static std::vector<std::future<void>> futures(THREAD_POOL.ThreadNumber());

    for (Eigen::Index index = 0; index < film.Height() * film.Width();
#ifndef NDEBUG
        ++index
#endif
    )
    {
#ifdef NDEBUG
        Eigen::Index prev_index = index;
        for (Eigen::Index j = 0; index < film.Height() * film.Width() && j < (Eigen::Index)THREAD_POOL.ThreadNumber(); ++index, ++j)
        {
            futures[j] = THREAD_POOL.Submit([&film, &config, &camera, &hittable, index, spp_norm_factor]
            {
#endif
                const Eigen::Index row = index / film.Width();
                const Eigen::Index col = index % film.Width();
                Eigen::Vector3d color = Eigen::Vector3d{0.0, 0.0, 0.0};
                for (size_t i = 0; i < config.SPP; ++i)
                {
                    const Ray sample_ray = camera.SampleRay(row, col);
                    color += Ray::RayCast(sample_ray, hittable, 0);
                }
                color *= spp_norm_factor;
                auto& pixel = film(row, col);
                // pixel.head(3) = color; GNUC Bug, See: https://gitlab.com/libeigen/eigen/-/merge_requests/1820
                pixel.x() = color.x();
                pixel.y() = color.y();
                pixel.z() = color.z();
                pixel.w() = 1.0;
#ifdef NDEBUG
            });
        }
        for (Eigen::Index j = 0; prev_index < film.Height() * film.Width() && j < (Eigen::Index)THREAD_POOL.ThreadNumber(); ++prev_index, ++j)
        {
            futures[j].wait();
        }
#endif
    }
}
