// copied from https://blog.csdn.net/xiaoqiu_cr/article/details/95756625 
// and https://blog.csdn.net/kk_flying/article/details/136890675 and modified

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <deque>
#include <vector>
#include <memory>
#include <atomic>

// 自旋锁，由于提供了 lock 和 unlock 方法，因此可以使用 unique_lock 来管理锁的生命周期
class SpinLock {
public:
    void lock() {
        while (flag.test_and_set(std::memory_order_acquire)); // 自旋等待
    }

    void unlock() {
        flag.clear(std::memory_order_release); // 释放锁
    }

    bool try_lock() {
        return !flag.test_and_set(std::memory_order_acquire);
    }
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT; // 原子标志位
};

// 信号量
class Semaphore {
public:
    explicit Semaphore(int cnt = 0) : count(cnt) {}

    void release() {
        std::unique_lock<std::mutex> lock(mtx);
        count++;
        cv.notify_one();
    }

    void acquire() {
        std::unique_lock<std::mutex> lock(mtx);
        while (count == 0) {
            cv.wait(lock);
        }
        count--;
    }
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count; // 信号量计数
};

class Queue {
public:
    virtual ~Queue() = default;
    virtual void put(int i) = 0; // 生产者放入数据
    virtual int get() = 0; // 消费者取出数据
};

void produce(Queue& queue, int start, int n) {
    for (int i = start; i < start + n; i++) {
        queue.put(i);
        std::cout << "Producer " << std::this_thread::get_id() << " produced: " << i << "\n";
    }
}

void conmuse(Queue& queue, int n) {
    for (int i = 0; i < n; i++) {
        int value = queue.get();
        std::cout << "Consumer " << std::this_thread::get_id() << " consumed: " << value << "\n";
    }
}

// Single Producer Single Consumer
class SPSCQueue : public Queue {
public:
    explicit SPSCQueue(int cap) {
        capacity = cap;
        empty = std::make_unique<Semaphore>(cap); // 初始化空槽位信号量
        full = std::make_unique<Semaphore>(0);  // 初始化满槽位信号量
    }

    void put(int i) {
        empty->acquire(); // 等待空槽位
        queue.push_front(i); // 生产者放入数据
        full->release(); // 增加满槽位信号量
    }

    int get() {
        full->acquire(); // 等待满槽位
        int value = queue.back(); // 消费者取出数据
        queue.pop_back();
        empty->release(); // 增加空槽位信号量
        return value;
    }

private:
    int capacity;
    std::deque<int> queue; // 队列
    // 不需要 mutex，因为SPSC队列只有一个生产者和一个消费者
    std::unique_ptr<Semaphore> empty; // 空槽位
    std::unique_ptr<Semaphore> full;  // 满槽位
};

void testSPSCQueue() {
    SPSCQueue queue(10); // 创建一个容量为 10 的 SPSC 队列

    std::thread producer(produce, std::ref(queue), 0, 20);
    std::thread consumer(conmuse, std::ref(queue), 20);

    producer.join();
    consumer.join();
}

// Multi Producer Multi Consumer 
class MPMCQueue : public Queue {
public:
    explicit MPMCQueue(int cap) {
        capacity = cap;
        empty = std::make_unique<Semaphore>(cap); // 初始化空槽位信号量
        full = std::make_unique<Semaphore>(0);  // 初始化满槽位信号量
    }

    void put(int i) {
        empty->acquire(); // 等待空槽位
        {
            std::unique_lock<std::mutex> lock(mtx); // 互斥锁保护队列
            queue.push_front(i); // 生产者放入数据
        }
        full->release(); // 增加满槽位信号量
    }

    int get() {
        full->acquire(); // 等待满槽位
        int value;
        {
            std::unique_lock<std::mutex> lock(mtx); // 互斥锁保护队列
            value = queue.back(); // 消费者取出数据
            queue.pop_back();   
        }
        empty->release(); // 增加空槽位信号量
        return value;
    }

private:
    int capacity;
    std::mutex mtx; // 互斥锁
    std::deque<int> queue; // 队列
    std::unique_ptr<Semaphore> empty; // 空槽位
    std::unique_ptr<Semaphore> full;  // 满槽位
};

void testMPMCQueue() {
    MPMCQueue queue(10); // 创建一个容量为 10 的 MPMC 队列

    std::vector<std::thread> producers;
    for (int i = 0; i < 5; i++) {
        producers.emplace_back(produce, std::ref(queue), i * 4, 4); // 每个生产者生产 4 个数据
    }
    std::vector<std::thread> consumers;
    for (int i = 0; i < 5; i++) {
        consumers.emplace_back(conmuse, std::ref(queue), 4); // 每个消费者消费 4 个数据
    }

    // 等待所有生产者和消费者线程完成
    for (int i = 0; i < 5; i++) {
        producers[i].join();
    }
    for (int i = 0; i < 5; i++) {
        consumers[i].join();
    }
}

// Lock-Free Single Producer Single Consumer Queue
class LockFreeSPSCQueue : public Queue {
public:
    explicit LockFreeSPSCQueue(int cap) {
        capacity = cap + 1; // 环形队列需要一个额外的槽位来区分满和空状态
        queue = new int[capacity]; // 创建环形队列
        head = 0; // 初始化消费者指针
        tail = 0; // 初始化生产者指针
    }

    ~LockFreeSPSCQueue() {
        delete[] queue; // 释放环形队列内存
    }

    void put(int i) {
        while ((tail + 1) % capacity == head); // 等待空槽位
        queue[tail] = i; // 生产者放入数据
        tail = (tail + 1) % capacity; // 更新生产者指针
    }

