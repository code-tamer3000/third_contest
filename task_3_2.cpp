/*
Дано число N и N строк. Каждая строка содержащит команду добавления или удаления натуральных чисел, а
также запрос на получение k-ой порядковой статистики. 
Команда добавления числа A задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”. 
Запрос на получение k-ой порядковой статистики задается числом k. Требуемая скорость выполнения запроса - O(log n).
Ввод:   Вывод:
3       1 1 2
1 0
2 0
-1 0
*/

#include <iostream>

struct AVLNode {
    int Key;
    int Height = 1; 
    int Count = 1;
    AVLNode* Left = nullptr;
    AVLNode* Right = nullptr;
 
    explicit AVLNode(int key) : Key(key) {}
    ~AVLNode() {
        delete Left;
        delete Right;
    }
};
 
int GetHeight(AVLNode* node) {
    return node ? node->Height : 0;
}
 
int GetCount(AVLNode* node) {
    return node ? node->Count : 0;
}
 
void UpdateHeight(AVLNode* node) {
    node->Height = std::max(GetHeight(node->Left), GetHeight(node->Right)) + 1;
}
 
void UpdateCount(AVLNode* node) {
    node->Count = GetCount(node->Left) + GetCount(node->Right) + 1;
}
 
int Balance(AVLNode* node) {
    return GetHeight(node->Left) - GetHeight(node->Right);
}
 
class AVLTree {
    public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { delete root; }
    int Add(int key);
    int Size() const { return GetCount(root); }
    int KStat(int k) const {
        return KStat(k, root);
    }
    void Remove(int key);
 
    private:
    AVLNode* root = nullptr;
 
    static int Add(int key, AVLNode*& node);
    static void Remove(int key, AVLNode*& node);
    static void RotateLeft(AVLNode*& node);
    static void RotateRight(AVLNode*& node);
    static void FixBalance(AVLNode*& node);
    static int KStat(int k, AVLNode* node);
};
 
int AVLTree::Add(int key) {
    return Add(key, root);
}
 
int AVLTree::Add(int key, AVLNode*& node) {
    if (!node) {
        node = new AVLNode(key);
        return 0;
    }
    int stat = 0;
    if (node->Key < key) {
        stat = Add(key, node->Right) + GetCount(node->Left) + 1;
    } else {
        stat = Add(key, node->Left);
    }

    UpdateHeight(node);
    UpdateCount(node);
    FixBalance(node);
    return stat;
}
 
void AVLTree::RotateLeft(AVLNode*& node) {
    AVLNode* right = node->Right;
    node->Right = right->Left;
    right->Left = node;
    node = right;
 
    UpdateHeight(node->Left);
    UpdateCount(node->Left);
    UpdateHeight(node);
    UpdateCount(node);
}
 
void AVLTree::RotateRight(AVLNode*& node) {
    AVLNode* left = node->Left;
    node->Left = left->Right;
    left->Right = node;
    node = left;
 
    UpdateHeight(node->Right);
    UpdateCount(node->Right);
    UpdateHeight(node);
    UpdateCount(node);
}
 
void AVLTree::FixBalance(AVLNode*& node) {
    int balance = Balance(node);
    if (balance == -2) {
        if (Balance(node->Right) == 1) {
            RotateRight(node->Right);
        }
        RotateLeft(node);
    } else if (balance == 2) {
        if (Balance(node->Left) == -1) {
            RotateLeft(node->Left);
        }
        RotateRight(node);
    }
}
 
int AVLTree::KStat(int k, AVLNode* node) {
    if (GetCount(node->Left) == k) {
        return node->Key;
    }
    if (GetCount(node->Left) > k) {
        return KStat(k, node->Left);
    }
    return KStat(k - GetCount(node->Left) - 1, node->Right);
}

void AVLTree::Remove(int key) {
    Remove(key, root);
}

void AVLTree::Remove(int key, AVLNode*& node) {
    if (!node) {
        return;
    }
    if (node->Key < key) {
        Remove(key, node->Right);
    } else if (key < node->Key) {
        Remove(key, node->Left);
    } else {
        if (node->Count == 1) {
            AVLNode* tmp = node;
            node = nullptr;
            delete tmp;
            return;
        } else if (GetCount(node->Right) == 0 || GetCount(node->Left) == 0) {
            if (node->Left == nullptr) {
                node->Key = node->Right->Key;
                Remove(node->Key, node->Right);
            } else {
                node->Key = node->Left->Key;
                Remove(node->Key, node->Left);
            }
        } else {
            AVLNode* tmp = node->Right;
            while (tmp->Left) {
                tmp = tmp->Left;
            }
            node->Key = tmp->Key;
            Remove(node->Key, node->Right);
        }
    }
    
    UpdateHeight(node);
    UpdateCount(node);
    FixBalance(node);
}

int main() {
    int n = 0; 
    std::cin >> n;
    AVLTree tree;
    for (int i = 0; i < n; i++){
        int num = 0; 
        int stat = 0;
        std::cin >> num >> stat;
        if (num < 0) {
            tree.Remove(-num);
        } else {
            tree.Add(num);
        }
        std::cout << tree.KStat(stat) << ' ';
    }
    std::cout << std::endl;
    return 0;
}
 