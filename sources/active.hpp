#ifndef ACTIVE_H
#define ACTIVE_H
#include <atomic>

#include "queue.hpp"
#include "shared_variables.hpp"
// cpp
class ActiveObject
{
private:
    ThreadQueue *queue = nullptr;
    void (*func)(void *) = nullptr;
    bool active = false;
    std::thread thread;
    int iteration;
    int num;

public:
    ActiveObject(void (*func)(void *));
    ~ActiveObject(){}

    void startThread()
    {
        thread = std::thread([this]()
                             { runInternal(); });
    }
    int getIteration() const
    {
        return iteration;
    }
    bool isActive() const
    {
        return active;
    }
    void runInternal();
    ThreadQueue *getQueue(){return this->queue;}
    void stop();
};



ActiveObject *createActiveObject(void (*func)(void *));
void destroyActiveObject(ActiveObject *obj);
static int numAO = 1;

#endif
