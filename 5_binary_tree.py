from typing import List, Optional
from collections import deque

# 236. 二叉树的最近公共祖先
# https://leetcode.cn/problems/lowest-common-ancestor-of-a-binary-tree
# 1 当前节点是空节点、p 节点或 q 节点，返回当前节点
# 2 其他节点：
# 2.1 左右子树都找到，返回当前节点
# 2.2 左子树找到，右子树没找到，返回左子树的结果
# 2.3 右子树找到，左子树没找到，返回右子树的结果
# 2.4 左右子树都没找到，返回空节点
class TreeNode:
    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None

def lowestCommonAncestor(self, root: 'TreeNode', p: 'TreeNode', q: 'TreeNode') -> 'TreeNode':
    return dfs(root, p, q)

def dfs(root: 'TreeNode', p: 'TreeNode', q: 'TreeNode') -> 'TreeNode':
    if root is None or root == p or root == q:
        return root

    left = dfs(root.left, p, q)
    right = dfs(root.right, p, q)

    if left and right:
        return root

    if left:
        return left

    return right


# BFS 层序遍历
# 102. 二叉树的层序遍历
# https://leetcode.cn/problems/binary-tree-level-order-traversal
def levelOrder(self, root: Optional[TreeNode]) -> List[List[int]]:
    if root is None:
        return []

    ans = []
    q = deque([root])
    while q:
        vals = []
        for _ in range(len(q)):
            node = q.popleft()
            vals.append(node.val)
            if node.left:
                q.append(node.left)
            if node.right:
                q.append(node.right)
        ans.append(vals)

    return ans
