#include <iostream>
#include <string.h>
#include <algorithm>
#include <cstdio>
#include <fstream>
typedef unsigned long long ull;
struct ans {
    char cmd;
    char* key;
    ull val;
};

using namespace std;
class TAvl {
private:
    struct TNode {
        friend class TAvl;
        char* key;
        ull value;
        ull height;
        TNode* left;
        TNode* right;
        TNode() : value(), height{1}, left{nullptr}, right{nullptr} {
            key = (char*)malloc(257);
        };
        TNode(char* k, ull v) : value{v}, height{1}, left{nullptr}, right{nullptr} {
            key = (char*)malloc(257);
            strcpy(key, k);
        };
        ~TNode() {
            free(key);
        }
    };

    TNode* root;
    ull Height(const TNode *node) {
        return node != nullptr ? node->height : 0;
    }

    int Balance(const TNode *node) {
        return Height(node->left) - Height(node->right);
    }

    void Reheight(TNode *node) {
        node->height = std::max(Height(node->left), Height(node->right)) + 1;
    }

    TNode *RotateLeft(TNode *a) {
        TNode *b = a->right;
        a->right = b->left;
        b->left = a;
        Reheight(a);
        Reheight(b);
        return b;
    }

    TNode *RotateRight(TNode *a) {
        TNode *b = a->left;
        a->left = b->right;
        b->right = a;
        Reheight(a);
        Reheight(b);
        return b;
    }

    TNode *BigRotateLeft(TNode *a) {
        a->right = RotateRight(a->right);
        return RotateLeft(a);
    }

    TNode *BigRotateRight(TNode *a) {
        a->left = RotateLeft(a->left);
        return RotateRight(a);
    }

    TNode *Rebalance(TNode *node) {
        if (node == nullptr) {
            return nullptr;
        }
        Reheight(node);
        int balanceRes = Balance(node);
        if (balanceRes == -2) {
            if (Balance(node->right) == 1) {
                return BigRotateLeft(node);
            }
            return RotateLeft(node);
        }
        else if (balanceRes == 2) {
            if (Balance(node->left) == -1) {
                return BigRotateRight(node);
            }
            return RotateRight(node);
        }
        return node;
    }

    TNode *Insert(TNode *node, char* k, ull v) {
        if (node == nullptr) {
            try {
                node = new TNode(k, v);
            }
            catch (std::bad_alloc &e) {
                std::cout << "ERROR: " << e.what() << "\n";
                return nullptr;
            }
            std::cout << "OK\n";
            return node;
        }
        if (strcmp(k, node->key) < 0) {
            node->left = Insert(node->left, k, v);
        }
        else if (strcmp(k, node->key) > 0) {
            node->right = Insert(node->right, k, v);
        }
        else {
            std::cout << "Exist\n";
        }
        return Rebalance(node);
    }

    TNode *RemoveMin(TNode *node, TNode *tempNode) {
        if (tempNode->left != nullptr) {
            tempNode->left = RemoveMin(node, tempNode->left);
        }
        else {
            TNode *rightChild = tempNode->right;
            strcpy(node->key, tempNode->key);
            node->value = tempNode->value;
            delete tempNode;
            tempNode = rightChild;
        }
        return Rebalance(tempNode);
    }

    TNode *Remove(TNode *node, char* k) {
        if (node == nullptr) {
            std::cout << "NoSuchWord\n";
            return nullptr;
        }
        if (strcmp(k, node->key) < 0) {
            node->left = Remove(node->left, k);
        }
        else if (strcmp(k, node->key) > 0) {
            node->right = Remove(node->right, k);
        }
        else {
            TNode *leftChild = node->left;
            TNode *rightChild = node->right;
            if (leftChild == nullptr && rightChild == nullptr) {
                std::cout << "OK\n";
                delete node;
                return nullptr;
            }
            if (rightChild == nullptr) {
                std::cout << "OK";
                std::cout << "\n";
                delete node;
                return leftChild;
            }
            if (leftChild == nullptr) {
                std::cout << "OK\n";
                delete node;
                return rightChild;
            }
            node->right = RemoveMin(node, rightChild);
            std::cout << "OK\n";
        }
        return Rebalance(node);
    }

    TNode *Search(TNode *node, char* k) {
        if (node == nullptr) {
            std::cout << "NoSuchWord\n";
            return nullptr;
        }
        for (TNode *iter = node; iter != nullptr; ) {
            if (strcmp(k, iter->key) < 0) {
                iter = iter->left;
            }
            else if (strcmp(k, iter->key) > 0) {
                iter = iter->right;
            }
            else {
                std::cout << "OK: " << iter->value << "\n";
                return iter;
            }
        }
        std::cout << "NoSuchWord\n";
        return nullptr;
    }
public:
    TAvl() : root(nullptr) {};

    TNode* GetRoot() {
        return root;
    }

    void Add(char* k, ull v) {
        root = Insert(root, k, v);
    }

