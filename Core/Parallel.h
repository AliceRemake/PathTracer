/**
  ******************************************************************************
  * @file           : Parallel.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-18
  ******************************************************************************
  */



#ifndef PARALLEL_H
#define PARALLEL_H

#include <Core/Common.h>
#include <Core/ThreadPool.h>

struct Parallel
{
    FORCE_INLINE static void For(const size_t begin, const size_t end, size_t thread_number, const std::function<void(size_t thread_begin, size_t thread_end)>& work) NOEXCEPT
    {
        ASSERT(thread_number > 0);
        thread_number = std::min(thread_number, THREAD_POOL.ThreadNumber());
        static std::vector<std::future<void>> futures(THREAD_POOL.ThreadNumber());
        const size_t workload = (end - begin) / thread_number;
        for (size_t i = 0; i < thread_number; ++i)
        {
            futures[i] = THREAD_POOL.Submit(work, begin + workload * i, i == thread_number - 1 ? end : begin + workload * (i + 1));
        }
        for (const auto& future : futures)
        {
            future.wait();
        }
    }
};

#endif //PARALLEL_H
