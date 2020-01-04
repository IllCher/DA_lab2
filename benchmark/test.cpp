#include <iostream>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <chrono>
#include <map>
#include <vector>
typedef unsigned long long ull;
typedef struct {
    char* key;
    ull val;
} kv;
class TAvl {
private:
    struct TNode {
        friend class TAvl;

        char *key;
        ull value;
        ull height;
        TNode *left;
        TNode *right;

        TNode() : value(), height{1}, left{nullptr}, right{nullptr} {
            key = (char *) malloc(257);
        };

        TNode(char *k, ull v) : value{v}, height{1}, left{nullptr}, right{nullptr} {
            key = (char *) malloc(257);
            strcpy(key, k);
        };

        ~TNode() {
            free(key);
        }
    };

    TNode *root;

    ull Height(const TNode *node) {
        return node != nullptr ? node->height : 0;
    }

    int Balance(const TNode *node) {
        return Height(node->left) - Height(node->right);
    }

    void FixHeight(TNode *node) {
        node->height = std::max(Height(node->left), Height(node->right)) + 1;
    }

    TNode *RotateLeft(TNode *p) {
        TNode *q = p->right;
        p->right = q->left;
        q->left = p;
        FixHeight(p);
        FixHeight(q);
        return q;
    }

    TNode *RotateRight(TNode *p) {
        TNode *q = p->left;
        p->left = q->right;
        q->right = p;
        FixHeight(p);
        FixHeight(q);
        return q;
    }

    TNode *BigRotateLeft(TNode *p) {
        p->right = RotateRight(p->right);
        return RotateLeft(p);
    }

    TNode *BigRotateRight(TNode *p) {
        p->left = RotateLeft(p->left);
        return RotateRight(p);
    }

    TNode *Rebalance(TNode *node) {
        if (node == nullptr) {
            return nullptr;
        }
        FixHeight(node);
        int balanceRes = Balance(node);
        if (balanceRes == -2) {
            if (Balance(node->right) == 1) {
                return BigRotateLeft(node);
            }
            return RotateLeft(node);
        } else if (balanceRes == 2) {
            if (Balance(node->left) == -1) {
                return BigRotateRight(node);
            }
            return RotateRight(node);
        }
        return node;
    }

    TNode *Insert(TNode *node, char *k, ull v) {
        if (node == nullptr) {
            try {
                node = new TNode(k, v);
            }
            catch (std::bad_alloc &e) {
                std::cout << "ERROR: " << e.what() << "\n";
                return nullptr;
            }
            return node;
        }
        int res = strcmp(k, node->key);
        if (res < 0) {
            node->left = Insert(node->left, k, v);
        } else if (res > 0) {
            node->right = Insert(node->right, k, v);
        }
        return Rebalance(node);
    }

    TNode *RemoveMin(TNode *node, TNode *tmp) {
        if (tmp->left != nullptr) {
            tmp->left = RemoveMin(node, tmp->left);
        } else {
            TNode *rightSubNode = tmp->right;
            strcpy(node->key, tmp->key);
            node->value = tmp->value;
            delete tmp;
            tmp = rightSubNode;
        }
        return Rebalance(tmp);
    }

    TNode *Remove(TNode *node, char *k) {
        if (node == nullptr) {
            return nullptr;
        }
        int res = strcmp(k, node->key);
        if (res < 0) {
            node->left = Remove(node->left, k);
        } else if (res > 0) {
            node->right = Remove(node->right, k);
        } else {
            TNode *leftSubNode = node->left;
            TNode *rightSubNode = node->right;
            if (leftSubNode == nullptr && rightSubNode == nullptr) {
                delete node;
                return nullptr;
            }
            if (rightSubNode == nullptr) {
                delete node;
                return leftSubNode;
            }
            if (leftSubNode == nullptr) {
                delete node;
                return rightSubNode;
            }
            node->right = RemoveMin(node, rightSubNode);
        }
        return Rebalance(node);
    }

    TNode *Search(TNode *node, char *k) {
        if (node == nullptr) {
            return nullptr;
        }
        TNode *tmp = node;
        while (tmp != nullptr) {
            int res = strcmp(k, tmp->key);
            if (res < 0) {
                tmp = tmp->left;
            } else if (res > 0) {
                tmp = tmp->right;
            } else {
                return tmp;
            }
        }
        return nullptr;
    }

public:
    TAvl() : root(nullptr) {};

    TNode *GetRoot() {
        return root;
    }

    void Add(char *k, ull v) {
        root = Insert(root, k, v);
    }

    void Delete(char *k) {
        root = Remove(root, k);
    }

    TNode *Find(char *k) {
        return Search(root, k);
    }

    void TreeDelete(TNode *node) {
        if (node != nullptr) {
            TreeDelete(node->left);
            TreeDelete(node->right);
            delete node;
        }
    }

    ~TAvl() {
        TreeDelete(root);
    }
};
int main(int argc, char **argv) {
    int top = 1000000;
    TAvl tree;
    kv tmp;
    tmp.key = (char *) malloc(257);
    tmp.key[256] = '\0';
    for (int i = 0; i < top; i++) {
        std::cin >> tmp.key >> tmp.val;
        tree.Add(tmp.key, tmp.val);
    }
    free(tmp.key);
    return 0;
}