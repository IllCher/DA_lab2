#include<iostream>
int flag=0;
// An AVL tree node
struct node {
    char* key;
    unsigned long long value;
    struct node *left;
    struct node *right;
    int height;
}*root;
//Class Declaration
class avlTree {
public:
    int max(int a, int b);
    int height(struct node *N);
    node* newNode(char* key);
    void display(node *ptr, int level);
    node* rightRotate(struct node *y);
    node* leftRotate(struct node *x);
    int getBalance(struct node *N);
    struct node* insert(struct node* node, char* key);
    struct node * minValueNode(struct node* node);
    struct node* deleteNode(struct node* root, char* key);
    void inOrder(struct node *root);
    void search(char* key);
    void reflect(node * p);
    void defoliate();
    //Default constructor
    avlTree() {
        root = nullptr;
    }
};

int char_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return (c-'0');
    } else if (c >= 'a' && c <= 'z') {
        c -= 'W';
    } else if (c >= 'A' && c <= 'Z') {
        c = tolower(c);
        c -= 'W';
    } else {
        return 0;
    }
}

bool a_lower_or_eq_b(char* a, char* b) {
    int v1 = 0;
    int v2 = 0;
    int i = 0;
    while (a[i] != '\0') {
        i++;
    }
    int k = 1;
    while (i > 0) {
        i--;
        v1 += char_to_int(a[i])*k;
        k*=10;
    }
    i = 0;
    while (a[i] != '\0') {
        i++;
    }
    k = 1;
    while (i > 0) {
        i--;
        v2 += char_to_int(b[i])*k;
        k*=10;
    }
    if (v1 < v2) {
        return  true;
    }
    return false;
}

class A {
    const char* str;
public:
    /* ... */
    bool operator<(const A & rhs);
};

bool A::operator<(const A & rhs) {
    return
}




void avlTree::search(char* key) {
    node *temp = root,*parent = root;
    if(temp==nullptr)
        std::cout<<"\nThe AVL Tree is empty\n"<<std::endl;
    else
    {
        while(temp!=nullptr && temp->key != key) {
            parent=temp;
            if(temp->key<key) {
                temp=temp->right;
            }
            else {
                temp=temp->left;
            }
        }
    }

    if(temp==nullptr)
        std::cout<<"This element is NOT present in the tree!";
    else
    {std::cout<<"\nThis element is present in the tree! ";
        std::cout<<"\nIt's height is: "<<temp->height;
    }

}

void avlTree::reflect(node * p)
//Swaps the left and right nodes to create a reflection of the AVL-tree
{
    if(!p) return;
    reflect(p->left);
    reflect(p->right);
    node * temp=p->left;
    p->left=p->right;
    p->right=temp;

}
void destroy(node *p)
//Function to destroy the AVL-Tree
{
    if(!p) return;
    destroy(p->left);
    destroy(p->right);
    delete p;
}
void d(node *p)
{    node* lc=p->left;
    if(lc&&(lc->left||lc->right)) d(lc);
    else
    { delete lc;
        p->left=nullptr;
    }
    node* rc=p->right;
    if(rc&&(rc->left||rc->right)) d(rc);
    else
    { delete rc;
        p->right=nullptr;
    }
}
void avlTree::defoliate()
//Function to delete all leaves
{
    if(!root) return;
    if(root->left || root->right) d(root);
    else
        destroy(root);
}


// A function to get maximum of two integers
int avlTree::max(int a, int b)
{
    return (a > b)? a : b;
}

// A function to get height of the tree
int avlTree::height(struct node *N)
{
    if (N == nullptr)
        return 0;
    return N->height;
}
//A function to display AVL Tree
void avlTree::display(node *ptr, int level)
{
    int i;
    if (ptr!=nullptr)
    {
        display(ptr->right, level + 1);
        printf("\n");
        if (ptr == root)
            std::cout<<"Root -> ";
        for (i = 0; i < level && ptr != root; i++)
            std::cout<<"        ";
        std::cout<<ptr->key;
        display(ptr->left, level + 1);
    }
}


/* Function that allocates a new node with the given key and
    nullptr left and right pointers. */
struct node* avlTree::newNode(int key)
{
    struct node* node = (struct node*)
            malloc(sizeof(struct node));
    node->key   = key;
    node->left   = nullptr;
    node->right  = nullptr;
    node->height = 1;  // new node is initially added at leaf
    return(node);
}

// A function to right rotate subtree rooted with y
struct node* avlTree::rightRotate(struct node *y)
{
    struct node *x = y->left;
    struct node *T2 = x->right;
    // Perform rotation
    x->right = y;
    y->left = T2;
    // Update heights
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
    // Return new root
    return x;
}

// A function to left rotate subtree rooted with x
struct node* avlTree::leftRotate(struct node *x)
{
    struct node *y = x->right;
    struct node *T2 = y->left;
    // Perform rotation
    y->left = x;
    x->right = T2;
    //  Update heights
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
    // Return new root
    return y;
}

