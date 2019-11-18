#ifndef DA_LAB2_AVL_H
#define DA_LAB2_AVL_H
struct node {
    char key[256];
    unsigned long long value;
    unsigned char height;
    node* left;
    node* right;
    node(char k[256]) { string_copy(key,k); left = right = nullptr; height = 1; }
};

#endif //DA_LAB2_AVL_H
