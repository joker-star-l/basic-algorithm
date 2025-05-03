from typing import List

# KMP
# 28. 找出字符串中第一个匹配项的下标
# https://leetcode.cn/problems/find-the-index-of-the-first-occurrence-in-a-string
def strStr(self, haystack: str, needle: str) -> int:
    pos = kmp(haystack, needle)
    return pos[0] if pos else -1

# 在文本串 text 中查找模式串 pattern，返回所有成功匹配的位置（pattern[0] 在 text 中的下标）
def kmp(text: str, pattern: str) -> List[int]:
    m = len(pattern)
    pi = [0] * m
    cnt = 0  # 相同前后缀长度，pattern[cnt] 就是该前后缀的最后一个字符
    for i in range(1, m):
        b = pattern[i]
        while cnt and pattern[cnt] != b:
            cnt = pi[cnt - 1]
        if pattern[cnt] == b:
            cnt += 1
        pi[i] = cnt

    pos = []
    cnt = 0  # 相同前后缀长度，pattern[cnt] 就是该前后缀的最后一个字符
    for i, b in enumerate(text):
        while cnt and pattern[cnt] != b:
            cnt = pi[cnt - 1]
        if pattern[cnt] == b:
            cnt += 1
        if cnt == m:
            pos.append(i - m + 1)
            cnt = pi[cnt - 1]
    return pos
