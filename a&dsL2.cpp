#include <iostream>
#include <queue>
#include <random>
const char choice_str[146] = "0 - exit, 1 - find, 2 - insert, 3 - delete, 4 - max, 5 - min, 6 - plr, 7 - lpr, 8 - rpl, 9 - levels, 10 - height random, 11 - height increasing: ";
enum Color { RED, BLACK };
struct Node {
    int key;
    Node* left;
    Node* right;
    Node* parent;
    int height;
    Color color;
    Node(int v) { key = v, left = nullptr, right = nullptr, parent = nullptr, height = 1, color = RED; }
};
class BSTtree {
private:
    Node* root;
public:
    BSTtree() : root(nullptr) {}
    Node* find(Node* start, int v)
    {
        if (start != nullptr)
        {
            if (start->key == v) return start;
            if (start->key > v) return find(start->left, v);
            else return find(start->right, v);
        }
        return nullptr;
    }
    int max(Node* start)
    {
        Node* max = start;
        while (max->right != nullptr) max = max->right;
        return max->key;
    }
    int min(Node* start)
    {
        Node* min = start;
        while (min->left != nullptr) min = min->left;
        return min->key;
    }
    void plr(Node* start, char c = 'n')
    {
        if (start == nullptr) return;
        if (c == 'r')
        {
            if (start->color == RED) std::cout << "red" << " ";
            else std::cout << "black" << " ";
        }
        std::cout << start->key << ", ";
        plr(start->left, c);
        plr(start->right, c);
    }
    void lpr(Node* start, char c = 'n')
    {
        if (start == nullptr) return;
        lpr(start->left, c);
        if (c == 'r')
        {
            if (start->color == RED) std::cout << "red" << " ";
            else std::cout << "black" << " ";
        }
        std::cout << start->key << ", ";
        lpr(start->right, c);
    }
    void rpl(Node* start, char c = 'n')
    {
        if (start == nullptr) return;
        rpl(start->right, c);
        if (c == 'r')
        {
            if (start->color == RED) std::cout << "red" << " ";
            else std::cout << "black" << " ";
        }
        std::cout << start->key << ", ";
        rpl(start->left, c);
    }
    void level_print(Node* start, char c = 'n')
    {
        if (start == nullptr) return;
        std::queue<Node*> queue;
        Node* nod;
        queue.push(start);
        while (!queue.empty())
        {
            nod = queue.front();
            queue.pop();
            if (nod->left)
            {
                queue.push(nod->left);
            }
            if (nod->right)
            {
                queue.push(nod->right);
            }
            if (c == 'r')
            {
                if (nod->color == 0) std::cout << "red" << " ";
                else std::cout << "black" << " ";
            }
            std::cout << nod->key << ", ";
        }
        std::cout << "\n";
    }
    virtual Node* insert(Node* cur, int val)
    {
        if (cur == nullptr) {
            cur = new Node(val); return cur;}
        if (cur->key == val) return root;
        else if (cur->key < val)
        {
            if (cur->right == nullptr) { cur->right = new Node(val); return root; }
            else return insert(cur->right, val);
        }
        else
        {
            if (cur->left == nullptr) { cur->left = new Node(val); return root; }
            else return insert(cur->left, val);
        }
    }
    virtual Node* deleteNode(Node* start, int val)
    {
        if (start == nullptr) return start;
        if (start->key > val)
            deleteNode(start->left, val);
        else if (start->key < val)
            deleteNode(start->right, val);
        else
        {
            if ((start->left == nullptr) || (start->right == nullptr)) {
                Node* temp = start->left ? start->left : start->right;
                if (temp == nullptr) {
                    temp = start;
                    start = nullptr;
                }
                else
                    *start = *temp;
                delete temp;
            }
            else {
                Node* temp = find(start, min(start->right));
                start->key = temp->key;
                start->right = deleteNode(start->right, temp->key);
            }
        }
        if (root == nullptr)
            return root;
    }
    int height(Node* start)
    {
        Node* cur = start;
        if (!cur) return 0;
        return 1 + std::max(height(cur->left), height(cur->right));
    }
    virtual int heightroot()
    {
        return height(root);
    }
    virtual void findroot(int v)
    {
        if (find(root, v)) std::cout << "Key found\n";
        else std::cout << "Key not found";
    }
    virtual int maxroot()
    {
        return max(root);
    }
    virtual int minroot()
    {
        return min(root);
    }
    virtual void insertroot(int key) {
        root = insert(root, key);
    }