    void Delete(char* k) {
        root = Remove(root, k);
    }
    TNode *Find(char* k) {
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


    void Save(std::ostream &os, const TNode* node) {
        if (node == nullptr) {
            return;
        }
        int keySize = 0;
        int i = 0;
        while (node->key[i] != '\0') {
            i++;
        }
        keySize = i+1;
        os.write(reinterpret_cast<char*>(&keySize), sizeof(keySize));
        os.write(node->key, keySize);
        os.write((char*)&node->value, sizeof(node->value));

        bool hasLeft = node->left != nullptr;
        bool hasRight = node->right != nullptr;

        os.write(reinterpret_cast<char*>(&hasLeft), sizeof(hasLeft));
        os.write(reinterpret_cast<char*>(&hasRight), sizeof(hasRight));

        if (hasLeft) {
            Save(os, node->left);
        }
        if (hasRight) {
            Save(os, node->right);
        }
    }

    TNode *Load(std::istream &is, const TNode *node) {
        (void)(node);
        TNode* root = nullptr;
        int keySize = 0;
        is.read((char*)(&keySize), sizeof(keySize));

        if (is.gcount() == 0) {
            return root;
        }

        char* key = (char*) malloc(keySize);

        key[keySize-1] = '\0';
        is.read(key, keySize);

        ull value;

        is.read(reinterpret_cast<char*>(&value), sizeof(value));

        bool hasLeft = false;
        bool hasRight = false;
        is.read(reinterpret_cast<char*>(&hasLeft), sizeof(hasLeft));
        is.read(reinterpret_cast<char*>(&hasRight), sizeof(hasRight));

        root = new TNode();
        strcpy(root->key, key);
        root->value = value;

        if (hasLeft) {
            root->left = Load(is, root);
        } else {
            root->left = nullptr;
        }

        if (hasRight) {
            root->right = Load(is, root);
        } else {
            root->right = nullptr;
        }
        free(key);
        return root;
    }

    bool OpenFileSave(char* fileName) {
        std::ofstream os{fileName, std::ios::binary | std::ios::out};
        if (os) {
            Save(os, root);
        }
        else {
            return false;
        }
        os.close();
        return true;
    }

    bool OpenFileLoad(char* &fileName) {
        std::ifstream is{fileName, std::ios::binary | std::ios::in};
        if (is) {
            TreeDelete(root);
            root = Load(is, nullptr);
        }
        else {
            return false;
        }
        is.close();
        return true;
    }
};
bool valid_val(char* numb) {
    if (numb == nullptr) {
        return false;
    }
    bool flag = true;
    int i = 0;
    while (i < 21) {
        if (numb[i] == '\0') {
            break;
        }
        if (!(numb[i] >= '0' && numb[i] <= '9')) {
            flag = false;
            break;
        }
        i++;
    }
    return flag;
}

bool valid_key(char* key) {
    if (key == nullptr) {
        return false;
    }
    for (int i = 0; i < 257; i++) {
        if (key[i] == '\0') {
            break;
        } else if (!((key[i] >= 'a' && key[i] <= 'z') || (key[i] >= 'A' && key[i] <= 'Z'))) {
            return false;
        }
        if (key[i] >= 'A' && key[i] <= 'Z') {
            key[i] = tolower(key[i]);
        }
    }
    return true;
}





ans* parser(char* cmd, ans* parsed) {
    char* pch = strtok(cmd," \n");
    while (pch != nullptr) {
        if (strcmp(pch, "-") == 0) {
            pch = strtok(nullptr, " \n");
            if (valid_key(pch)) {
                parsed->cmd = '-';
                strcpy(parsed->key, pch);
                break;
            } else {
                parsed->cmd = -1;
                break;
            }
        } else if (strcmp(pch, "+") == 0) {
            pch = strtok(nullptr, " \n");
            if (valid_key(pch)) {
                strcpy(parsed->key, pch);
                pch = strtok(nullptr, " \n");
                if (valid_val(pch)) {
                    parsed->cmd = '+';
                    parsed->val = stoull(pch);
                    break;
                } else {
                    parsed->cmd = -2;
                    break;
                }
            } else {
                parsed->cmd = -1;
                break;
            }
        } else if (valid_key(pch)) {
            parsed->cmd = 'f';
            strcpy(parsed->key, pch);
            break;
        } else if (strcmp(pch, "!") == 0) {
            pch = strtok(nullptr, " \n");
            if (strcmp(pch, "Save") == 0) {
                pch = strtok(nullptr, " \n");
                parsed->cmd = 's';
                strcpy(parsed->key,pch);
            } else if (strcmp(pch, "Load") == 0) {
                pch = strtok(nullptr, " \n");
                parsed->cmd = 'l';
                strcpy(parsed->key,pch);
            } else {
                parsed->cmd = -1;
            }
            break;
        } else {
            parsed->cmd = -9;
            break;
        }
    }
    return parsed;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    TAvl tree;
    ans* parsed =  (ans*)malloc(sizeof(ans));
    parsed->key = (char*)malloc(257);
    char* command = (char*)malloc(300);

    while (std::cin.getline(command, 300)) {
        parsed = parser(command, parsed);
        parsed->key[256] = '\0';
        if (parsed->cmd == '+') {
            tree.Add(parsed->key, parsed->val);
        } else if (parsed->cmd == '-') {
            tree.Delete(parsed->key);
        } else if (parsed->cmd == 'f') {
            tree.Find(parsed->key);
        } else if (parsed->cmd == 's') {
            if (tree.OpenFileSave(parsed->key)) {
                std::cout << "OK\n";
            } else {
                std::cout << "ERROR: cannot save\n";
            }
        } else if (parsed->cmd == 'l') {
            if (tree.OpenFileLoad(parsed->key)) {
                std::cout << "OK\n";
            } else {
                std::cout << "ERROR: cannot load\n";
            }
        }
    }
    free(parsed->key);
    free(parsed);
    free(command);
    return 0;
}