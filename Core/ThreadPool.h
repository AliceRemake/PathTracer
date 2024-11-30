/**
  ******************************************************************************
  * @file           : ThreadPool.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-20
  ******************************************************************************
  */



// Ref: https://github.com/progschj/ThreadPool.git
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <Core/Common.h>

class ThreadPool
{
private:
    const size_t THREAD_NUMBER;
    bool terminate;
    std::mutex mutex;
    std::condition_variable condition;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> works;

    // ReSharper disable once CppDFAUnreachableFunctionCall
    void ThreadLoop() NOEXCEPT
    {
        while(true)
        {
            std::function<void()> work;
            {
                std::unique_lock lock(mutex);
                condition.wait(lock, [this]() -> bool
                {
                    return !works.empty() || terminate;
                });
                if (terminate)
                {
                    return;
                }
                work = std::move(works.front());
                works.pop();
            }
            work();
        }
    }
    
public:
    NODISCARD CONSTEXPR FORCE_INLINE size_t ThreadNumber() const NOEXCEPT
    {
        return THREAD_NUMBER;
    }
    
    ThreadPool() NOEXCEPT : THREAD_NUMBER(std::thread::hardware_concurrency()), terminate(false)
    {
        threads.reserve(THREAD_NUMBER);
        for (size_t thread_id = 0; thread_id < THREAD_NUMBER; ++thread_id)
        {
            threads.emplace_back(std::bind(ThreadLoop, this));
        }
    }

    ~ThreadPool() NOEXCEPT
    {
        {
            std::unique_lock lock(mutex);
            terminate = true;
        }
        condition.notify_all();
        for (auto& thread : threads)
        {
            thread.join();
        }
        threads.clear();
    }

    template<class Function, class... Args>
    std::future<std::result_of_t<Function(Args...)>> Submit(Function&& function, Args&&... args)
    {
        using ReturnType = std::result_of_t<Function(Args...)>;
        std::shared_ptr<std::packaged_task<ReturnType()>> work = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<Function>(function), std::forward<Args>(args)...)
        );
        std::future<ReturnType> future = work->get_future();
        {
            std::unique_lock lock(mutex);
            ASSERT(!terminate);
            works.emplace([work]() -> void
            {
                (*work)();
            });
        }
        condition.notify_one();
        return future;
    }
};

static ThreadPool THREAD_POOL;

#endif //THREAD_POOL_H
