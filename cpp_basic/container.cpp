#include <string>
#include <vector>
#include <deque>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <iostream>

void test_str() {
    std::cout << "=== Testing string:\n";
    // 初始化
    std::string str = "Hello, World!";
    // 查找
    size_t pos = str.find("World");
    if (pos != std::string::npos) {
        std::cout << "Found 'World' at position: " << pos << "\n";
    } else {
        std::cout << "'World' not found\n";
    }
    // 替换
    str.replace(/* start */ pos, /* size */ 5, "C++");
    std::cout << "After replacement: " << str << "\n";
    // 子串
    std::string sub_str = str.substr(/* start */ 0, /* size */ 5);
    std::cout << "Substring: " << sub_str << "\n";
    // 排序
    std::sort(str.begin(), str.end());
    // std::ranges::sort(str); // C++20 范围排序
    std::cout << "Sorted string: " << str << "\n";
}

bool compare(int a, int b) {
    return a > b; // 降序
}

void test_array() {
    std::cout << "=== Testing array:\n";
    // 初始化
    int arr[5];
    std::fill(arr, arr + 5, 1); // 填充初始值为 1
    // 遍历
    for (auto& a : arr) {
        std::cout << a << " ";
    }
    std::cout << "\n";
}

void test_vector() {
    std::cout << "=== Testing vector:\n";
    // 初始化
    std::vector<int> vec(/* 大小 */ 3, /* 初始值 */ 0);
    // 增 / 删 / 查最后一个元素
    vec.push_back(1);
    vec.pop_back();
    vec.push_back(1);
    std::cout << "Last element: " << vec.back() << "\n";
    // 增 / 删 / 查第一个元素
    int i = 0;
    vec.insert(vec.begin(), 2);
    vec.erase(vec.begin());
    vec.insert(vec.begin(), 2);
    std::cout << "First element: " << vec.front() << "\n";
    // 大小
    std::cout << "Size: " << vec.size() << "\n";
    // 判空
    std::cout << "Empty: " << vec.empty() << "\n";
    // 自定义排序
    std::sort(vec.begin(), vec.end(), [](int a, int b) {
        return a > b; // 降序
    });
    std::sort(vec.begin(), vec.end(), compare);
    // 拷贝
    std::vector<int> vec_copy(vec); // 使用拷贝构造函数
    std::vector<int> vec_copy2(vec.begin(), vec.end());
    // 遍历
    for (const auto& v : vec_copy2) {
        std::cout << v << " ";
    }
    std::cout << "\n";
}

void test_deque() {
    std::cout << "=== Testing deque:\n";
    // 初始化
    std::deque<int> dq(/* 大小 */ 3, /* 初始值 */ 0);
    // 增 / 删 / 查最后一个元素
    dq.push_back(1);
    dq.pop_back();
    dq.push_back(1);
    std::cout << "Last element: " << dq.back() << "\n";
    // 增 / 删 / 查第一个元素
    dq.push_front(2);
    dq.pop_front();
    dq.push_front(2);
    std::cout << "First element: " << dq.front() << "\n";
    // 大小
    std::cout << "Size: " << dq.size() << "\n";
    // 判空
    std::cout << "Empty: " << dq.empty() << "\n";
    // 自定义排序
    std::sort(dq.begin(), dq.end(), [](int a, int b) {
        return a > b; // 降序
    });
    std::sort(dq.begin(), dq.end(), compare);
    // 拷贝
    std::deque<int> dq_copy(dq); // 使用拷贝构造函数
    std::deque<int> dq_copy2(dq.begin(), dq.end());
    // 遍历
    for (const auto& v : dq_copy2) {
        std::cout << v << " ";
    }
    std::cout << "\n";
}

