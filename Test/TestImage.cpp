/**
  ******************************************************************************
  * @file           : TestImage.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-20
  ******************************************************************************
  */



#include <doctest/doctest.h>
#include <Core/Common.h>
#include <Core/Debug.h>
#include <Core/Image.h>

TEST_CASE("Image")
{
    SUBCASE("Test01.png")
    {
        const size_t st = Debug::TimeMilliS();
        const Image image = Image::From((FS::path(STR(CMAKE_SOURCE_DIR)) / "Test" / "Image" / "Test01.png").string().c_str());
        const bool result = Image::ToPNG(image, (FS::path(STR(CMAKE_SOURCE_DIR)) / "Test" / "Image" / "TestWrite01.png").string().c_str());
        ASSERT(result);
        const size_t ed = Debug::TimeMilliS();
        fmt::printf("Time Escape: %llu\n", ed - st); fflush(stdout);
    }

    SUBCASE("Test02.png")
    {
        const size_t st = Debug::TimeMilliS();
        const Image image = Image::From((FS::path(STR(CMAKE_SOURCE_DIR)) / "Test" / "Image" / "Test02.png").string().c_str());
        const bool result = Image::ToPNG(image, (FS::path(STR(CMAKE_SOURCE_DIR)) / "Test" / "Image" / "TestWrite02.png").string().c_str());
        ASSERT(result);
        const size_t ed = Debug::TimeMilliS();
        fmt::printf("Time Escape: %llu\n", ed - st); fflush(stdout);
    }
}
