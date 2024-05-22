#include "bst.h"
#include <functional>
#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>
#include <initializer_list>

std::ostream &operator<<(std::ostream &os, const BST::Node &node) {
    std::ostringstream oss;
    oss << std::setw(10) << &node << " => "
        << std::setw(10) << "value: " << std::setw(10) << node.value
        << std::setw(10) << "left: " << std::setw(10) << node.left
        << std::setw(10) << "right: " << std::setw(10) << node.right;
    os << oss.str();
    return os;
}

bool operator<(const BST::Node &node, int val) {
    return node.value < val;
}

bool operator>(const BST::Node &node, int val) {
    return node.value > val;
}

bool operator<=(const BST::Node &node, int val) {
    return node.value <= val;
}

bool operator>=(const BST::Node &node, int val) {
    return node.value >= val;
}

bool operator==(const BST::Node &node, int val) {
    return node.value == val;
}

bool operator<(int val, const BST::Node &node) {
    return val < node.value;
}

bool operator>(int val, const BST::Node &node) {
    return val > node.value;
}

bool operator<=(int val, const BST::Node &node) {
    return val <= node.value;
}

bool operator>=(int val, const BST::Node &node) {
    return val >= node.value;
}

bool operator==(int val, const BST::Node &node) {
    return val == node.value;
}

BST::Node *add_node_(BST::Node *root, int val) {
    if (root == nullptr) {
        BST::Node *node = new BST::Node{val, nullptr, nullptr};
        return node;
    }
    if (val == root->value) return root;
    if (val < root->value) root->left = add_node_(root->left, val);
    if (val > root->value) root->right = add_node_(root->right, val);
    return root;
}

bool BST::add_node(int val) {
    BST::Node **res = find_node(val);
    if (res != nullptr) {
        return false;
    }
    delete res;
    this->root_ = add_node_(this->root_, val);
    this->size_ += 1;
    return true;
}

BST::Node **BST::find_node(int val) const {
    BST::Node *iter = root_;
    while (iter != nullptr) {
        if (val == iter->value) {
            BST::Node **ret = new BST::Node *;
            *ret = &*iter;
            return ret;
        } else if (val < iter->value) {
            iter = iter->left;
        } else if (val > iter->value) {
            iter = iter->right;
        }
    }
    return nullptr;
}

void BST::bfs(std::function<void(BST::Node *&)> callback) {
    if (root_ == nullptr) return;
    std::queue<BST::Node *> qu;
    qu.push(root_);
    while (!qu.empty()) {
        for (auto sz = qu.size(); sz != 0; --sz) {
            BST::Node *node = qu.front();
            qu.pop();
            if (node->left != nullptr) qu.push(node->left);
            if (node->right != nullptr) qu.push(node->right);
            callback(node);
        }
    }
}

std::size_t BST::length() const {
    return this->size_;
}

std::ostream &operator<<(std::ostream &os, const BST &bst) {
    std::ostringstream oss;
    oss << std::string(80, '*') << std::endl;
    (const_cast<BST &>(bst)).bfs([&oss](BST::Node *&node) {
        oss << *node << std::endl;
    });
    oss << "binary search tree size: " << bst.length() << std::endl;
    oss << std::string(80, '*') << std::endl;
    os << oss.str();
    return os;
}

BST::Node **BST::find_parrent(int val) const {
    if (root_ == nullptr) return nullptr;
    BST::Node *iter = root_, *parent = nullptr;
    while (iter != nullptr) {
        if (val == iter->value) {
            BST::Node **ret = new BST::Node *;
            *ret = (parent == nullptr) ? nullptr : &*parent;
            return ret;
        } else if (val < iter->value) {
            parent = iter;
            iter = iter->left;
        } else if (val > iter->value) {
            parent = iter;
            iter = iter->right;
        }
    }
    return nullptr;
}

BST::Node **BST::find_successor(int val) const {
    BST::Node **res = find_node(val);
    if (res == nullptr) {
        return nullptr;
    }
    BST::Node *node = *res;
    delete res;
    if (node->left == nullptr) return nullptr;
    BST::Node *iter = node->left;
    while (iter->right != nullptr) iter = iter->right;
    BST::Node **ret = new BST::Node *;
    *ret = &*iter;
    return ret;
}

bool delete_node_(BST::Node *&root, int val, BST &&bst) {
    if (root == nullptr) return false;
    if (val < root->value) {
        return delete_node_(root->left, val, std::move(bst));
    }
    if (val > root->value) {
        return delete_node_(root->right, val, std::move(bst));
    }
    if (root->left == nullptr && root->right == nullptr) {
        delete root;
        root = nullptr;
        return true;
    }
    if (root->left == nullptr ^ root->right == nullptr) {
        if (root->left == nullptr) {
            BST::Node *tmp = root->right;
            delete root;
            root = tmp;
        }
        if (root->right == nullptr) {
            BST::Node *tmp = root->left;
            delete root;
            root = tmp;
        }
        return true;
    }
    BST::Node **successor = bst.find_successor(val);
    root->value = (*successor)->value;
    delete_node_(root->left, (*successor)->value, std::move(bst));
    return true;
}

bool BST::delete_node(int val) {
    bool res = delete_node_(root_, val, std::move(*this));
    if (res) {
        this->size_ -= 1;
    }
    return res;
}

BST &BST::operator++() {
    bfs([](BST::Node *&node) {
        node->value += 1;
    });
    return *this;
}

const BST BST::operator++(int) {
    BST old = *this;
    ++*this;
    return old;
}

BST::Node *deep_copy(const BST::Node *const root) {
    if (root == nullptr) return nullptr;
    BST::Node *node = new BST::Node;
    node->value = root->value;
    node->left = deep_copy(root->left);
    node->right = deep_copy(root->right);
    return node;
}

BST::BST(const BST &bst) {
    this->size_ = bst.size_;
    this->root_ = deep_copy(bst.root_);
}

BST::BST(BST &&bst) {
    this->root_ = nullptr;
    this->size_ = 0;
    std::swap(this->root_, bst.root_);
    std::swap(this->size_, bst.size_);
}

BST::~BST() {
    bfs([](BST::Node *&node) {
        delete node;
    });
}

BST &BST::operator=(const BST &bst) {
    if (this == &bst) return *this;
    bfs([](BST::Node *&node) {
        delete node;
    });
    this->size_ = bst.size_;
    this->root_ = deep_copy(bst.root_);
    return *this;
}

BST &BST::operator=(BST &&bst) {
    if (this == &bst) return *this;
    std::swap(this->root_, bst.root_);
    std::swap(this->size_, bst.size_);
    return *this;
}

BST::BST(std::initializer_list<int> lst) {
    std::for_each(lst.begin(), lst.end(), [&](const int el) {
        add_node(el);
    });
}