#include <queue>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <mutex>
#include <condition_variable>
#include <memory>

template <typename T>
class DelayQueue {
public:
    void push(const T& item, std::chrono::seconds delay) {
        std::unique_lock<std::mutex> lock(mtx);
        auto expire_time = std::chrono::steady_clock::now() + delay;
        pq.emplace(expire_time, item);
        auto& top = pq.top();
        if (top.first == expire_time) {
            cv.notify_all();
        }
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        for (;;) {
            if (pq.empty()) {
                cv.wait(lock);
            } else {
                auto& top = pq.top();
                auto now = std::chrono::steady_clock::now();
                auto next_expire = top.first;
                if (now >= next_expire) {
                    T item = pq.top().second;
                    pq.pop();
                    return item;
                }
                cv.wait_until(lock, next_expire);
            }
        }
    }

    bool empty() {
        std::unique_lock<std::mutex> lock(mtx);
        return pq.empty();
    }

    size_t size() {
        std::unique_lock<std::mutex> lock(mtx);
        return pq.size();
    }

private:
    std::mutex mtx;
    using E = std::pair<std::chrono::steady_clock::time_point, T>;
    std::priority_queue<E, std::vector<E>, std::greater<E>> pq;
    std::condition_variable cv;
};
