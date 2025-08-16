// copied from https://leetcode.cn/problems/binary-tree-inorder-traversal/ and modified
#include <iostream>
#include <cassert>
#include "binary_tree_traversal.h"

struct Node;
extern void preorder(Node* root, std::vector<int>& result);
extern void inorder(Node* root, std::vector<int>& result);
extern void postorder(Node* root, std::vector<int>& result);

bool vector_equal(const std::vector<int>& a, const std::vector<int>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

void vector_print(const std::vector<int>& vec) {
    for (int val : vec) {
        std::cout << val << " ";
    }
    std::cout << "\n";
}

void check(Node* root, const std::vector<std::vector<int>>& expected) {
    std::cout << "Preorder Traversal" << "\n";
    std::vector<int> result;
    preorder(root, result);
    vector_print(result);
    vector_print(expected[0]);
    assert(vector_equal(result, expected[0]));

    std::cout << "Inorder Traversal" << "\n";
    result.clear();
    inorder(root, result);
    vector_print(result);
    vector_print(expected[1]);
    assert(vector_equal(result, expected[1]));
    
    std::cout << "Postorder Traversal" << "\n";
    result.clear();
    postorder(root, result);
    vector_print(result);
    vector_print(expected[2]);
    assert(vector_equal(result, expected[2]));
}

void test_full() {
    //    1
    //  2   3
    // 4 5 6 7
    Node* root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);
    root->right->left = new Node(6);
    root->right->right = new Node(7);
    check(root, {
        {1, 2, 4, 5, 3, 6, 7}, // Preorder
        {4, 2, 5, 1, 6, 3, 7}, // Inorder
        {4, 5, 2, 6, 7, 3, 1}  // Postorder
    });
}

void test_left() {
    //   1
    //  2
    // 3
    Node* root = new Node(1);
    root->left = new Node(2);
    root->left->left = new Node(3);
    check(root, {
        {1, 2, 3}, // Preorder
        {3, 2, 1}, // Inorder
        {3, 2, 1}  // Postorder
    });
}

void test_right() {
    // 1
    //  2
    //   3
    Node* root = new Node(1);
    root->right = new Node(2);
    root->right->right = new Node(3);
    check(root, {
        {1, 2, 3}, // Preorder
        {1, 2, 3}, // Inorder
        {3, 2, 1}  // Postorder
    });
}

int main() {
    std::cout << "Test Full Binary Tree:" << "\n";
    test_full();
    std::cout << "Test Left-skewed Binary Tree:" << "\n";
    test_left();
    std::cout << "Test Right-skewed Binary Tree:" << "\n";
    test_right();
    return 0;
}
