from typing import Optional

# 206. 反转链表
# https://leetcode.cn/problems/reverse-linked-list
class ListNode:
    def __init__(self, val=0, next=None):
        self.val = val
        self.next = next

def reverseList(self, head: Optional[ListNode]) -> Optional[ListNode]:
    prev = None
    curr = head
    while curr:
        next = curr.next  # 首先记录下一个节点
        curr.next = prev
        prev = curr # prev 是已反转部分的头节点
        curr = next # curr 是未反转部分的头节点
    return prev


# 快慢指针
# 876. 链表的中间结点
# https://leetcode.cn/problems/middle-of-the-linked-list
def middleNode(self, head: Optional[ListNode]) -> Optional[ListNode]:
    slow = head
    fast = head
    while fast and fast.next:
        slow = slow.next
        fast = fast.next.next
    return slow # 链表长度为奇数时，返回的是中间节点；为偶数时，返回的是第二个中间结点


# 快慢指针
# 141. 环形链表 
# https://leetcode.cn/problems/linked-list-cycle
# 142. 环形链表 II 
# https://leetcode.cn/problems/linked-list-cycle-ii
# 发现 slow 与 fast 相遇时，我们再额外使用一个指针 ptr。起始，它指向链表头部；随后，它和 slow 每次向后移动一个位置。最终，它们会在入环点相遇。


# 删除节点
# 19. 删除链表的倒数第 N 个结点 
# https://leetcode.cn/problems/remove-nth-node-from-end-of-list
def removeNthFromEnd(self, head: Optional[ListNode], n: int) -> Optional[ListNode]:
    dummy = ListNode(0, head)
    left = dummy
    right = dummy
    for _ in range(n):
        right = right.next
    while right.next: # 当 right 指向链表中的最后一个节点时，left 指向倒数第 n + 1 个节点
        left = left.next
        right = right.next

    left.next = left.next.next
    return dummy.next


# 归并排序
# 148. 排序链表
# https://leetcode.cn/problems/sort-list
def sortList(self, head: Optional[ListNode]) -> Optional[ListNode]:
    length = getListLength(head)
    dummy = ListNode(0, head)
    step = 1
    while step < length:
        prev = dummy
        curr = dummy.next
        while curr:
            head1 = curr
            head2 = splitList(head1, step)
            curr = splitList(head2, step)
            prev.next, tail = merge2Lists(head1, head2)
            prev = tail
        step *= 2
    return dummy.next

def getListLength(head: Optional[ListNode]) -> int:
    length = 0
    while head:
        length += 1
        head = head.next
    return length

# 如果链表长度 <= size，不做任何操作，返回空节点
# 如果链表长度 > size，把链表的前 size 个节点分割出来（断开连接），并返回剩余链表的头节点
def splitList(head: Optional[ListNode], size: int) -> Optional[ListNode]:
    curr = head
    for _ in range(size - 1):
        if curr is None:
            break
        curr = curr.next

    if curr is None or curr.next is None:
        return None

    next_head = curr.next
    curr.next = None

    return next_head

# 返回合并后的链表的头节点和尾节点
def merge2Lists(list1: Optional[ListNode], list2: Optional[ListNode]) -> (Optional[ListNode], Optional[ListNode]):
    dummy = ListNode()
    curr = dummy

    while list1 and list2:
        if list1.val < list2.val:
            curr.next = list1
            list1 = list1.next
        else:
            curr.next = list2
            list2 = list2.next
        curr = curr.next

    if list1:
        curr.next = list1
    elif list2:
        curr.next = list2

    while curr.next:
        curr = curr.next

    return dummy.next, curr
