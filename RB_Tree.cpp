//
// Created by DevilInChina on 2018/9/21.
//
#include <functional>
#include <iostream>
#include <cstdio>
#include <set>
#include <queue>

template <class T,class Compare = std::less<T> >
class RB_Tree {
protected:
#define BLACK false
#define RED true

    struct Node {
        Node() {
            Color = RED;
            father = left = right = nullptr;
        }

        explicit Node(const T &Copy) : element(Copy) {
            Color = RED;
            father = left = right = nullptr;
        }

        T element;
        bool Color;
        Node *father;
        Node *left, *right;
    };

public:

    RB_Tree() {
        soldier = new Node;
        soldier->Color = BLACK;
        root = soldier;
        soldier->left = soldier->right = soldier;
        soldier->father = nullptr;
    }

    class iterator {
    public:
        iterator() {
            PtrNode = nullptr;
        }

        explicit iterator(Node *Copy) {
            PtrNode = Copy;
        }

        iterator(const iterator &Copy) {
            PtrNode = Copy.PtrNode;
        }

        bool operator==(const iterator &Temp) {
            return PtrNode == Temp.PtrNode;
        }

        bool operator!=(const iterator &Temp) {
            return PtrNode != Temp.PtrNode;
        }

        const T &operator*() const {
            return PtrNode->element;
        }

        T &operator*() {
            return PtrNode->element;
        }

        iterator operator++() {
            if (PtrNode->right->father) {
                PtrNode = PtrNode->right;
                while (PtrNode->left->father) {
                    PtrNode = PtrNode->left;
                }
                return *this;
            } else {
                while (PtrNode->father and PtrNode != PtrNode->father->left) {
                    PtrNode = PtrNode->father;
                }
                PtrNode = PtrNode->father;
                return *this;
            }
        }

        iterator operator--() {
            if (PtrNode->left->father) {
                PtrNode = PtrNode->left;
                while (PtrNode->right->father) {
                    PtrNode = PtrNode->right;
                }
                return *this;
            } else {
                while (PtrNode->father and PtrNode != PtrNode->father->right) {
                    PtrNode = PtrNode->father;
                }
                PtrNode = PtrNode->father;
                return *this;
            }
        }

    private:
        friend RB_Tree<T>;
        Node *PtrNode;
    };

    bool empty() {
        return root == soldier;
    }

    iterator rebegin() {
        Node *ptr = soldier;
        if (root) {
            ptr = root;
            while (ptr->right != soldier) {
                ptr = ptr->right;
            }
        }
        return iterator(ptr);
    }

    iterator begin() {
        Node *ptr = soldier;
        if (root) {
            ptr = root;
            while (ptr->left != soldier) {
                ptr = ptr->left;
            }
        }
        return iterator(ptr);
    }

    iterator end() {
        return iterator(nullptr);
    }

    void insert(const T &ele) {
        if (root == soldier) {
            root = new Node(ele);
            root->father = soldier;
            root->right = root->left = soldier;
            root->Color = BLACK;
        } else {
            Node *it = root, *pre = root;
            while (it != soldier) {
                pre = it;
                if (cmp(ele, it->element)) {
                    it = it->left;
                } else if (cmp(it->element, ele)) {
                    it = it->right;
                } else {
                    return;
                }
            }
            auto data = new Node(ele);
            data->left = data->right = soldier;
            data->father = pre;
            if (cmp(ele, pre->element)) {
                pre->left = data;
            } else {
                pre->right = data;
            }
            if (pre->Color and data->Color) {
                FixUp(data);
            }
        }
    }

    iterator find(const T&temp){
        Node *ptr = root;
        while (ptr!=soldier){
            if(cmp(ptr->element,temp)){
                ptr = ptr->left;
            }
            else if(cmp(temp,ptr->element)){
                ptr = ptr->right;
            }
            else return iterator(ptr);
        }
        return iterator(nullptr);
    }

    void remove(typename RB_Tree<T>::iterator &it) {
        if (it->PtrNode) {
            Remove(it->PtrNode);
        }
    }

    void show() {
        Mid(root);
    }

private:

    void LeftRotate(Node *x) {///assume x.right is not nill
        if (x == soldier)return;
        Node *y = x->right;
        x->right = y->left;
        if (y->left != soldier)y->left->father = x;
        y->father = x->father;
        if (x->father == soldier) {
            root = y;
            y->father = soldier;
        } else if (x == x->father->left) {
            x->father->left = y;
        } else {
            x->father->right = y;
        }
        y->left = x;
        x->father = y;
    }

