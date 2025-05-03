from typing import List

# 在有序整数数组上的二分查找的四种类型
# 1. >= x
# 2. > x 等价于 >= x + 1
# 3. <= x 等价于 idx(> x) - 1
# 4. < x 等价于 idx(>= x) - 1

# 34. 在排序数组中查找元素的第一个和最后一个位置
# https://leetcode.cn/problems/find-first-and-last-position-of-element-in-sorted-array
def searchRange(self, nums: List[int], target: int) -> List[int]:
    start = lower_bound(nums, target)
    if start == len(nums) or nums[start] != target:  # 数组为空，或者没有找到 target
        return [-1, -1]
    end = lower_bound(nums, target + 1) - 1
    return [start, end]

def lower_bound(nums: List[int], target: int) -> int:
    # 返回第一个大于等于 target 的元素的下标
    l = 0
    r = len(nums) - 1
    while l <= r:
        mid = (l + r) // 2
        if nums[mid] < target: # 因为要返回第一个元素，所以不能是 l <= r
            l = mid + 1  # [mid + 1, r]
        else:
            r = mid - 1  # [l, mid - 1]
    return l
