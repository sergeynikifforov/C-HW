//
// Created by SergeyN on 24.03.2018.
//
#ifndef UNTITLED7_THREADPOOL_H
#define UNTITLED7_THREADPOOL_H

#include <future>
#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <condition_variable>
#include <thread>
#include <mutex>

class ThreadPool{
public:
    using Task = std::function<void()>;

    explicit ThreadPool(std::size_t threads) {
        start(threads);
    }

    ~ThreadPool() {
        stop();
    }

    template <class T>
    auto enqueue(T task)->std::future<decltype(task())>
    {
        auto wrapper = std::make_shared<std::packaged_task<decltype(task()) ()>>(std::move(task));
        {
            std::unique_lock<std::mutex> lock{mEventmutex};
            mTasks.emplace([=] {
                (*wrapper)();
            });
        }
        mEvent.notify_one();
        return wrapper->get_future();
    }


private:
    std::vector< std::thread > mThreads;
    std::condition_variable mEvent;
    std::queue<Task> mTasks;
    //std::queue< std::function<void()> > task;
    std::mutex mEventmutex;

    bool stopper = false;

    void stop() {
        {
            std::unique_lock<std::mutex> lock{mEventmutex};
            stopper = true;
        }

        mEvent.notify_all();

        for(auto &thread : mThreads){
            thread.join();
        }
    }
    void start(std::size_t numThreads){
        for(unsigned int i =0; i <  numThreads; ++i){
            mThreads.emplace_back([=]{
                while(true)
                {
                    Task task;
                    {
                        std::unique_lock<std::mutex> lock{mEventmutex};

                        mEvent.wait(lock, [=]{ return stopper || !mTasks.empty();});

                        if(stopper && mTasks.empty()) {
                            break;
                        }

                        task = std::move(mTasks.front());
                        mTasks.pop();
                    }

                    task();
                }
            });
        }
    }

};
//перегрузить оператор равно
#endif //UNTITLED7_THREADPOOL_H
