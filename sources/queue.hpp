#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
#include <mutex>
#include <iostream>
#include <thread>
#include <memory> // for std::unique_ptr
#include <condition_variable>

class ThreadQueue
{
private:
    std::queue<void *> th_queue;
    std::mutex mutex;
    std::condition_variable notEmpty;

public:
    ThreadQueue();
    ~ThreadQueue() = default;

    void enqueue(void *item);
    void *dequeue();
};

#endif