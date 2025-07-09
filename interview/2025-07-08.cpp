// 2025.07.08 快手（k-star）一面
// 给定 M 个长度为N的降序数组，求最大的 K 个数，复杂度 O(K * M) -> 大顶堆 O(K * logM)
#include <queue>
#include <vector>
#include <iostream>

struct Element {
    int i;
    int j;
    int v;
    Element(int i, int j, int v) : i(i), j(j), v(v) {}
};

struct Comparator {
    bool operator() (const Element& a, const Element& b) {
        return a.v < b.v; // 大顶堆
    }
};

int main() {
    int M = 3, N = 4, K = 5;
    int arr[3][4] = {
        {10, 9, 8, 7},
        {6, 5, 4, 3},
        {2, 1, 0, -1}
    };

    // 使用大顶堆来维护最大的 M 个数
    std::priority_queue<Element, std::vector<Element>, Comparator> maxHeap;
    std::vector<int> ans;
    for (int i = 0; i < M; i++) {
        maxHeap.push(Element(i, 0, arr[i][0]));
    }
    while (ans.size() < K) {
        Element top = maxHeap.top();
        maxHeap.pop();
        ans.push_back(top.v);
        int j = top.j + 1;
        if (j < N) {
            maxHeap.push(Element(top.i, j, arr[top.i][j]));
        }
    }

    // 输出结果
    for (int i = 0; i < K; i++) {
        std::cout << ans[i] << " ";
    }
    std::cout << "\n";
}
