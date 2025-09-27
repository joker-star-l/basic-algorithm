// copied from https://leetcode.cn/problems/binary-tree-inorder-traversal/solutions/34581/die-dai-fa-by-jason-2/ and modified
#include <vector>

struct Node {
    int value;
    Node* left;
    Node* right;

    Node(int val) : value(val), left(nullptr), right(nullptr) {}
};

// Preorder Traversal (Root, Left, Right)
// 144. 二叉树的前序遍历 https://leetcode.cn/problems/binary-tree-preorder-traversal
void preorder(Node* root, std::vector<int>& result) {
    Node* curr = root;
    std::vector<Node*> stk;
    while (curr || !stk.empty()) {
        while (curr) {
            result.push_back(curr->value);
            stk.push_back(curr->right);
            curr = curr->left;
        }
        curr = stk.back();
        stk.pop_back();
    }
}

// Inorder Traversal (Left, Root, Right)
// 94. 二叉树的中序遍历 https://leetcode.cn/problems/binary-tree-inorder-traversal
void inorder(Node* root, std::vector<int>& result) {
    Node* curr = root;
    std::vector<Node*> stk;
    while (curr || !stk.empty()) {
        while (curr) {
            stk.push_back(curr);
            curr = curr->left;
        }
        curr = stk.back();
        stk.pop_back();
        result.push_back(curr->value);
        curr = curr->right;
    }
}

struct NodeState {
    Node* node;
    bool done; // whether the right subtree has been processed
    NodeState(Node* n, bool v) : node(n), done(v) {}
};

// Postorder Traversal (Left, Right, Root)
// 145. 二叉树的后序遍历 https://leetcode.cn/problems/binary-tree-postorder-traversal
void postorder(Node* root, std::vector<int>& result) {
    Node* curr = root;
    std::vector<NodeState> stk;
    while (curr || !stk.empty()) {
        while (curr) {
            stk.emplace_back(curr, false);
            curr = curr->left;
        }
        while (!stk.empty() && stk.back().done) {
            result.push_back(stk.back().node->value);
            stk.pop_back();
        }
        if (!stk.empty()) {
            curr = stk.back().node->right;
            stk.back().done = true;
        }
    }
}
