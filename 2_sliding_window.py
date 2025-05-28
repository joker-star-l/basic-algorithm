from typing import List

# 滑动窗口
# 1. 入
# 2. 出
# 3. 记录答案
# 209. 长度最小的子数组
# https://leetcode.cn/problems/minimum-size-subarray-sum
def minSubArrayLen(self, target: int, nums: List[int]) -> int:
    # 因为 nums 里都是正数，所以可以用滑动窗口
    n = len(nums)
    l = 0
    sum_ = 0
    ans = n + 1
    for r, x in enumerate(nums):
        sum_ += x  # 入
        while l <= r and sum_ >= target:
            ans = min(ans, r - l + 1)  # 记录答案
            sum_ -= nums[l]  # 出
            l += 1

    return ans if ans <= n else 0