//A function to get the balance factor of node N
int avlTree::getBalance(struct node *N)
{
    if (N == nullptr)
        return 0;
    return height(N->left) - height(N->right);
}
//A function to delete a selected node
struct node* avlTree::insert(struct node* node, int key)
{
    //First we perform a normal BST rotation
    if (node == nullptr)
        return(newNode(key));
    if (key < node->key)
        node->left  = insert(node->left, key);
    else
        node->right = insert(node->right, key);
    //Then we update the height of this ancestor node
    node->height = max(height(node->left), height(node->right)) + 1;
    //Get the balance factor of this ancestor node to check whether this node became unbalanced
    int balance = getBalance(node);
    //If this node becomes unbalanced, then 4 cases arise
    // 1.Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
    // 2.Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
    // 3.Left Right Case
    if (balance > 1 && key > node->left->key)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
    // 4.Right Left Case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    /* Return the (unchanged) node pointer */
    return node;
}
//A function return the node with minimum key value found in the non-empty binary search tree.
struct node * avlTree::minValueNode(struct node* node)
{
    struct node* current = node;
    //Loop to find the leftmost leaf */
    while (current->left != nullptr)
        current = current->left;
    return current;
}

struct node* avlTree::deleteNode(struct node* root, int key)
{
    // STEP 1: Perform standard BST delete
    if (root == nullptr)
        return root;
    // If the key to be deleted is smaller than the root's key, then it lies in left subtree
    if ( key < root->key )
        root->left = deleteNode(root->left, key);
        // If the key to be deleted is greater than the root's key, then it lies in right subtree
    else if( key > root->key )
        root->right = deleteNode(root->right, key);
        // If the key is the same as root's key, then this is the node to be deleted
    else
    {   // node with only one child or no child
        if( (root->left == nullptr) || (root->right == nullptr) )
        {   struct node *temp = root->left ? root->left : root->right;
            flag=1;
            // Node with no child
            if(temp == nullptr)
            {
                temp = root;
                root = nullptr;
                flag=1;
            }
            else // Node with one child
            {*root = *temp;flag=1;}//Copy the contents of the non-empty child
            free(temp);
        }
        else
        {   // node with two children: Get the inorder successor (smallest in the right subtree)
            struct node* temp = minValueNode(root->right);
            // Copy the inorder successor's data to this node
            root->key = temp->key;
            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
    }
    //If the tree has only one node then return
    if (root == nullptr)
        return root;
    //STEP 2: Update the height of the current node
    root->height = max(height(root->left), height(root->right)) + 1;
    //STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to check whether this node became unbalanced)
    int balance = getBalance(root);
    // If this node becomes unbalanced, then there are 4 cases
    // 1.Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    // 2.Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }
    // 3.Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    // 4.Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}
//A function to print inorder traversal of the tree.
void avlTree::inOrder(struct node *root)
{
    if(root != nullptr)
    {
        inOrder(root->left);
        printf("%d ", root->key);
        inOrder(root->right);
    }
}
//Driver function that forms the user interface
int main()
{
    int choice, item;
    avlTree avl;
    while (1)
    {
        std::cout<<"\n\n***************************************"<<std::endl;
        std::cout<<"\tAVL Tree Implementation"<<std::endl;
        std::cout<<"***************************************"<<std::endl;
        std::cout<<"1.Insert Element into the tree"<<std::endl;
        std::cout<<"2.Display Balanced AVL Tree"<<std::endl;
        std::cout<<"3.Delete Element from tree"<<std::endl;
        std::cout<<"4.InOrder traversal"<<std::endl;
        std::cout<<"5.PreOrder traversal"<<std::endl;
        std::cout<<"6.PostOrder traversal"<<std::endl;
        std::cout<<"7.Search for an Element"<<std::endl;
        std::cout<<"8.Reflect AVL tree"<<std::endl;
        std::cout<<"9.Defoliate AVL tree"<<std::endl;
        std::cout<<"0.Exit"<<std::endl;
        std::cout<<"\nEnter your Choice: ";
        std::cin>>choice;
        std::cout<<"\n";
        switch(choice)
        {
            case 1:
                std::cout<<"Enter value to be inserted: ";
                std::cin>>item;
                root = avl.insert(root, item);
                break;
            case 2:
                if (root == nullptr)
                {
                    std::cout<<"Tree is Empty"<<std::endl;
                    continue;
                }
                std::cout<<"Balanced AVL Tree:"<<std::endl;
                avl.display(root, 1);
                break;
            case 3: std::cout<<"\nEnter value to be deleted: ";
                std::cin>>item;
                root = avl.deleteNode(root, item);
                if(flag==0)
                    std::cout<<"\nElement not found in this tree!";
                else
                    std::cout<<"\nElement deleted successfully!";
                break;
            case 4:
                std::cout<<"Inorder Traversal:"<<std::endl;
                avl.inOrder(root);
                std::cout<<std::endl;
                break;
            case 5:
                std::cout<<"Preorder Traversal:"<<std::endl;
                avl.preOrder(root);
                std::cout<<std::endl;
                break;
            case 6:
                std::cout<<"Postorder Traversal:"<<std::endl;
                avl.postOrder(root);
                std::cout<<std::endl;
                break;
            case 7:std::cout<<"\nEnter element to search: ";
                std::cin>>item;
                avl.search(item);
                break;
            case 8:std::cout<<std::endl;
                avl.reflect(root);
                std::cout<<"\nThe left nodes have become the right nodes and vice versa, i.e. the tree has been successfully reflected.\n";
                break;
            case 9:avl.defoliate();
                std::cout<<"\nTree defoliated. (All leaves deleted)";
                break;
            case 0:exit(1);
                break;
            default:
                std::cout<<"\nWrong Choice!"<<std::endl;
        }
    }
}