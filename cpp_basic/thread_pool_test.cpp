// copied from https://github.com/progschj/ThreadPool/blob/master//example.cpp and modified

#include <iostream>
#include <vector>
#include <chrono>

#include "thread_pool_copy.h"

int main() {

    ThreadPool pool(4);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                std::cout << std::this_thread::get_id() << " start " << i << "\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << std::this_thread::get_id() << " finish " << i << "\n";
                return i * i;
            })
        );
    }

    for(auto && result: results) {
        std::cout << result.get() << ' ';
    }
    std::cout << "\n";

    return 0;
}
