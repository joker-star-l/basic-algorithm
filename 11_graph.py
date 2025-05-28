from typing import List
from math import inf
from collections import deque
from heapq import heappop, heappush

# 拓扑排序
# Kahn 算法：
# 1. 计算每个顶点的入度
# 2. 将入度为 0 的顶点加入队列
# 3. 从队列中取出顶点，将其加入拓扑排序结果
# 4. 对于每个出边，将对应的顶点的入度减 1
# 5. 如果入度减为 0，将该顶点加入队列
# 6. 重复步骤 3-5，直到队列为空

# 210. 课程表 II
# https://leetcode.cn/problems/course-schedule-ii
# 与 207. 课程表 https://leetcode.cn/problems/course-schedule 做法相同
def findOrder(numCourses: int, prerequisites: List[List[int]]) -> List[int]:
    g = [[] for _ in range(numCourses)]
    deg = [0] * numCourses  # 入度
    for a, b in prerequisites:
        g[b].append(a)
        deg[a] += 1

    q = deque()
    for i, d in enumerate(deg):
        if d == 0:
            q.append(i)

    orders = []
    while len(q) > 0:
        i = q.popleft()
        orders.append(i)
        for j in g[i]:
            deg[j] -= 1
            if deg[j] == 0:
                q.append(j)

    if len(orders) < numCourses:
        return []

    return orders


# 单源最短路 稠密图 Dijkstra 算法
# dis[i] 表示从起点到 i 的最短距离, 每次取出 dis 中最小的点
# pre[i] 表示 i 的前驱节点, 用于记录路径
# https://leetcode.cn/problems/network-delay-time
# 743. 网络延迟时间
def networkDelayTime(times: List[List[int]], n: int, k: int) -> int:
    g = [[inf for _ in range(n)] for _ in range(n)]
    for u, v, w in times:
        g[u - 1][v - 1] = w

    dis = [inf] * n
    dis[k - 1] = 0
    done = [False] * n

    pre = [-1] * n  # 前驱数组，用于存储路径，本题没用到

    ans = 0
    while True:
        x = -1
        for i in range(n):
            if not done[i] and (x < 0 or dis[i] < dis[x]):
                x = i

        if x < 0:
            return ans # 最后一次算出的最短路就是最大的

        if dis[x] == inf: # 有节点无法到达
            return -1

        ans = dis[x]
        done[x] = True

        for y, d in enumerate(g[x]):
            if dis[x] + d < dis[y]:
                dis[y] = dis[x] + d
                pre[y] = x


# 单源最短路 稀疏图 Dijkstra 算法（堆优化）
# 使用堆完成寻找最小值的操作
# https://leetcode.cn/problems/path-with-maximum-probability
# 1514. 概率最大的路径
# 最大概率转化为最短路
# 不使用堆优化会超时
def maxProbability(n: int, edges: List[List[int]], succProb: List[float], start_node: int, end_node: int) -> float:
    g = [[] for _ in range(n)]  # 邻接表
    for i in range(len(edges)):
        e = edges[i]
        p = succProb[i]
        g[e[0]].append((e[1], p))
        g[e[1]].append((e[0], p))

    prob = [0] * n
    prob[start_node] = 1
    h = [(-1, start_node)]  # 堆

    while h:
        p, x = heappop(h)
        p = -p
        if p < prob[x]:
            continue

        for y, p in g[x]:
            new_p = prob[x] * p
            if new_p > prob[y]:
                prob[y] = new_p
                heappush(h, (-new_p, y))

    return prob[end_node]


# 多源最短路 Floyd 算法
# dis[i][j] 表示从 i 到 j 的最短距离
# pre[i][j] 表示 i 到 j 的最短路中 j 的前驱节点, 用于记录路径
# 1334. 阈值距离内邻居最少的城市
# https://leetcode.cn/problems/find-the-city-with-the-smallest-number-of-neighbors-at-a-threshold-distance
def findTheCity(n: int, edges: List[List[int]], distanceThreshold: int) -> int:
    dis = [[inf] * n for _ in range(n)]
    pre = [[-1] * n for _ in range(n)]  # 前驱数组，用于存储路径，本题没用到

    for x, y, d in edges:
        dis[x][y] = d
        dis[y][x] = d
        pre[x][y] = x
        pre[y][x] = y

    for i in range(n):
        dis[i][i] = 0

    for k in range(n):  # 中间节点
        for i in range(n):
            for j in range(n):
                if dis[i][j] > dis[i][k] + dis[k][j]:
                    dis[i][j] = dis[i][k] + dis[k][j]
                    pre[i][j] = pre[k][j]

    ans = -1
    min_cnt = inf
    for i in range(n):
        cnt = 0
        for j in range(n):
            if j != i and dis[i][j] <= distanceThreshold:
                cnt += 1
        if cnt <= min_cnt:
            min_cnt = cnt
            ans = i

    return ans


# Floyd 算法动态加单向边
def addEdge(edge: List[int], dis: List[List[int]], pre: List[List[int]]):
    x, y, d = edge
    if d >= dis[x][y]:
        return

    dis[x][y] = d  # 更新距离
    pre[x][y] = x  # 更新前驱节点

    n = len(dis)
    for i in range(n):
        for j in range(n):
            if dis[i][j] > dis[i][x] + d + dis[y][j]:  # 如果通过 x 和 y 可以找到更短的路径
                dis[i][j] = dis[i][x] + d + dis[y][j]
                pre[i][j] = pre[y][j]


# 最小生成树 稠密图 Prim 算法（加点法）
# 每次取出当前未加入生成树的点中，距离生成树最近的点
# 1584. 连接所有点的最小费用
# https://leetcode.cn/problems/min-cost-to-connect-all-points
def minCostConnectPoints(points: List[List[int]]) -> int:
    n = len(points)
    g = [[inf] * n for _ in range(n)]

    for i in range(n):
        for j in range(n):
            g[i][j] = abs(points[i][0] - points[j][0]) + abs(points[i][1] - points[j][1])  # 不需要考虑方向

    min_dis = [inf] * n
    min_dis[0] = 0
    in_mst = [False] * n

    ans = 0
    while True:
        x = -1
        for i in range(n):
            if not in_mst[i] and (x < 0 or min_dis[i] < min_dis[x]):
                x = i

        if x < 0:
            return ans

        in_mst[x] = True
        ans += min_dis[x]

        for y, d in enumerate(g[x]):
            if not in_mst[y] and min_dis[y] > d:
                min_dis[y] = d


# 最小生成树 稀疏图 Kruskal 算法（加边法）
# 每次取出当前未加入生成树的边中，权值最小的边，要求该边的两个顶点未连通
# 1584. 连接所有点的最小费用
# https://leetcode.cn/problems/min-cost-to-connect-all-points
def minCostConnectPoints(points: List[List[int]]) -> int:
    n = len(points)
    edges = []
    for i in range(n):
        for j in range(n):
            d = abs(points[i][0] - points[j][0]) + abs(points[i][1] - points[j][1])
            edges.append((d, i, j))

    edges.sort(key = lambda x : x[0])

    fa = [i for i in range(n)]  # 并查集

    ans = 0
    for d, x, y in edges:
        fx = fa_find(fa, x)
        fy = fa_find(fa, y)

        if fx != fy:
            fa[fy] = fx  # fa[fx] = fy 也可以
            ans += d

    return ans

def fa_find(fa, x):
    if fa[x] != x:
        fa[x] = fa_find(fa, fa[x])
    return fa[x]