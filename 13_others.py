# 前缀和
# 560. 和为 K 的子数组
# https://leetcode.cn/problems/subarray-sum-equals-k
# 437. 路径总和 III
# https://leetcode.cn/problems/path-sum-iii


# 差分数组
# ./interview/2025-08-14.cpp
# 1094. 拼车
# https://leetcode.cn/problems/car-pooling


# 树状数组（Binary Indexed Tree）
class BIT:
    def __init__(self, n: int):
        self.tree = [0] * (n + 1)  # 使用下标 1 到 n

    # a[i] 增加 val
    # 1 <= i <= n
    # 时间复杂度 O(log n)
    def update(self, i: int, val: int) -> None:
        t = self.tree
        while i < len(t):
            t[i] += val
            i += i & -i

    # 计算前缀和 a[1] + ... + a[i]
    # 1 <= i <= n
    # 时间复杂度 O(log n)
    def pre(self, i: int) -> int:
        t = self.tree
        res = 0
        while i > 0:
            res += t[i]
            i &= i - 1
        return res

    # 计算区间和 a[l] + ... + a[r]
    # 1 <= l <= r <= n
    # 时间复杂度 O(log n)
    def query(self, l: int, r: int) -> int:
        if r < l:
            return 0
        return self.pre(r) - self.pre(l - 1)

# 307. 区域和检索 - 数组可修改
# https://leetcode.cn/problems/range-sum-query-mutable/description/


# 质因数分解
def factorize(m):
    factors = {}
    i = 2
    while i * i <= m:
        while m % i == 0:
            factors[i] = factors.get(i, 0) + 1
            m //= i
        i += 1
    if m > 1:
        factors[m] = 1
    return factors

# 2521. 数组乘积中的不同质因数数目
# https://leetcode.cn/problems/distinct-prime-factors-of-product-of-array
