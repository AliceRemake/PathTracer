/**
  ******************************************************************************
  * @file           : Image.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-18
  ******************************************************************************
  */



#include <Core/Image.h>
#include <Core/Parallel.h>

// Ignore stb Warnings.
#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#elif defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wmissing-field-initializers"
#elif defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable:4996)
#endif

#include <stb_image.h>
#include <stb_image_write.h>

#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#elif defined(__clang__)
    #pragma clang diagnostic pop
#elif defined(_MSC_VER)
    #pragma warning(pop)
#endif

NODISCARD Image Image::From(const char* filename) NOEXCEPT
{
    int width, height, nr_channel;
    const float* data = stbi_loadf(filename, &width, &height, &nr_channel, STBI_rgb_alpha);

    Image image(height, width);

    Parallel::For(0, image.Height() * image.Width(), THREAD_POOL.ThreadNumber(),
        [&image, &data](size_t thread_begin, size_t thread_end) -> void
        {
            for (Eigen::Index i = (Eigen::Index)thread_begin; i < (Eigen::Index)thread_end; ++i)
            {
                image.Data()[i].x() = data[(i << 2) | 0];
                image.Data()[i].y() = data[(i << 2) | 1];
                image.Data()[i].z() = data[(i << 2) | 2];
                image.Data()[i].w() = data[(i << 2) | 3];
            }
        }
    );

    return image;
}

NODISCARD bool Image::ToPNG(const Image& image, const char* filename) NOEXCEPT
{
    stbi_uc* buffer = new stbi_uc[(image.Height() * image.Width()) << 2];

    Parallel::For(0, image.Height() * image.Width(), THREAD_POOL.ThreadNumber(),
        [&buffer, &image](size_t thread_begin, size_t thread_end) -> void
        {
            for (Eigen::Index i = (Eigen::Index)thread_begin; i < (Eigen::Index)thread_end; ++i)
            {
                Eigen::Vector4d gamma_correction = Eigen::round(Eigen::pow(image.Data()[i].array(), 1.0 / 2.2) * 255.0);
                buffer[(i << 2) | 0] = (stbi_uc)gamma_correction.x();
                buffer[(i << 2) | 1] = (stbi_uc)gamma_correction.y();
                buffer[(i << 2) | 2] = (stbi_uc)gamma_correction.z();
                buffer[(i << 2) | 3] = (stbi_uc)gamma_correction.w();
            }
        }
    );

    const bool result = stbi_write_png(filename, (int)image.Width(), (int)image.Height(), STBI_rgb_alpha, buffer, 0);

    delete[] buffer;

    return result;
}