    virtual void deleteNoderoot(int key) {
        root = deleteNode(root, key);
    }
    virtual void plrroot()
    {
        plr(root);
    }
    virtual void lprroot()
    {
        lpr(root);
    }
    virtual void rplroot()
    {
        rpl(root);
    }
    virtual void level_printroot()
    {
        level_print(root);
    }
};
class AVLtree : public BSTtree {
private:
    Node* root;
    int height(Node* N) {
        if (N == nullptr)
            return 0;
        return N->height;
    }
    int getBalance(Node* N) {
        if (N == nullptr)
            return 0;
        return height(N->left) - height(N->right);
    }
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* xr = x->right;
        x->right = y;
        y->left = xr;
        y->height = 1 + std::max(height(y->left), height(y->right));
        x->height = 1 + std::max(height(x->left), height(x->right));
        return x;
    }
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* yl = y->left;
        y->left = x;
        x->right = yl;
        x->height = 1 + std::max(height(x->left), height(x->right));
        y->height = 1 + std::max(height(y->left), height(y->right));
        return y;
    }
    Node* insert(Node* node, int key) override {
        if (node == nullptr)
            return new Node(key);
        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node;
        node->height = 1 + std::max(height(node->left), height(node->right));
        int balance = getBalance(node);
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }
    Node* deleteNode(Node* root, int key) override {
        if (root == nullptr)
            return root;

        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {
            if ((root->left == nullptr) || (root->right == nullptr)) {
                Node* temp = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                }
                else
                    *root = *temp;
                delete temp;
            }
            else {
                Node* temp = find(root->right, min(root->right));
                root->key = temp->key;
                root->right = deleteNode(root->right, temp->key);
            }
        }
        if (root == nullptr)
            return root;
        root->height = 1 + std::max(height(root->left), height(root->right));
        int balance = getBalance(root);
        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;
    }

public:
    AVLtree() : root(nullptr) {}

    int heightroot() override
    {
        return height(root);
    }
    void findroot(int v) override
    {
        if (find(root, v)) std::cout << "Key found\n";
        else std::cout << "Key not found";
    }
    int maxroot() override
    {
        return max(root);
    }
    int minroot() override
    {
        return min(root);
    }
    void insertroot(int key) override {
        root = insert(root, key);
    }
    void deleteNoderoot(int key) override {
        root = deleteNode(root, key);
    }
    void plrroot() override
    {
        plr(root);
    }
    void lprroot() override
    {
        lpr(root);
    }
    void rplroot() override
    {
        rpl(root);
    }
    void level_printroot() override
    {
        level_print(root);
    }
};

