#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <LogManager.h>
#include <QFile>
#include <QString>
#include <QTextStream>


class Task {
public:
    Task(const QString& logMsg, const QString& threadId, LogManager* manager)
        : logMsg_(logMsg),
          threadId_(threadId),
          manager_(manager) {}
    /**
     * @brief operator ()
     * Execute task
     */
    void operator()()
    {
        manager_->setRequestedThread(threadId_);
        QFile* file = manager_->getFile();
        QTextStream ts(file);
        ts << logMsg_ << "\n";
        ts.flush();
    }

private:
    QString logMsg_;
    QString threadId_;
    LogManager* manager_;
};


class ThreadPool {
public:
    ThreadPool(int numThreads);

    /**
     * @brief addTask
     * @param task
     * Add a task to the queue
     */
    void addTask(std::function<void()> task);

    /**
     * @brief stop
     * Stop all threads and wait for them to complete
     */
    void stop();

private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queueMutex_;
    std::condition_variable condition_;
    bool stop_ = false;
};


#endif // THREADPOOL_H