    int get() {
        while (head == tail); // 等待满槽位
        int value = queue[head]; // 消费者取出数据
        head = (head + 1) % capacity; // 更新消费者指针
        return value;
    }

private:
    int* queue; // 环形队列
    int capacity; // 队列容量
    alignas(64) std::atomic<int> head; // 消费者指针
    alignas(64) std::atomic<int> tail; // 生产者指针
};

void testLockFreeSPSCQueue() {
    LockFreeSPSCQueue queue(10); // 创建一个容量为 10 的 SPSC 队列

    std::thread producer(produce, std::ref(queue), 0, 20);
    std::thread consumer(conmuse, std::ref(queue), 20);

    producer.join();
    consumer.join();
}

// Lock-Free Multi Producer Multi Consumer Queue
// copied from https://github.com/couchbase/phosphor/blob/master/thirdparty/dvyukov/include/dvyukov/mpmc_bounded_queue.h and modified
class LockFreeMPMCQueue : public Queue {
public:
    explicit LockFreeMPMCQueue(int cap) {
        capacity = cap; // head 和 tail 不取模，不需要额外的一个槽位
        queue = new cell_t[capacity]; // 创建环形队列
        for (int i = 0; i < capacity; i++) {
            queue[i].sequence.store(i, std::memory_order_relaxed); // 初始化序列号
        }
        head.store(0, std::memory_order_relaxed); // 初始化消费者指针
        tail.store(0, std::memory_order_relaxed); // 初始化生产者指针
    }

    ~LockFreeMPMCQueue() {
        delete[] queue; // 释放环形队列内存
    }

    void put(int i) {
        cell_t* cell;
        size_t pos = tail.load(std::memory_order_relaxed); // 获取当前生产者指针位置
        while (true) {
            cell = &queue[pos % capacity];
            size_t seq = cell->sequence.load(std::memory_order_acquire); // 获取当前序列号
            intptr_t diff = (intptr_t) seq - (intptr_t) pos; // 计算序列号与生产者指针的差值
            if (diff == 0) { // 如果序列号等于生产者指针位置，说明可以放入数据
                if (tail.compare_exchange_strong(pos, pos + 1, /* success */ std::memory_order_relaxed)) {
                    cell->data = i; // 放入数据
                    cell->sequence.store(pos + 1, std::memory_order_release); // 更新序列号
                    return;
                }
            } else if (diff < 0) { // 如果序列号小于生产者指针位置，说明队列已满
                continue; // 重试
            } else { // 如果序列号大于生产者指针位置，说明有其他线程在消费数据
                pos = tail.load(std::memory_order_relaxed); // 更新生产者指针位置
            }
        }
    }

    int get() {
        cell_t* cell;
        size_t pos = head.load(std::memory_order_relaxed); // 获取当前消费者指针位置
        while (true) {
            cell = &queue[pos % capacity];
            size_t seq = cell->sequence.load(std::memory_order_acquire); // 获取当前序列号
            intptr_t diff = (intptr_t) seq - (intptr_t) (pos + 1); // 计算序列号与消费者指针位置的差值
            if (diff == 0) { // 如果序列号等于消费者指针位置，说明可以取出数据
                if (head.compare_exchange_strong(pos, pos + 1, /* success */ std::memory_order_relaxed)) {
                    cell->sequence.store(pos + capacity, std::memory_order_release); // 更新序列号
                    return cell->data; // 返回数据
                }
            } else if (diff < 0) { // 如果序列号小于消费者指针位置，说明队列为空
                continue; // 重试
            } else { // 如果序列号大于消费者指针位置，说明有其他线程在生产数据
                pos = head.load(std::memory_order_relaxed); // 更新消费者指针位置
            }
        }
        return cell->data;
    }

private:
    struct cell_t {
        std::atomic<size_t> sequence;
        int data;  
    };

    cell_t* queue; // 环形队列
    int capacity; // 队列容量
    alignas(64) std::atomic<size_t> head; // 消费者指针，不取模
    alignas(64) std::atomic<size_t> tail; // 生产者指针，不取模
};

void testLockFreeMPMCQueue() {
    LockFreeMPMCQueue queue(10); // 创建一个容量为 10 的 MPMC 队列

    std::vector<std::thread> producers;
    for (int i = 0; i < 5; i++) {
        producers.emplace_back(produce, std::ref(queue), i * 4, 4); // 每个生产者生产 4 个数据
    }
    std::vector<std::thread> consumers;
    for (int i = 0; i < 5; i++) {
        consumers.emplace_back(conmuse, std::ref(queue), 4); // 每个消费者消费 4 个数据
    }

    // 等待所有生产者和消费者线程完成
    for (int i = 0; i < 5; i++) {
        producers[i].join();
    }
    for (int i = 0; i < 5; i++) {
        consumers[i].join();
    }
}

int main() {
    std::cout << "=== Testing SPSC Queue: ===" << "\n";
    testSPSCQueue(); // 测试 SPSC 队列
    std::cout << "\n=== Testing MPMC Queue: ===" << "\n";
    testMPMCQueue(); // 测试 MPMC 队列
    std::cout << "\n=== Testing Lock-Free SPSC Queue: ===" << "\n";
    testLockFreeSPSCQueue(); // 测试 Lock-Free SPSC 队列
    std::cout << "\n=== Testing Lock-Free MPMC Queue: ===" << "\n";
    testLockFreeMPMCQueue(); // 测试 Lock-Free MPMC 队列
    return 0;
}
