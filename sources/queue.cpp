#include "queue.hpp"

using namespace std;

std::condition_variable cond;
std::mutex mtx;

ThreadQueue::ThreadQueue() : th_queue(), mutex(), notEmpty() {}

void ThreadQueue::enqueue(void *item)
{
    // To ensures that the lock is released before notEmpty.notify_one() is called
    {
        std::lock_guard<std::mutex> lock(mutex);
        th_queue.push(item);
    }
    notEmpty.notify_one();
}

void *ThreadQueue::dequeue()
{
    std::unique_lock<std::mutex> lock(mutex);
    notEmpty.wait(lock, [this]
                  { return !th_queue.empty(); });
    void *item = th_queue.front();
    th_queue.pop();
    return item;
}