void test_list() {
    std::cout << "=== Testing list:\n";
    // 初始化
    std::list<int> ls(/* 大小 */ 3, /* 初始值 */ 0);
    // 增 / 删 / 查最后一个元素
    ls.push_back(1);
    ls.pop_back();
    ls.push_back(1);
    std::cout << "Last element: " << ls.back() << "\n";
    // 增 / 删 / 查第一个元素
    ls.push_front(2);
    ls.pop_front();
    ls.push_front(2);
    std::cout << "First element: " << ls.front() << "\n";
    // 大小
    std::cout << "Size: " << ls.size() << "\n";
    // 判空
    std::cout << "Empty: " << ls.empty() << "\n";
    // 自定义排序
    ls.sort(compare);
    // 拷贝
    std::list<int> ls_copy(ls); // 使用拷贝构造函数
    std::list<int> ls_copy2(ls.begin(), ls.end());
    // 遍历
    for (const auto& v : ls_copy2) {
        std::cout << v << " ";
    }
    std::cout << "\n";
}

void test_map() {
    std::cout << "=== Testing unordered_map:\n";
    // 初始化
    std::unordered_map<int, int> map = {
        {1, 10},
        {2, 20},
        {3, 30}
    };
    // 增
    map[4] = 40;
    map.insert({5, 50});
    // 删
    map.erase(2);
    // 查
    auto it = map.find(3);
    if (it != map.end()) {
        std::cout << "Found key 3 with value: " << it->second << "\n";
    } else {
        std::cout << "Key 3 not found\n";
    }
    // 改
    map[1] = 100;
    // 大小
    std::cout << "Size: " << map.size() << "\n";
    // 判空
    std::cout << "Empty: " << map.empty() << "\n";
    // 拷贝
    std::unordered_map<int, int> map_copy(map); // 使用拷贝构造函数
    std::unordered_map<int, int> map_copy2(map.begin(), map.end());
    // 遍历
    for (const auto& pair : map_copy2) {
        std::cout << "{Key: " << pair.first << ", Value: " << pair.second << "} ";
    }
    std::cout << "\n";
}

void test_set() {
    std::cout << "=== Testing set:\n";
    // 初始化
    std::unordered_set<int> set = {1, 2, 3, 4, 5};
    // 增
    set.insert(6);
    // 删
    set.erase(3);
    // 查
    if (set.find(2) != set.end()) {
        std::cout << "Found 2 in the set\n";
    } else {
        std::cout << "2 not found in the set\n";
    }
    // 大小
    std::cout << "Size: " << set.size() << "\n";
    // 判空
    std::cout << "Empty: " << set.empty() << "\n";
    // 拷贝
    std::unordered_set<int> set_copy(set); // 使用拷贝构造函数
    std::unordered_set<int> set_copy2(set.begin(), set.end());
    // 遍历
    for (const auto& v : set_copy2) {
        std::cout << v << " ";
    }
    std::cout << "\n";
}

struct compare_operator {
    bool operator() (int a, int b) const {
        return a < b;
    }
};

void test_priority_queue() {
    std::cout << "=== Testing priority_queue:\n";
    // 初始化
    static auto cmp = [] (int a, int b) {
        return a < b;
    };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> pq(cmp); // 大顶堆，与 compare_operator 相反
    // std::priority_queue<int, std::vector<int>, compare_operator> pq; // 也可以
    // 增
    pq.push(5);
    pq.push(1);
    pq.push(3);
    pq.push(2);
    pq.push(4);
    // 查
    std::cout << "Top element: " << pq.top() << "\n";
    // 删
    pq.pop();
    std::cout << "After pop, top element: " << pq.top() << "\n";
    // 大小
    std::cout << "Size: " << pq.size() << "\n";
    // 判空
    std::cout << "Empty: " << pq.empty() << "\n";
}

void test_rand() {
    int r = std::rand() % 100; // 生成 0-99 的随机数
    std::cout << "Random number: " << r << "\n";
}

int main() {
    test_str();
    test_array();
    test_vector();
    test_deque();
    test_list();
    test_map();
    test_set();
    test_priority_queue();
    test_rand();
}
