// copied from https://leetcode.cn/problems/binary-tree-inorder-traversal/ and modified
#include <vector>

struct Node {
    int value;
    Node* left;
    Node* right;

    Node(int val) : value(val), left(nullptr), right(nullptr) {}
};

// Preorder Traversal (Root, Left, Right)
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
void postorder(Node* root, std::vector<int>& result) {
    NodeState curr(root, false);
    std::vector<NodeState> stk;
    while (curr.node || !stk.empty()) {
        while (curr.node) {
            stk.push_back(curr);
            curr = NodeState(curr.node->left, false);
        }
        while (!stk.empty() && stk.back().done) {
            result.push_back(stk.back().node->value);
            stk.pop_back();
        }
        if (!stk.empty()) {
            curr = NodeState(stk.back().node->right, false);
            stk.back().done = true;
        }
    }
}
