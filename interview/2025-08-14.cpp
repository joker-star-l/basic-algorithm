// 2025.08.14 字节跳动（AML）一面
// 给定一天以内日志文件，数据量很大，每条数据表示为 [userId, loginTime, logoutTime]，统计峰值在线用户数与其持续时间
#include <iostream>
#include <vector>

// 差分数组 + 扫描线
void solve(std::vector<std::vector<int>>& logs, int& ans, std::pair<int, int>& duration) {
    int n = 3600 * 24; // 一天的秒数
    std::vector<int> diff(n, 0); // 差分数组
    for (const auto& log : logs) {
        int loginTime = log[1];
        int logoutTime = log[2];
        diff[loginTime]++;
        if (logoutTime < n) {
            diff[logoutTime]--;
        }
    }
    
    int current = 0;
    ans = 0;
    int start = -1;
    duration = {0, 0};
    for (int i = 0; i < n; i++) {
        current += diff[i];
        if (current > ans) {
            ans = current;
            start = i;
            duration = {start, start};
        } else if (current == ans && start != -1) {
            duration.second = i;
        } else if (current < ans) {
            start = -1;
        }
    }
}

int main() {
    std::vector<std::vector<int>> logs = {
        {1, 1, 5},
        {2, 2, 6},
        {3, 4, 8},
        {4, 5, 7}
    };
    int ans;
    std::pair<int, int> duration;
    solve(logs, ans, duration);
    std::cout << "峰值在线用户数: " << ans << "\n";
    std::cout << "持续时间: [" << duration.first << ", " << duration.second << "]\n";
    return 0;
}