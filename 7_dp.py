from typing import List
from math import inf

# 0-1 背包（选或不选）
# 有 n 个物品，第 i 个物品的体积为 w[i]，价值为 v[i]，每个物品至多选一个，背包的容量为 c，求背包能装下的最大价值
# dfs(i, c) = max(dfs(i - 1, c), dfs(i - 1, c - w[i]) + v[i])
# 变形
# 1. 至多装 capacity，求方案数 / 最大价值和
# 2. 恰好装 capacity，求方案数 / 最大 / 小价值和
# 3. 至少装 capacity，求方案数 / 最大价值和
# 416. 分割等和子集
# https://leetcode.cn/problems/partition-equal-subset-sum
def canPartition(self, nums: List[int]) -> bool:
    n = len(nums)
    s = sum(nums)
    if s % 2 != 0:
        return False
    s //= 2  # s 等价于背包的容量，如果元素有负数，则不能使用该方法
    dp = [[False for _ in range(s + 1)] for _ in range(n + 1)]
    dp[0][0] = True
    for i in range(n):
        x = nums[i]
        for j in range(s + 1):
            if j >= x:
                dp[i + 1][j] = dp[i][j] or dp[i][j - x]
            else:
                dp[i + 1][j] = dp[i][j]

    return dp[n][s]


# 完全背包（选或不选）
# 与 0-1 背包的区别在于每个物品可以选多次
# dfs(i, c) = max(dfs(i - 1, c), dfs(i, c - w[i]) + v[i])
# 变形与 0-1 背包类似
# 322. 零钱兑换
# https://leetcode.cn/problems/coin-change
def coinChange(self, coins: List[int], amount: int) -> int:
    n = len(coins)
    dp = [[inf for _ in range(amount + 1)] for _ in range(n + 1)]
    dp[0][0] = 0
    for i in range(n):
        x = coins[i]
        for j in range(amount + 1):
            if j >= x:
                dp[i + 1][j] = min(dp[i][j], dp[i + 1][j - x] + 1)
            else:
                dp[i + 1][j] = dp[i][j]

    return -1 if dp[n][amount] == inf else dp[n][amount]
