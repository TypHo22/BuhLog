#include <ThreadPool.h>

ThreadPool::ThreadPool(int numThreads) {

    for(size_t a = 0; a < numThreads; a++)
    {
        threads_.push_back(std::thread([this]()
        {
            while (true)
            {
                // Get the next task
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex_);

                    while (tasks_.empty() && !stop_)
                    {
                        condition_.wait(lock);
                    }

                    if (stop_)
                    {
                        return;
                    }
                    task = tasks_.front();
                    tasks_.pop();
                }
                // Run the task
                queueMutex_.lock();
                task();
                queueMutex_.unlock();
            }
        }));
    }
}

void ThreadPool::addTask(std::function<void ()> task)
{
    {
        std::unique_lock<std::mutex> lock(queueMutex_);
        tasks_.push(task);
    }
    // Notify a waiting thread that there is a new task
    condition_.notify_one();
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(queueMutex_);
        stop_ = true;
    }
    condition_.notify_all();
    for (auto& thread : threads_) {
        thread.join();
    }
}
