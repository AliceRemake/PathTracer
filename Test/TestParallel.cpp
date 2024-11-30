/**
  ******************************************************************************
  * @file           : TestParallel.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-18
  ******************************************************************************
  */



#include <doctest/doctest.h>
#include <Core/Common.h>
#include <Core/Debug.h>
#include <Core/Parallel.h>

TEST_CASE("Parallel")
{
    SUBCASE("For") {
        std::vector<int> vec(1000000);

        size_t st = Debug::TimeMilliS();
        Parallel::For(0, 1000000, THREAD_POOL.ThreadNumber(), [&](size_t thread_begin, size_t thread_end) -> void
        {
            for (size_t i = thread_begin; i < thread_end; ++i)
            {
                int n = 10000;
                while(n--) ++vec[i];
            }
        });
        size_t ed = Debug::TimeMilliS();
        fmt::printf("Parallel Time: %d(ms)\n", ed - st); fflush(stdout);

        for (size_t i = 0; i < 1000000; ++i)
        {
            ASSERT(vec[i] == 10000);
        }

        st = Debug::TimeMilliS();
        for (size_t i = 0; i < 1000000; ++i)
        {
            int n = 10000;
            while(n--) --vec[i];
        }
        ed = Debug::TimeMilliS();
        fmt::printf("Serial   Time: %d(ms)\n", ed - st); fflush(stdout);

        for (size_t i = 0; i < 1000000; ++i)
        {
            ASSERT(vec[i] == 0);
        }
    }
}
