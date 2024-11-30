/**
  ******************************************************************************
  * @file           : TestThreadPool.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-26
  ******************************************************************************
  */



#include <doctest/doctest.h>
#include <Core/Common.h>
#include <Core/ThreadPool.h>

TEST_CASE("ThreadPool")
{
    std::vector<std::future<void>> futures;
    futures.resize(8);
    for (size_t i = 0; i < 8; ++i)
    {
        futures[i] = THREAD_POOL.Submit([](size_t tid) -> void
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100 * tid));
            fmt::printf("[%d] Hello Thread %d\n", tid, tid); fflush(stdout);
        }, i);
    }
    for (const auto& future : futures)
    {
        future.wait();
    }
    fmt::printf("Hello Main Thread\n"); fflush(stdout);
}
