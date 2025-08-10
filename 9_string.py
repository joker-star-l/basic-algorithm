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
    cnt = 0  # 相同前后缀长度，pattern[cnt] 就是前缀的下一个字符（真前后缀，不包含自身！！！）
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


# Manacher 算法
# 5. 最长回文子串
# https://leetcode.cn/problems/longest-palindromic-substring
def longestPalindrome(self, s: str) -> str:
    # Manacher 模板
    # 将 s 改造为 t，这样就不需要讨论 len(s) 的奇偶性，因为新串 t 的每个回文子串都是奇回文串（都有回文中心）
    # s 和 t 的下标转换关系：
    # (si + 1) * 2 = ti
    # ti / 2 - 1 = si
    # ti 为偶数，对应奇回文串（从 2 开始）
    # ti 为奇数，对应偶回文串（从 3 开始）
    t = "#".join("^" + s + "$")  # aba -> ^#a#b#a#$

    # 定义一个奇回文串的回文半径 = (长度 + 1) / 2，即保留回文中心，去掉一侧后的剩余字符串的长度
    # half_len[i] 表示在 t 上的以 t[i] 为回文中心的最长回文子串的回文半径
    # 即 [i - half_len[i] + 1, i + half_len[i] - 1] 是 t 上的一个回文子串
    half_len = [0] * (len(t) - 2)
    half_len[1] = 1
    # box_r 表示当前右边界下标最大的回文子串的右边界下标
    # box_m 为该回文子串的中心位置，二者的关系为 box_r = box_m + half_len[mid] - 1
    box_m = box_r = max_i = 0
    for i in range(2, len(half_len)):
        hl = 1
        if i <= box_r:
            # 记 i 关于 box_m 的对称位置 i' = box_m * 2 - i
            # 若以 i' 为中心的最长回文子串范围超出了以 box_m 为中心的回文串的范围（即 i + half_len[i'] - 1 >= box_r）
            # 则 half_len[i] 应先初始化为已知的回文半径 box_r - i + 1，然后再继续暴力匹配
            # 否则 half_len[i] 与 half_len[i'] 相等
            hl = min(half_len[box_m * 2 - i], box_r - i + 1)

        # 暴力扩展
        # 算法的复杂度取决于这部分执行的次数
        # 由于扩展之后 box_r 必然会更新（右移），且扩展的的次数就是 box_r 右移的次数
        # 因此算法的复杂度 = O(len(t)) = O(n)
        while t[i - hl] == t[i + hl]:
            hl += 1
            box_m, box_r = i, i + hl - 1

        half_len[i] = hl
        if hl > half_len[max_i]:
            max_i = i

    hl = half_len[max_i]
    # 注意 t 上的最长回文子串的最左边和最右边都是 '#'
    # 所以要对应到 s，最长回文子串的下标是从 max_i - hl + 2 到 max_i + hl - 2
    # 结合上文的下标转换关系，得到其在 s 上的下标范围是从 (max_i - hl) / 2 到 (max_i + hl) / 2 - 2
    return s[(max_i - hl) // 2 : (max_i + hl) // 2 - 1]
