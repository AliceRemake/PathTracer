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
#include <Core/Hittable.h>
#include <Core/Material.h>

NODISCARD Eigen::Vector3d Renderer::RayCast(const Ray& ray, const Ref<Hittable>& hittable, const Eigen::Index bounce, const double stop_prob) NOEXCEPT // NOLINT(*-no-recursion)
{
    HitRecord record;

    // Background.
    if (!hittable->Hit(ray, Interval{ 10 * EPS, INF }, record)) UNLIKELY
    {
        // TODO: Skybox.
        return Eigen::Vector3d{0.0, 0.0, 0.0};
    }

    if (bounce <= 3) UNLIKELY
    {
        // In General, Scatter The Ray, Trace The Scattered Ray, Shade The Color.
        const Ray scattered_ray = record.material->Scatter(ray, record);
        const Eigen::Vector3d color = RayCast(scattered_ray, hittable, bounce + 1, stop_prob);
        return record.material->Radiance(ray, record, color) + record.material->Emission(record.texcoord2d);
    }
    else LIKELY // Use Russian Roulette.
    {
        if (auto dist = RNG::UniformDist<double>(0, 1); RNG::Rand(dist) < stop_prob)
        {
            return Eigen::Vector3d{0.0, 0.0, 0.0};
        }
        else
        {
            const Ray scattered_ray = record.material->Scatter(ray, record);
            const Eigen::Vector3d color = RayCast(scattered_ray, hittable, bounce + 1, stop_prob);
            return record.material->Radiance(ray, record, (1 - stop_prob) * color) + record.material->Emission(record.texcoord2d);
        }
    }
}

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
                    color += RayCast(sample_ray, hittable, 0, config.stop_prob);
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
