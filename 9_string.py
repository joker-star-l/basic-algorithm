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
    cnt = 0  # 相同前后缀长度，pattern[cnt] 就是前缀的下一个字符
    for i in range(1, m):
        b = pattern[i]
        while cnt and pattern[cnt] != b:
            cnt = pi[cnt - 1]
        if pattern[cnt] == b:
            cnt += 1
        pi[i] = cnt

    pos = []
    cnt = 0  # 相同前后缀长度，pattern[cnt] 就是前缀的下一个字符
    for i, b in enumerate(text):
        while cnt and pattern[cnt] != b:
            cnt = pi[cnt - 1]
        if pattern[cnt] == b:
            cnt += 1
        if cnt == m:
            pos.append(i - m + 1)
            cnt = pi[cnt - 1]
    return pos


# 字符串哈希
# 对于字符串 s，h(s) = sum(s[i] * P^i) % M
# P 是一个质数，通常取 131 或 13331
# M 是一个大整数，通常取 ULL
# 28. 找出字符串中第一个匹配项的下标
# https://leetcode.cn/problems/find-the-index-of-the-first-occurrence-in-a-string
def strStr(self, haystack: str, needle: str) -> int:
    m = len(needle)
    n = len(haystack)
    if m > n:
        return -1

    p = [1] * (n + 1)
    h_n = [0] * (n + 1)
    h_m = [0] * (m + 1)
    init(haystack, p, h_n)
    init(needle, p, h_m)
    
    for i in range(n - m + 1):
        if get(h_n, p, i, i + m - 1) == h_m[m]:
            return i
    return -1

P = 131

def init(s: str, p: List[int], h: List[int]):
    n = len(s)
    for i in range(n):
        p[i + 1] = p[i] * P
        h[i + 1] = h[i] * P + ord(s[i])

def get(h: List[int], p: List[int], l: int, r: int) -> int:
    return h[r + 1] - h[l] * p[r - l + 1]  # 线性
