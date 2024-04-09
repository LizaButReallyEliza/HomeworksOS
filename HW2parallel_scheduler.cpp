#include "parallel_scheduler.h"

parallel_scheduler::parallel_scheduler(size_t capacity) : stop(false) {
    for (size_t i = 0; i < capacity; ++i) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                    if (this->stop && this->tasks.empty())
                        return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        });
    }
}

parallel_scheduler::~parallel_scheduler() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers)
        worker.join();
}

void parallel_scheduler::run(std::function<void()> func) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.emplace(func);
    }
    condition.notify_one();
}
