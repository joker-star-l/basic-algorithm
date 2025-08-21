// 2025.08.20 阿里控股（智能检索）一面
// 有 N 个小球，分别是 1...N 每轮可以合并一对相邻的小球成新的小球，比如 2 和 3 合并成 2-3，
// 2-3 和 4 合并成 2-4; 2-3, 4-5 -> 2-5 每轮选取的合并的规则是挑选合并后分数最高的小球对，
// 分数用（2-4，100）表示把 2-4 的小球合并后的获取的分，
// 可以是 2-3 和 4 合并，也可以是 2 和 3-4 合并。直到不能合并为止。
// 给定一个 N 和若干个合并及其分数，求最后的状态。

#include <unordered_map>
#include <vector>
#include <cassert>
#include <algorithm>
#include <queue>
#include <cstdio>
#include <set>

class UnionFind {
public:
    UnionFind(int n) : fa(n), sz(n, 1) {
        for (int i = 0; i < n; ++i) {
            fa[i] = i;
        }
    }

    int find(int x) {
        if (fa[x] != x) {
            fa[x] = find(fa[x]); // 路径压缩
        }
        return fa[x];
    }

    int find_left(int x) { // 返回左边界
        return find(x);
    }

    int find_right(int x) { // 返回右边界
        int left = find(x);
        return left + sz[left] - 1;
    }

    void merge(int a, int b) {
        int x = find(a);
        int y = find(b);
        if (x == y) {
            return; // 已经在同一个集合中
        }
        if (x > y) {
            std::swap(x, y);
        }
        fa[y] = x; // 合并 y 到 x
        sz[x] += sz[y]; // 更新大小
    }

private:
    std::vector<int> fa;
    std::vector<int> sz;
};

struct PairHash {
    size_t operator()(const std::pair<int, int>& t) const {
        return std::hash<int>()(t.first + t.second);
    }
};

// 大根堆 + 并查集
std::vector<std::pair<int,int>> solve(int N, std::vector<std::vector<int>>& pairs) {
    UnionFind uf(N + 1);
    std::unordered_map<std::pair<int, int>, int, PairHash> pair_to_score;
    std::priority_queue<std::tuple<int, int, int>> pq; // 分数, 左边界, 右边界
    for (const auto& p : pairs) {
        int left = p[0];
        int right = p[1];
        int score = p[2];
        pair_to_score[{left, right}] = score;
        if (right - left == 1) {
            pq.push({score, left, right});
        }
    }
    
    while (!pq.empty()) {
        auto [score, left, right] = pq.top();
        pq.pop();
        // printf("Merge (%d, %d) with score %d\n", left, right, score);
        if (uf.find(left) != uf.find(right) && uf.find_left(left) == left && uf.find_right(right) == right) {
            uf.merge(left, right);
            int new_left = uf.find_left(left);
            int new_right = uf.find_right(right);
            if (new_left > 1) {
                int left_neighbor = new_left - 1;
                int left_neighbor_left = uf.find_left(left_neighbor);
                std::pair<int, int> key = {left_neighbor_left, new_right};
                // printf("(%d, %d) left considering (%d, %d)\n", left, right, left_neighbor_left, new_right);
                auto it = pair_to_score.find(key);
                if (it != pair_to_score.end()) {
                    // printf("Found score %d for (%d, %d)\n", it->second, left_neighbor_left, new_right);
                    pq.push({it->second, left_neighbor_left, new_right});
                }
            }
            if (new_right < N) {
                int right_neighbor = new_right + 1;
                int right_neighbor_right = uf.find_right(right_neighbor);
                // printf("(%d, %d) right considering (%d, %d)\n", left, right, right_neighbor, right_neighbor_right);
                std::pair<int, int> key = {new_left, right_neighbor_right};
                auto it = pair_to_score.find(key);
                if (it != pair_to_score.end()) {
                    // printf("Found score %d for (%d, %d)\n", it->second, new_left, right_neighbor_right);
                    pq.push({it->second, new_left, right_neighbor_right});
                }
            }   
        }
    }
    
    std::vector<std::pair<int, int>> result;
    for (int i = 1; i <= N;) {
        int left = uf.find_left(i);
        int right = uf.find_right(i);
        result.push_back({left, right});
        i = right + 1;
    }
    return result;
}

void vector_print(const std::vector<std::pair<int, int>>& v) {
    for (const auto& p : v) {
        printf("%d-%d, ", p.first, p.second);
    }
    printf("\n");
}

void vector_equals(std::vector<std::pair<int, int>>& a, std::vector<std::pair<int, int>>& b) {
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    vector_print(a);
    vector_print(b);
    assert(a.size() == b.size());
    for (int i = 0; i < a.size(); ++i) {
        assert(a[i] == b[i]);
    }
}

void test1() {
    int N = 5;
    std::vector<std::vector<int>> pairs = {
        {1, 2, 90},
        {2, 3, 80},
        {3, 4, 70},
        {4, 5, 60},
        {1, 4, 50}
    };
    std::vector<std::pair<int, int>> expected = {
        {1, 4},
        {5, 5}
    };
    std::vector<std::pair<int, int>> result = solve(N, pairs);
    vector_equals(result, expected);
}

void test2() {
    int N = 5;
    std::vector<std::vector<int>> pairs = {
        {1, 5, 100},
        {1, 2, 90},
        {2, 3, 80},
        {3, 4, 70},
        {4, 5, 60},
        {1, 4, 50}
    };
    std::vector<std::pair<int, int>> expected = {
        {1, 5}
    };
    std::vector<std::pair<int, int>> result = solve(N, pairs);
    vector_equals(result, expected);
}

void test3() {
    int N = 5;
    std::vector<std::vector<int>> pairs = {
        {1, 5, 100},
        {1, 2, 90},
        {2, 3, 80},
        {3, 4, 70}
    };
    std::vector<std::pair<int, int>> expected = {
        {1, 2},
        {3, 4},
        {5, 5}
    };
    std::vector<std::pair<int, int>> result = solve(N, pairs);
    vector_equals(result, expected);
}

int main() {
    test1();
    test2();
    test3();
    return 0;
}
