#include <iostream>
#include <thread>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
int flag = 0;
int CNT = 10;

void print_string(const std::string& s, int id) {
    std::unique_lock<std::mutex> lk(mtx);
    for (int i = 0; i < CNT; i++) {
        // while (flag % 3 != n) {
        //     cv.wait(lk);
        // }
        // 与上面的 while 循环等价
        cv.wait(lk, [id] { return flag % 3 == id; });

        std::cout << s << " ";
        flag++;
        cv.notify_all();
    }
}

int main() {
    std::thread t1(print_string, "A", 0);
    std::thread t2(print_string, "B", 1);
    std::thread t3(print_string, "C", 2);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "\n";

    return 0;
}