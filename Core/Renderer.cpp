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
#include <Core/Sampler.h>

NODISCARD Eigen::Vector3d Renderer::RayCast(const Ray& ray, const Ref<Hittable>& hittable, const std::vector<Ref<Hittable>>& lights, const Eigen::Index bounce, const double stop_prob) NOEXCEPT // NOLINT(*-no-recursion)
{
    HitRecord record;

    if (!hittable->Hit(ray, Interval{ EPS, INF }, record))
    {
        return Eigen::Vector3d{ 0.0, 0.0, 0.0 };
    }

    auto dist = RNG::UniformDist<double>(0.0, 1.0);
    const Eigen::Vector3d& omega_o = -ray.direction;
    const Eigen::Vector3d& origin = record.hit_point;
    const Eigen::Vector3d& normal = record.hit_normal;
    const Eigen::Vector2d& texcoord = record.texcoord;
    const Ref<Material>& material = record.material;

    Eigen::Vector3d Lo = material->Emission(texcoord, normal, omega_o);

    // NOTE: Emission: [min_bounce = 0] Direct: [min_bounce = 1] Indirect: [min_bounce = 2]
    CONSTEXPR Eigen::Index min_bounce = 8;
    if (const double rng = RNG::Rand(dist); bounce < min_bounce || rng >= stop_prob) UNLIKELY
    {
        Ref<Sampler> sampler = nullptr;

        if (DynCast<LambertMaterial>(material.get()))
        {
            sampler = MakeRef<CosineHemiSphereSampler>();
        }
        else if (const BlinnPhongMaterial* mat = DynCast<BlinnPhongMaterial>(material.get()); mat != nullptr)
        {
            const Ref<Sampler> diffuse_sampler = MakeRef<CosineHemiSphereSampler>();
            const Ref<Sampler> specular_sampler = MakeRef<BlinnPhongSpecularSampler>(mat->ns);
            sampler = MakeRef<MixSampler>(
                std::vector<Ref<Sampler>>{diffuse_sampler, specular_sampler},
                std::vector<double>{mat->diffuse_tex->Sample(texcoord).norm(), mat->specular_tex->Sample(texcoord).norm()}
            );
        }
        else
        {
            ASSERT(false);
        }

        // sampler = MakeRef<MixSampler>(
        //     std::vector<Ref<Sampler>>{sampler, MakeRef<LightImportanceSampler>(lights)},
        //     std::vector<double>{0.5, 0.5}
        // );

        const Eigen::Vector3d omega_i = sampler->Sample(origin, texcoord, normal, omega_o);
        const double pdf = sampler->PDF(origin, texcoord, normal, omega_i, omega_o);

        Eigen::Vector3d Li = RayCast(Ray(record.hit_point, omega_i), hittable, lights, bounce + 1, stop_prob);
        Eigen::Vector3d fr = material->BRDF(texcoord, normal, omega_i, omega_o);
        Lo.array() += (Li.array() * fr.array() * normal.dot(omega_i)) / (bounce < min_bounce ? pdf : pdf * (1.0 - stop_prob));
    }

    return Lo;
}

void Renderer::Render(const Camera& camera, const Ref<Hittable>& scene, const std::vector<Ref<Hittable>>& lights, const RenderConfig& config, Image& film) NOEXCEPT
{
    film.data.resize(camera.height, camera.width);

    RNG::Seed(std::random_device()());
    auto dist = RNG::UniformDist<unsigned int>(0, std::numeric_limits<unsigned int>::max());

    const Eigen::Index jitter_size = (Eigen::Index)SSqrt((double)config.SPP);
    #ifdef NDEBUG
    static std::vector<std::future<Eigen::Vector3d>> futures(jitter_size * jitter_size);
    #endif
    const double spp_norm_factor = 1.0 / (double)(jitter_size * jitter_size);

    for (Eigen::Index row = 0, progress = 0, acc = 0; row < film.Height(); ++row)
    {
        for (Eigen::Index col = 0; col < film.Width(); ++col, ++progress, (++acc) %= 3)
        {
            Eigen::Vector3d color = Eigen::Vector3d{ 0.0, 0.0, 0.0 };

            for (Eigen::Index jitter_row = 0, jitter_index = 0; jitter_row < jitter_size; ++jitter_row)
            {
                for (Eigen::Index jitter_col = 0; jitter_col < jitter_size; ++jitter_col, ++jitter_index)
                {
                    #ifdef NDEBUG
                    futures[jitter_index] = THREAD_POOL.Submit([&camera, &scene, &lights, &config, row, col, jitter_size, jitter_row, jitter_col](const unsigned int seed) -> Eigen::Vector3d
                    {
                        RNG::Seed(seed);
                        const Ray sample_ray = camera.SampleRay(row, col, jitter_size, jitter_row, jitter_col);
                        return RayCast(sample_ray, scene, lights, 0, config.stop_prob);
                    }, RNG::Rand(dist));
                    #else
                    color += [&camera, &scene, &lights, &config, row, col, jitter_size, jitter_row, jitter_col](const unsigned int seed) -> Eigen::Vector3d
                    {
                        RNG::Seed(seed);
                        const Ray sample_ray = camera.SampleRay(row, col, jitter_size, jitter_row, jitter_col);
                        return RayCast(sample_ray, scene, lights, 0, config.stop_prob);
                    }(RNG::Rand(dist));
                    #endif
                }
            }

            #ifdef NDEBUG
            for (Eigen::Index i = 0; i < jitter_size * jitter_size; ++i)
            {
                color += futures[i].get();
            }
            #endif

            // NOTE: Without Jitter.
            // for (Eigen::Index i = 0; i < config.SPP; ++i)
            // {
            //     #ifdef NDEBUG
            //     futures[i] = THREAD_POOL.Submit([&camera, &hittable, &config, row, col](const unsigned int seed) -> Eigen::Vector3d
            //     {
            //         RNG::Seed(seed);
            //         const Ray sample_ray = camera.SampleRay(row, col);
            //         return RayCast(sample_ray, hittable, 0, config.stop_prob);
            //     }, RNG::Rand(dist));
            //     #else
            //     color += [&camera, &hittable, &config, row, col](const unsigned int seed) -> Eigen::Vector3d
            //     {
            //         RNG::Seed(seed);
            //         const Ray sample_ray = camera.SampleRay(row, col);
            //         return RayCast(sample_ray, hittable, 0, config.stop_prob);
            //     }(RNG::Rand(dist));
            //     #endif
            // }
            //
            // #ifdef NDEBUG
            // for (Eigen::Index i = 0; i < config.SPP; ++i)
            // {
            //     color += futures[i].get();
            // }
            // #endif

            color *= spp_norm_factor;

            auto& pixel = film(row, col);
            // pixel.head(3) = color; GNUC Bug, See: https://gitlab.com/libeigen/eigen/-/merge_requests/1820
            pixel.x() = color.x(), pixel.y() = color.y(), pixel.z() = color.z(), pixel.w() = 1.0;

            CONSTEXPR Eigen::Index length = 100;
            const Eigen::Index finished = (progress * length / (film.Height() * film.Width()));
            const Eigen::Index remained = length - finished;
            std::clog << "\rProgress " << std::setw(6) << std::fixed << std::setprecision(2) << 100.0 * (double)progress / ((double)film.Height() * (double)film.Width()) << "% " << '[' << std::string(finished, '>') << std::string(remained, ' ') << ((acc == 0) ? "-]" : ((acc == 1) ? "\\]" : "/]")) << std::flush;
        }
    }

    std::clog << "\rProgress 100.00% Complete!" << std::endl;
}
