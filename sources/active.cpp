#include "active.hpp"
#include "shared_variables.hpp"

using namespace std;

ActiveObject::ActiveObject(void (*func)(void *))
{
    this->func = func;
    this->active = true;
    this->queue = new ThreadQueue();
    this->iteration = 0;
    this->num = numAO++;
}

void ActiveObject::runInternal()
{
    while (void *task = queue->dequeue())
    {
        int *taskInt = static_cast<int *>(task);
        if (*taskInt == 0)
        {
            break;
        }

        func(task);
        this->iteration++;

        if (this->iteration == global_N)
        {
            if (this->num == 4)
            {
                std::unique_lock<std::mutex> lock(mtx);
                cond.notify_one();
            }
            else
            {
                cond.notify_all();
            }
        }
    }
}

void ActiveObject::stop()
{
    active = false;
    int end = 0;
    queue->enqueue(&end);
    thread.join();
    delete queue;
    queue = nullptr;
}

ActiveObject *createActiveObject(void (*func)(void *))
{
    ActiveObject *obj = new ActiveObject(func);
    obj->startThread();
    return obj;
}

void destroyActiveObject(ActiveObject *obj)
{
    obj->stop();
    delete obj;
}