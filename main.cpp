#include <string.h>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <iostream>
typedef unsigned long long ull;
struct TAns {
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
    ull Height(const TNode* node) {
        return node != nullptr ? node->height : 0;
    }

    int Balance(const TNode* node) {
        return Height(node->left) - Height(node->right);
    }

    void FixHeight(TNode* node) {
        node->height = std::max(Height(node->left), Height(node->right)) + 1;
    }

    TNode* RotateLeft(TNode* p) {
        TNode* q = p->right;
        p->right = q->left;
        q->left = p;
        FixHeight(p);
        FixHeight(q);
        return q;
    }

    TNode* RotateRight(TNode* p) {
        TNode* q = p->left;
        p->left = q->right;
        q->right = p;
        FixHeight(p);
        FixHeight(q);
        return q;
    }

    TNode* BigRotateLeft(TNode* p) {
        p->right = RotateRight(p->right);
        return RotateLeft(p);
    }

    TNode* BigRotateRight(TNode* p) {
        p->left = RotateLeft(p->left);
        return RotateRight(p);
    }

    TNode* Rebalance(TNode* node) {
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

    TNode* Insert(TNode* node, char* k, ull v) {
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
        int res = strcmp(k, node->key);
        if (res < 0) {
            node->left = Insert(node->left, k, v);
        } else if (res > 0) {
            node->right = Insert(node->right, k, v);
        } else {
            std::cout << "Exist\n";
        }
        return Rebalance(node);
    }

    TNode* RemoveMin(TNode* node, TNode* tmp) {
        if (tmp->left != nullptr) {
            tmp->left = RemoveMin(node, tmp->left);
        } else {
            TNode* rightSubNode = tmp->right;
            strcpy(node->key, tmp->key);
            node->value = tmp->value;
            delete tmp;
            tmp = rightSubNode;
        }
        return Rebalance(tmp);
    }

    TNode* Remove(TNode* node, char* k) {
        if (node == nullptr) {
            std::cout << "NoSuchWord\n";
            return nullptr;
        }
        int res = strcmp(k, node->key);
        if (res < 0) {
            node->left = Remove(node->left, k);
        } else if (res > 0) {
            node->right = Remove(node->right, k);
        } else {
            TNode* leftSubNode = node->left;
            TNode* rightSubNode = node->right;
            if (leftSubNode == nullptr && rightSubNode == nullptr) {
                std::cout << "OK\n";
                delete node;
                return nullptr;
            }
            if (rightSubNode == nullptr) {
                std::cout << "OK";
                std::cout << "\n";
                delete node;
                return leftSubNode;
            }
            if (leftSubNode == nullptr) {
                std::cout << "OK\n";
                delete node;
                return rightSubNode;
            }
            node->right = RemoveMin(node, rightSubNode);
            std::cout << "OK\n";
        }
        return Rebalance(node);
    }

    TNode* Search(TNode* node, char* k) {
        if (node == nullptr) {
            std::cout << "NoSuchWord\n";
            return nullptr;
        }
        TNode* tmp = node;
        while (tmp != nullptr) {
            int res = strcmp(k, tmp->key);
            if (res < 0) {
                tmp = tmp->left;
            } else if (res > 0) {
                tmp = tmp->right;
            } else {
                std::cout << "OK: " << tmp->value << "\n";
                return tmp;
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
    TNode* Find(char* k) {
        return Search(root, k);
    }
    void TreeDelete(TNode* node) {
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
        bool hasLeft = false;
        bool hasRight = false;
        if (node->left != nullptr) {
            hasLeft = node->left != nullptr;
        }
        if (node->right != nullptr) {
            hasRight = node->right != nullptr;
        }
        os.write(reinterpret_cast<char*>(&keySize), sizeof(keySize));
        os.write(node->key, keySize);
        os.write((char*)&node->value, sizeof(node->value));
        os.write(reinterpret_cast<char*>(&hasLeft), sizeof(hasLeft));
        os.write(reinterpret_cast<char*>(&hasRight), sizeof(hasRight));
        if (hasLeft) {
            Save(os, node->left);
        }
        if (hasRight) {
            Save(os, node->right);
        }
    }

    TNode* Load(std::istream &is, const TNode* node) {
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
        bool hasLeft = false;
        bool hasRight = false;
        is.read(reinterpret_cast<char*>(&value), sizeof(value));
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
        } else {
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
        } else {
            return false;
        }
        is.close();
        return true;
    }
};
bool CheckVal(char* numb) {
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

bool CheckKey(char* key) {
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
TAns* Parser(char* cmd, TAns* parsed) {
    char* pch = strtok(cmd," \n");
    while (pch != nullptr) {
        if (strcmp(pch, "-") == 0) {
            pch = strtok(nullptr, " \n");
            if (CheckKey(pch)) {
                parsed->cmd = '-';
                strcpy(parsed->key, pch);
                break;
            } else {
                parsed->cmd = -1;
                break;
            }
        } else if (strcmp(pch, "+") == 0) {
            pch = strtok(nullptr, " \n");
            if (CheckKey(pch)) {
                strcpy(parsed->key, pch);
                pch = strtok(nullptr, " \n");
                if (CheckVal(pch)) {
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
        } else if (CheckKey(pch)) {
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
    TAns* parsed =  (TAns*)malloc(sizeof(TAns));
    parsed->key = (char*)malloc(257);
    char* command = (char*)malloc(300);

    while (std::cin.getline(command, 300)) {
        parsed = Parser(command, parsed);
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