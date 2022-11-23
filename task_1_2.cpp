/*
Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31] длиной N. 
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; 
иначе в левое поддерево root. Выведите элементы в порядке pre-order (сверху вниз).
Рекурсия запрещена.
Ввод:           Вывод:
10              2 1 5 4 3 6 7 8 9 10 
2
5
6
4
7
8
9
3
1
10
*/

#include <iostream> 
#include <stack> 

struct Node {
    ~Node();
 
    int Data;
    Node* Left = nullptr;
    Node* Right = nullptr;
    Node* Parent = nullptr;
 
    explicit Node(int data, Node* parent = nullptr) : Data(data), Parent(parent) {}
};
 
Node::~Node() {
    delete Left;
    delete Right;
}

class Tree {
    public:
    ~Tree();
    void Add(int key);
    void pre_order_printer() const;

    private:
    Node* root = nullptr;
 
    void pre_order_printer(Node* node) const;
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
        if (current->Data > key) {
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

void Tree::pre_order_printer() const{
    if (root) {
        pre_order_printer(root);
    }
}

void Tree::pre_order_printer(Node* root) const {
    Node* current = root;
    std::stack<Node*> nodes;
    nodes.push(current);
    while (!nodes.empty()) {
        current = nodes.top();
        nodes.pop();
        std::cout << current->Data << ' ';
        if (current->Right != nullptr) {
            nodes.push(current->Right);
        }
        if (current->Left != nullptr) {
            nodes.push(current->Left);
        }
    }
}

int main() {
    int n = 0;
    std::cin >> n;
    Tree tree;
    for (int i = 0; i < n; i++) {
        int tmp = 0;
        std::cin >> tmp;
        tree.Add(tmp);
    }
    tree.pre_order_printer();
}
