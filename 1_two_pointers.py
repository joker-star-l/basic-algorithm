from typing import List

# 167. 两数之和 II - 输入有序数组 
# https://leetcode.cn/problems/two-sum-ii-input-array-is-sorted
def twoSum(self, numbers: List[int], target: int) -> List[int]:
    n = len(numbers)
    i = 0
    j = n - 1
    while i < j:
        sum_ = numbers[i] + numbers[j]
        if sum_ == target:
            return [i + 1, j + 1]
        if sum_ < target:
            i += 1
        else:
            j -= 1
    return []


# 42. 接雨水
# https://leetcode.cn/problems/trapping-rain-water
def trap(self, height: List[int]) -> int:
    # 如果前缀最大值比后缀最大值小，那么左指针向右移动，否则右指针向左移动
    n = len(height)
    l = 0
    r = n - 1
    pre_max = 0
    suf_max = 0
    ans = 0

    while l <= r:
        pre_max = max(pre_max, height[l])
        suf_max = max(suf_max, height[r])
        if pre_max < suf_max:
            ans += pre_max - height[l]
            l += 1
        else:
            ans += suf_max - height[r]
            r -= 1

    return ans
