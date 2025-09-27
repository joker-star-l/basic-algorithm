#include <iostream>

class A {
public:
    static A instance; // 方法一
    static A& getInstance() { // 方法二
        static A instance;
        return instance;
    }
    void f() {
        std::cout << "A::f()" << std::endl;
    }
private:
    A() {}
    A(const A&) = delete;
    A(A&&) = delete;
    A& operator=(const A&) = delete;
    A& operator=(A&&) = delete;
};

A A::instance;

int main() {
    A::instance.f();
    A::getInstance().f();
    return 0;
}
