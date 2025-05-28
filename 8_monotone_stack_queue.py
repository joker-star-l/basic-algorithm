from typing import List
from collections import deque

# 单调栈
# 涉及上/下一个更大/小元素的计算
# 及时去掉无用数据，保证栈中数据有序
# 739. 每日温度
# https://leetcode.cn/problems/daily-temperatures
def dailyTemperatures(temperatures: List[int]) -> List[int]:
    n = len(temperatures)
    ans = [0] * n
    stk = [] # 单调递减栈，存储索引
    for i, t in enumerate(temperatures):
        while stk and t > temperatures[stk[-1]]:
            j = stk.pop()
            ans[j] = i - j
        stk.append(i)
    return ans


# 单调队列
# 及时去掉无用数据，保证双端队列有序
# 239. 滑动窗口最大值
# https://leetcode.cn/problems/sliding-window-maximum
def maxSlidingWindow(nums: List[int], k: int) -> List[int]:
    dq = deque()  # 单调递减队列
    ans = []
    for i, x in enumerate(nums):
        while dq and x >= nums[dq[-1]]:
            dq.pop()
        dq.append(i)  # 入
        if (i - dq[0] + 1 > k):
            dq.popleft()  # 出
        if i >= k - 1:  # 记录答案
            ans.append(nums[dq[0]])

    return ans
