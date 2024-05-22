#ifndef BST_H
#define BST_H

#include <functional>
#include <iostream>
#include <initializer_list>

class BST {
public:
    struct Node {
        Node(int v, Node *l, Node *r)
            : value(v), left(l), right(r) {}
        Node() : Node(0, nullptr, nullptr) {}
        Node(const Node &node)
            : value(node.value), left(node.left), right(node.right) {}

        int value;
        Node *left;
        Node *right;
    };

    BST() : root_(nullptr), size_(0) {}
    BST(const BST &bst);
    BST(BST &&bst);
    BST(std::initializer_list<int> lst);
    ~BST();
    BST &operator=(const BST &bst);
    BST &operator=(BST &&bst);

    Node *get_root() const { return this->root_; }
    void bfs(std::function<void(BST::Node *&)> callback);
    std::size_t length() const;
    bool add_node(int val);
    Node **find_node(int val) const;
    Node **find_parrent(int val) const;
    Node **find_successor(int val) const;
    bool delete_node(int val);

    BST& operator++();
    const BST operator++(int);

private:
    Node *root_;
    std::size_t size_;
};

std::ostream &operator<<(std::ostream &os, const BST::Node &node);
bool operator<(const BST::Node &node, int val);
bool operator>(const BST::Node &node, int val);
bool operator<=(const BST::Node &node, int val);
bool operator>=(const BST::Node &node, int val);
bool operator==(const BST::Node &node, int val);
bool operator<(int val, const BST::Node &node);
bool operator>(int val, const BST::Node &node);
bool operator<=(int val, const BST::Node &node);
bool operator>=(int val, const BST::Node &node);
bool operator==(int val, const BST::Node &node);
std::ostream &operator<<(std::ostream &os, const BST &bst);

#endif  // BST_H