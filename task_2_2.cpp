/*
Дано число N < 10^6 и последовательность пар целых чисел из [-2^31, 2^31] длиной N. 
Построить декартово дерево из N узлов, характеризующихся парами чисел (Xi, Yi). 
Каждая пара чисел (Xi, Yi) определяет ключ Xi и приоритет Yi в декартовом дереве. 
Добавление узла в декартово дерево выполняйте второй версией алгоритма, рассказанного на лекции:
При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом. 
Затем разбейте найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x, а во втором больше или равны x. 
Получившиеся два дерева сделайте дочерними для нового узла (x, y). Новый узел вставьте на место узла P. 
Построить также наивное дерево поиска по ключам Xi. Равные ключи добавляйте в правое поддерево. 
Вычислить количество узлов в самом широком слое декартового дерева и количество узлов в самом широком слое наивного дерева поиска. 
Вывести их разницу. Разница может быть отрицательна.

Ввод:       Вывод:
10          1
5 11
18 8
25 7
50 12
30 30
15 15
20 10
22 5
40 20
45 9
*/

#include <iostream>
#include <queue> 

struct Node {
    ~Node();
 
    int Data;
    Node* Left = nullptr;
    Node* Right = nullptr;
 
    explicit Node(int data) : Data(data) {}
};
 
Node::~Node() {
    delete Left;
    delete Right;
}

class Tree {
    public:
    Tree() : root(nullptr) {}
    ~Tree();
    void Add(int key);
    int getMaxWidth(); 

    private:
    Node* root = nullptr;
};
 
Tree::~Tree() {
    delete root;
}
 
void Tree::Add(int key) {
    if (!root) {
        root = new Node(key);
        return;
    }
    Node* current = root;
    while (true) {
        if (key < current->Data) {
            if (current->Left != nullptr) {
                current = current->Left;
            } else {
                current->Left = new Node(key, current);
                break;
            }
        } else {
            if (current->Right != nullptr) {
                current = current->Right;
            } else {
                current->Right = new Node(key, current);
                break;
            }
        }
    }
}

int Tree::getMaxWidth() {
    if (!root) {
        return 0;
    } 
    std::queue<Node*> width;
    int max_width = 0;
    int curr_width = 0;
    Node* current = root;
    width.push(current);
    while (!width.empty()) {
        curr_width = width.size();
        if (curr_width > max_width) {
            max_width = curr_width;
        }
        for (int i = 0; i < curr_width; i++) {
            current = width.front();
            if (current->Left) {
                width.push(current->Left);
            }
            if (current->Right) {
                width.push(current->Right);
            }
            width.pop();
        }
    }
    return max_width;
}

struct TreapNode {
    ~TreapNode();
    int Key;
    int Priority;
    TreapNode* Left = nullptr;
    TreapNode* Right = nullptr;
 
    TreapNode(int key, int priority) : Key(key), Priority(priority) {}
};

TreapNode::~TreapNode() {
    delete Left;
    delete Right;
}
 
class Treap {
    public:
    Treap() : root(nullptr) {}
    ~Treap();
    void Add(int key, int priority);
    int getMaxWidth();
 
    private:
    TreapNode* root = nullptr;
    std::pair<TreapNode*, TreapNode*> Split(int key, TreapNode* node);
    TreapNode* Merge(TreapNode* t1, TreapNode* t2);
};

Treap::~Treap() {
    delete root;
}

std::pair<TreapNode*, TreapNode*> Treap::Split(int key, TreapNode* node) {
    if (!node) {
        return {nullptr, nullptr};
    }
    if (node->Key < key) {
        auto [t1, t2] = Split(key, node->Right);
        node->Right = t1;
        return {node, t2};
    } else {
        auto [t1, t2] = Split(key, node->Left);
        node->Left = t2;
        return {t1, node};
    }
}

 TreapNode* Treap::Merge(TreapNode* left, TreapNode* right) {
    if (left == nullptr || right == nullptr) {
        return left == nullptr ? right : left;
    }
    if (left->Priority > right->Priority) {
        left->Right = Merge(left->Right, right);
        return left;
    } else {
        right->Left = Merge(left, right->Left);
        return right;
    }
}
 
void Treap::Add(int key, int priority) {
    auto [t1, t2] = Split(key, root);
    TreapNode* new_node = new TreapNode(key, priority);
    t1 = Merge(t1, new_node);
    root = Merge(t1, t2);
}

int Treap::getMaxWidth() {
    if (!root) {
        return 0;
    } 
    std::queue<TreapNode*> width;
    int max_width = 0;
    int curr_width = 0;
    TreapNode* current = root;
    width.push(current);
    while (!width.empty()) {
        curr_width = width.size();
        if (curr_width > max_width) {
            max_width = curr_width;
        }
        for (int i = 0; i < curr_width; i++) {
            current = width.front();
            if (current->Left) {
                width.push(current->Left);
            }
            if (current->Right) {
                width.push(current->Right);
            }
            width.pop();
        }
    }
    return max_width;
}

int main() {
    int n = 0;
    std::cin >> n;
    int key = 0;
    int priority = 0;
    Tree tree;
    Treap cartesian_tree;
    for (int i = 0; i < n; i++) {
        std::cin >> key;
        std::cin >> priority;
        tree.Add(key);
        cartesian_tree.Add(key, priority);
    }
    std::cout << cartesian_tree.getMaxWidth() - tree.getMaxWidth() << std::endl;
    return 0;
}
