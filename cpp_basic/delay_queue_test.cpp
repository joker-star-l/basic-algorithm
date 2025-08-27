#include "delay_queue.h"
#include <iostream>
#include <thread>
#include <string>
#include <cassert>

void test_single_thread() {
    std::cout << "=== test_single_thread ===" << std::endl;
    DelayQueue<int> dq;
    dq.push(1, std::chrono::seconds(3));
    dq.push(2, std::chrono::seconds(2));
    dq.push(3, std::chrono::seconds(1));
    assert(dq.size() == 3);
    int top = dq.pop();
    assert(top == 3);
    std::cout << "Popped: " << top << std::endl;
    top = dq.pop();
    assert(top == 2);
    std::cout << "Popped: " << top << std::endl;
    top = dq.pop();
    assert(top == 1);
    std::cout << "Popped: " << top << std::endl;
}

void test_SPSC() {
    std::cout << "=== test_SPSC ===" << std::endl;
    DelayQueue<int> dq;
    std::thread producer([&dq]() {
        dq.push(1, std::chrono::seconds(3));
        dq.push(2, std::chrono::seconds(2));
        dq.push(3, std::chrono::seconds(1));
    });

    std::thread consumer([&dq]() {
        for (int i = 0; i < 3; ++i) {
            int task = dq.pop();
            assert(task == 3 - i);
            std::cout << "Processed: " << task << std::endl;
        }
    });

    producer.join();
    consumer.join();
}

void test_MPSC() {
    std::cout << "=== test_MPSC ===" << std::endl;
    DelayQueue<int> dq;
    std::thread producer0([&dq]() {
        dq.push(1, std::chrono::seconds(3));
    });
    std::thread producer1([&dq]() {
        dq.push(2, std::chrono::seconds(2));
    });
    std::thread producer2([&dq]() {
        dq.push(3, std::chrono::seconds(1));
    });

    std::thread consumer([&dq]() {
        for (int i = 0; i < 3; ++i) {
            int task = dq.pop();
            assert(task == 3 - i);
            std::cout << "Processed: " << task << std::endl;
        }
    });

    producer0.join();
    producer1.join();
    producer2.join();
    consumer.join();
}

void test_MPMC() {
    std::cout << "=== test_MPMC ===" << std::endl;
    DelayQueue<int> dq;
    std::thread producer0([&dq]() {
        dq.push(1, std::chrono::seconds(3));
    });
    std::thread producer1([&dq]() {
        dq.push(2, std::chrono::seconds(2));
    });
    std::thread producer2([&dq]() {
        dq.push(3, std::chrono::seconds(1));
    });

    std::vector<int> results;
    std::mutex mtx;

    auto f = [&dq, &results, &mtx]() {
        int task = dq.pop();
        std::cout << "Processed: " << task << std::endl;
        std::unique_lock<std::mutex> lock(mtx);
        results.push_back(task);
    };

    std::thread consumer0(f);
    std::thread consumer1(f);
    std::thread consumer2(f);

    producer0.join();
    producer1.join();
    producer2.join();
    consumer0.join();
    consumer1.join();
    consumer2.join();

    assert(results.size() == 3);
    for (int i = 0; i < 3; ++i) {
        assert(results[i] == 3 - i);
    }
}

int main() {
    test_single_thread();
    test_SPSC();
    test_MPSC();
    test_MPMC();
    return 0;
}
