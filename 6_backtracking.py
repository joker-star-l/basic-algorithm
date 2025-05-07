from typing import List

# 子集型回溯
# 输入角度：对于每个元素，选或不选
# 答案角度：每次必须选择一个元素，枚举选哪个
# 39. 组合总和
# https://leetcode.cn/problems/combination-sum
def combinationSum(self, candidates: List[int], target: int) -> List[List[int]]:
    ans = []
    dfs(0, target, candidates, [], ans)
    return ans

# 选或不选
def dfs(i: int, target: int, candidates: List[int], path: List[int], ans: List[List[int]]):
    if i >= len(candidates) or target < 0:
        return

    if target == 0:
        ans.append(path.copy())
        return

    # 不选
    dfs(i + 1, target, candidates, path, ans)

    # 选
    path.append(candidates[i])
    dfs(i, target - candidates[i], candidates, path, ans)
    path.pop()

# 枚举选哪个
def dfs(i: int, target: int, candidates: List[int], path: List[int], ans: List[List[int]]):
    if target < 0:
        return

    if target == 0:
        ans.append(path.copy())
        return

    for j in range(i, len(candidates)):
        path.append(candidates[j])
        dfs(j, target - candidates[j], candidates, path, ans)
        path.pop()


# 组合型回溯
# 相比于子集型回溯，组合型回溯只能选 k 个元素
# 22. 括号生成
# https://leetcode.cn/problems/generate-parentheses
def generateParenthesis(self, n: int) -> List[str]:
    ans = []
    dfs(0, 0, n, [], ans)
    return ans

# 选或不选
def dfs(i: int, open: int, n: int, path: List[int], ans: List[List[int]]):
    if i == 2 * n:
        ans.append(''.join(path))
        return
    
    if open < n:  # 选 (
        path.append('(')
        dfs(i + 1, open + 1, n, path, ans)
        path.pop()
    if i - open < open:  # 选 )
        path.append(')')
        dfs(i + 1, open, n, path, ans)
        path.pop()


# 排列型回溯
# dfs(i, S) -> {dfs(i + 1, S - x[j]) | x[j] ∈ S}
# 51. N 皇后
# https://leetcode.cn/problems/n-queens
def solveNQueens(self, n: int) -> List[List[str]]:
    on_path = [False] * n
    ans = []
    dfs(0, on_path, n, [], ans)
    return ans

diag1 = set()
diag2 = set()

def dfs(i: int, on_path: List[int], n: int, path: List[int], ans: List[List[int]]):
    if i == n:
        ans.append(['.' * p + 'Q' + '.' * (n - 1 - p) for p in path])
        return

    for j in range(n):
        if not on_path[j] and i - j not in diag1 and i + j not in diag2:
            path.append(j) # 皇后放在 (i, path[i])
            diag1.add(i - j)
            diag2.add(i + j)
            on_path[j] = True
            dfs(i + 1, on_path, n, path, ans)
            path.pop()
            on_path[j] = False
            diag1.remove(i - j)
            diag2.remove(i + j)
