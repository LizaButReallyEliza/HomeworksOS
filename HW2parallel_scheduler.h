#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>

class parallel_scheduler {
public:
    parallel_scheduler(size_t capacity);
    ~parallel_scheduler();
    void run(std::function<void()> func);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

