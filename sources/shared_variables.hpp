#ifndef SHARED_VARIABLES_HPP
#define SHARED_VARIABLES_HPP

#include <condition_variable>
#include <mutex>

extern std::condition_variable cond;
extern std::mutex mtx;
extern int global_N;

#endif
