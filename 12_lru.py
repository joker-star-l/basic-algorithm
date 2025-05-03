from typing import Optional, Dict

# 146. LRU 缓存
# https://leetcode.cn/problems/lru-cache
class Node:
    def __init__(self, key: int, value: int):
        self.key = key
        self.value = value
        self.prev = None
        self.next = None

class LRUCache:

    def __init__(self, capacity: int):
        self.capacity = capacity
        self.dummy = Node(0, 0)
        self.dummy.prev = self.dummy
        self.dummy.next = self.dummy
        self.hash: Dict[int, Node] = {}

    def get(self, key: int) -> int:
        node = self.get_node(key)
        return node.value if node else -1

    def put(self, key: int, value: int) -> None:
        node = self.get_node(key)
        if node:
            node.value = value
            return
        node = Node(key, value)
        self.hash[key] = node
        self.push_front(node)
        if len(self.hash) > self.capacity:
            tail = self.dummy.prev
            del self.hash[tail.key]
            self.remove(tail)

    # 获取 key 对应的节点，同时把该节点移到链表头部
    def get_node(self, key: int) -> Optional[Node]:
        if key not in self.hash:
            return None
        node = self.hash[key]
        self.remove(node)
        self.push_front(node)
        return node

    # 删除节点
    def remove(self, x: Node) -> None:
        x.prev.next = x.next
        x.next.prev = x.prev
    
    # 在链表头部插入节点
    def push_front(self, x: Node) -> None:
        x.prev = self.dummy
        x.next = self.dummy.next
        x.prev.next = x
        x.next.prev = x