    void RightRotate(Node *x) {///assume x.right is not nill
        if (x == soldier)return;
        Node *y = x->left;
        x->left = y->right;
        if (y->right != soldier)y->right->father = x;
        y->father = x->father;
        if (x->father == soldier) {
            root = y;
            y->father = soldier;
        } else if (x == x->father->left) {
            x->father->left = y;
        } else {
            x->father->right = y;
        }
        y->right = x;
        x->father = y;
    }

    void FixUp(Node *z) {
        Node *y;
        while (z->father->Color == RED) {
            if (z->father == z->father->father->left) {
                y = z->father->father->right;
                if (y->Color == RED) {
                    z->father->Color = BLACK;
                    y->Color = BLACK;
                    z->father->father->Color = RED;
                    z = z->father->father;
                } else if (z == z->father->right) {
                    z = z->father;
                    LeftRotate(z);
                } else {
                    z->father->Color = BLACK;
                    z->father->father->Color = RED;
                    RightRotate(z->father->father);
                }
            } else if (z->father == z->father->father->right) {
                y = z->father->father->left;
                if (y->Color == RED) {
                    z->father->Color = BLACK;
                    y->Color = BLACK;
                    z->father->father->Color = RED;
                    z = z->father->father;
                } else if (z == z->father->left) {
                    z = z->father;
                    RightRotate(z);
                } else {
                    z->father->Color = BLACK;
                    z->father->father->Color = RED;
                    LeftRotate(z->father->father);
                }
            }
        }
        root->Color = BLACK;
    }

    Node *TreeMin(Node *ptr) {
        while (ptr->left != soldier) {
            ptr = ptr->left;
        }
        return ptr;
    }

    Node *TreeMax(Node *ptr) {
        while (ptr->right != soldier) {
            ptr = ptr->right;
        }
        return ptr;
    }

    void TransPlant(Node *_f, Node *_t) {
        if (_f->father == soldier) {
            root = _t;
        } else if (_f == _f->father->left) {
            _f->father->left = _t;
        } else {
            _f->father->right = _t;
        }
        _t->father = _f->father;
    }

    void DeleteFixUp(Node *x) {
        Node *w;
        while (x != root and x->Color == BLACK) {
            if (x == x->father->left) {
                w = x->father->right;
                if (w->Color == RED) {
                    w->Color = BLACK;
                    x->father->Color = RED;
                    LeftRotate(x->father);
                    w = x->father->right;
                }
                if (w->left->Color == BLACK and w->right->Color == BLACK) {
                    w->Color = RED;
                    x = x->father;
                } else if (w->right->Color == BLACK) {
                    w->left->Color = BLACK;
                    w->Color = RED;
                    RightRotate(w);
                    w = x->father->right;
                } else {
                    w->Color = x->father->Color;
                    x->father->Color = BLACK;
                    w->right->Color = BLACK;
                    LeftRotate(x->father);
                    x = root;
                }
            } else {
                w = x->father->left;
                if (w->Color == RED) {
                    w->Color = BLACK;
                    x->father->Color = RED;
                    RightRotate(x->father);
                    w = x->father->left;///x未被更新
                }
                if (w->right->Color == BLACK and w->left->Color == BLACK) {
                    w->Color = RED;
                    x = x->father;
                } else if (w->left->Color == BLACK) {
                    w->right->Color = BLACK;
                    w->Color = RED;
                    LeftRotate(w);
                    w = x->father->left;
                } else {
                    w->Color = x->father->Color;
                    x->father->Color = BLACK;
                    w->left->Color = BLACK;
                    RightRotate(x->father);
                    x = root;
                }
            }
        }
    }

    void Remove(Node *z) {
        bool originColor = z->Color;
        Node *y = z, *x;
        if (z->left == soldier) {
            x = z->right;
            TransPlant(z, z->right);
        } else if (z->right == soldier) {
            x = z->left;
            TransPlant(z, z->left);
        } else {
            y = TreeMin(z->right);
            x = y->right;
            originColor = y->Color;
            if (y->father == z) {
                if (x->father)x->father = y;
            } else {
                TransPlant(y, y->right);
                y->right = z->right;
                y->right->father = y;
            }
            TransPlant(z, y);
            y->left = z->left;
            y->left->father = y;
            y->Color = z->Color;
        }
        delete z;
        if (originColor == BLACK) {
            DeleteFixUp(x);
        }
    }

    Compare cmp;
    Node *root;
    Node *soldier;
};