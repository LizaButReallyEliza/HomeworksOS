#include "parallel_scheduler.h"
#include <iostream>
#include <unistd.h>

void task(int id) {
    std::cout << "Task " << id << " started\n";
    sleep(1); // Simulate some work
    std::cout << "Task " << id << " finished\n";
}

int main() {
    const size_t capacity = 4;
    parallel_scheduler scheduler(capacity);

    for (int i = 0; i < 10; ++i) {
        scheduler.run([i] { task(i); });
    }

    // Sleep to allow all tasks to finish
    sleep(5);

    return 0;
}