class RBtree : public BSTtree {
private:
    Node* root;
    void rotateLeft(Node*& node)
    {
        Node* child = node->right;
        node->right = child->left;
        if (node->right != nullptr)
            node->right->parent = node;
        child->parent = node->parent;
        if (node->parent == nullptr)
            root = child;
        else if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->left = node;
        node->parent = child;
    }
    void rotateRight(Node*& node)
    {
        Node* child = node->left;
        node->left = child->right;
        if (node->left != nullptr)
            node->left->parent = node;
        child->parent = node->parent;
        if (node->parent == nullptr)
            root = child;
        else if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->right = node;
        node->parent = child;
    }
    void fixInsert(Node*& node)
    {
        Node* parent = nullptr;
        Node* grandparent = nullptr;
        while (node != root && node->color == RED
            && node->parent->color == RED) {
            parent = node->parent;
            grandparent = parent->parent;
            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;
                if (uncle != nullptr
                    && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                }
                else {
                    if (node == parent->right) {
                        rotateLeft(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateRight(grandparent);
                    std::swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
            else {
                Node* uncle = grandparent->left;
                if (uncle != nullptr
                    && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                }
                else {
                    if (node == parent->left) {
                        rotateRight(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateLeft(grandparent);
                    std::swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }
        root->color = BLACK;
    }
    void fixDelete(Node*& node)
    {
        while (node != root && node->color == BLACK) {
            if (node == node->parent->left) {
                Node* sibling = node->parent->right;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateLeft(node->parent);
                    sibling = node->parent->right;
                }
                if ((sibling->left == nullptr
                    || sibling->left->color == BLACK)
                    && (sibling->right == nullptr
                        || sibling->right->color
                        == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                }
                else {
                    if (sibling->right == nullptr
                        || sibling->right->color == BLACK) {
                        if (sibling->left != nullptr)
                            sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = node->parent->right;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->right != nullptr)
                        sibling->right->color = BLACK;
                    rotateLeft(node->parent);
                    node = root;
                }
            }
            else {
                Node* sibling = node->parent->left;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateRight(node->parent);
                    sibling = node->parent->left;
                }
                if ((sibling->left == nullptr
                    || sibling->left->color == BLACK)
                    && (sibling->right == nullptr
                        || sibling->right->color
                        == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                }
                else {
                    if (sibling->left == nullptr
                        || sibling->left->color == BLACK) {
                        if (sibling->right != nullptr)
                            sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = node->parent->left;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->left != nullptr)
                        sibling->left->color = BLACK;
                    rotateRight(node->parent);
                    node = root;
                }
            }
        }
        node->color = BLACK;
    }
    void transplant(Node*& root, Node*& u, Node*& v)
    {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v != nullptr)
            v->parent = u->parent;
    }
public:
    RBtree() : root(nullptr) {}
    
    int heightroot() override
    {
        return height(root);
    }
    void findroot(int v) override
    {
        if (find(root, v)) std::cout << "Key found\n";
        else std::cout << "Key not found";
    }
    int maxroot() override
    {
        return max(root);
    }
    int minroot() override
    {
        return min(root);
    }
    void insertroot(int key) override
    {
        Node* node = new Node(key);
        Node* parent = nullptr;
        Node* current = root;
        while (current != nullptr) {
            parent = current;
            if (node->key < current->key)
                current = current->left;
            else
                current = current->right;
        }
        node->parent = parent;
        if (parent == nullptr)
            root = node;
        else if (node->key < parent->key)
            parent->left = node;
        else
            parent->right = node;
        fixInsert(node);
    }
    void deleteNoderoot(int key) override
    {
        Node* node = root;
        Node* z = nullptr;
        Node* x = nullptr;
        Node* y = nullptr;
        while (node != nullptr) {
            if (node->key == key) {
                z = node;
            }

            if (node->key <= key) {
                node = node->right;
            }
            else {
                node = node->left;
            }
        }
        if (z == nullptr) {
            std::cout << "Key not found in the tree" << std::endl;
            return;
        }
        y = z;
        Color yOriginalColor = y->color;
        if (z->left == nullptr) {
            x = z->right;
            transplant(root, z, z->right);
        }
        else if (z->right == nullptr) {
            x = z->left;
            transplant(root, z, z->left);
        }
        else {
            y = find(z->right, min(z->right));
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                if (x != nullptr)
                    x->parent = y;
            }
            else {
                transplant(root, y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(root, z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (yOriginalColor == BLACK) {
            fixDelete(x);
        }
    }
    void plrroot() override
    {
        plr(root, 'r');
    }
    void lprroot() override
    {
        lpr(root, 'r');
    }
    void rplroot() override
    {
        rpl(root, 'r');
    }
    void level_printroot() override
    {
        level_print(root, 'r');
    }
};
void checkTreeHeightInRandomBST(BSTtree* tree)
{
    int k;
    std::random_device dev;
    std::mt19937 range(dev());
    std::uniform_int_distribution<std::mt19937::result_type> randomkey(1, 1000000);
    std::cout << "Random keys:\n";
    for (int count = 1; count <= 20000; count++)
    {
        k = randomkey(range);
        (*tree).insertroot(k);
        if (count % 2000 == 0)
        {
            std::cout << count << " " << (*tree).heightroot() << "\n";
        }
    }
}
void checkTreeHeightInIncreasingBST(BSTtree* tree)
{
    int k = 1;
    std::cout << "Increasing keys:\n";
    for (int count = 1; count <= 20000; count++, k++)
    {
        (*tree).insertroot(k);
        if (count % 2000 == 0)
        {
            std::cout << count << " " << (*tree).heightroot() << "\n";
        }
    }
}
void checkTreeHeightInRandomAVL(AVLtree* tree)
{
    int k;
    std::random_device dev;
    std::mt19937 range(dev());
    std::uniform_int_distribution<std::mt19937::result_type> randomkey(1, 1000000);
    std::cout << "Random keys:\n";
    for (int count = 1; count <= 20000; count++)
    {
        k = randomkey(range);
        (*tree).insertroot(k);
        if (count % 2000 == 0)
        {
            std::cout << count << " " << (*tree).heightroot() << "\n";
        }
    }
}
void checkTreeHeightInIncreasingAVL(AVLtree* tree)
{
    int k = 1;
    std::cout << "Increasing keys:\n";
    for (int count = 1; count <= 20000; count++, k++)
    {
        (*tree).insertroot(k);
        if (count % 2000 == 0)
        {
            std::cout << count << " " << (*tree).heightroot() << "\n";
        }
    }
}
void checkTreeHeightInRandomRB(RBtree* tree)
{
    int k;
    std::random_device dev;
    std::mt19937 range(dev());
    std::uniform_int_distribution<std::mt19937::result_type> randomkey(1, 1000000);
    std::cout << "Random keys:\n";
    for (int count = 1; count <= 20000; count++)
    {
        k = randomkey(range);
        (*tree).insertroot(k);
        if (count % 2000 == 0)
        {
            std::cout << count << " " << (*tree).heightroot() << "\n";
        }
    }
}
void checkTreeHeightInIncreasingRB(RBtree* tree)
{
    int k = 1;
    std::cout << "Increasing keys:\n";
    for (int count = 1; count <= 20000; count++, k++)
    {
        (*tree).insertroot(k);
        if (count % 2000 == 0)
        {
            std::cout << count << " " << (*tree).heightroot() << "\n";
        }
    }
}

int main()
{
    BSTtree bst;
    AVLtree avl;
    RBtree rb;
    unsigned choice1 = 1, choice2 = 1;
    int val;
    std::cout << "0 - exit, 1 - BST, 2 - AVL, 3 - RB: ";
    std::cin >> choice1;
    while (choice1)
    {
        std::cout << choice_str << "\n";
        while (choice2)
        {
            std::cout << "Next action: ";
            std::cin >> choice2;
            switch (choice2)
            {
            case 1:
                std::cout << "insert int value: ";
                std::cin >> val;
                switch (choice1)
                {
                case 1:
                    bst.findroot(val);
                    break;
                case 2:
                    avl.findroot(val);
                    break;
                case 3:
                    rb.findroot(val);
                    break;
                }
                break;
            case 2:
                std::cout << "insert int value: ";
                std::cin >> val;
                switch (choice1)
                {
                case 1:
                    bst.insertroot(val);
                    break;
                case 2:
                    avl.insertroot(val);
                    break;
                case 3:
                    rb.insertroot(val);
                    break;
                }
                break;
            case 3:
                std::cout << "insert int value: ";
                std::cin >> val;
                switch (choice1)
                {
                case 1:
                    bst.deleteNoderoot(val);
                    break;
                case 2:
                    avl.deleteNoderoot(val);
                    break;
                case 3:
                    rb.deleteNoderoot(val);
                    break;
                }
                break;
            case 4:
                switch (choice1)
                {
                case 1:
                    std::cout << "max BST value: " << bst.maxroot() << "\n";
                    break;
                case 2:
                    std::cout << "max AVL value: " << avl.maxroot() << "\n";
                    break;
                case 3:
                    std::cout << "max RB value: " << rb.maxroot() << "\n";
                    break;
                }
                break;
            case 5:
                switch (choice1)
                {
                case 1:
                    std::cout << "min BST value: " << bst.minroot() << "\n";
                    break;
                case 2:
                    std::cout << "min AVL value: " << avl.minroot() << "\n";
                    break;
                case 3:
                    std::cout << "min RB value: " << rb.minroot() << "\n";
                    break;
                }
                break;
            case 6:
                switch (choice1)
                {
                case 1:
                    bst.plrroot();
                    std::cout << "\n";
                    break;
                case 2:
                    avl.plrroot();
                    std::cout << "\n";
                    break;
                case 3:
                    rb.plrroot();
                    std::cout << "\n";
                    break;
                }
                break;
            case 7:
                switch (choice1)
                {
                case 1:
                    bst.lprroot();
                    std::cout << "\n";
                    break;
                case 2:
                    avl.lprroot();
                    std::cout << "\n";
                    break;
                case 3:
                    rb.lprroot();
                    std::cout << "\n";
                    break;
                }
                break;
            case 8:
                switch (choice1)
                {
                case 1:
                    bst.rplroot();
                    std::cout << "\n";
                    break;
                case 2:
                    avl.rplroot();
                    std::cout << "\n";
                    break;
                case 3:
                    rb.rplroot();
                    std::cout << "\n";
                    break;
                }
                break;
            case 9:
                switch (choice1)
                {
                case 1:
                    bst.level_printroot();
                    std::cout << "\n";
                    break;
                case 2:
                    avl.level_printroot();
                    std::cout << "\n";
                    break;
                case 3:
                    rb.level_printroot();
                    std::cout << "\n";
                    break;
                }
                break;
            case 10:
                switch (choice1)
                {
                case 1:
                    checkTreeHeightInRandomBST(&bst);
                    break;
                case 2:
                    checkTreeHeightInRandomAVL(&avl);
                    break;
                case 3:
                    checkTreeHeightInRandomRB(&rb);
                    break;
                }
                break;
            case 11:
                switch (choice1)
                {
                case 1:
                    checkTreeHeightInIncreasingBST(&bst);
                    break;
                case 2:
                    checkTreeHeightInIncreasingAVL(&avl);
                    break;
                case 3:
                    checkTreeHeightInIncreasingRB(&rb);
                    break;
                }
                break;
            default:
                break;
            }
        }
        std::cout << "0 - exit, 1 - BST, 2 - AVL, 3 - RB: ";
        std::cin >> choice1;
        choice2 = 1;
    }
}