/*
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера. Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.

Вариант 2. Для разрешения коллизий используйте двойное хеширование.

Формат ввода
Каждая строка входных данных задает одну операцию над множеством. 
Запись операции состоит из типа операции и следующей за ним через пробел строки, над которой проводится операция. 
Тип операции – один из трех символов: + означает добавление данной строки в множество; - означает удаление строки из множества;
 ? означает проверку принадлежности данной строки множеству. 
 При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве. 
 При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.

Формат вывода
Программа должна вывести для каждой операции одну из двух строк OK или FAIL.

Для операции '?': OK, если элемент присутствует во множестве. FAIL иначе.

Для операции '+': FAIL, если добавляемый элемент уже присутствует во множестве и потому не может быть добавлен. OK иначе.

Для операции '-': OK, если элемент присутствовал во множестве и успешно удален. FAIL иначе.
*/

#include <iostream>
#include <vector>
#include <string>
#include <cassert>

class HashTable {
  public:
    explicit HashTable(int initial_size = 8) : table(initial_size) , fill_factor(initial_size) , curr_fill(0) {}
    bool Has(const std::string& key) const;
    bool Add(const std::string& key);
    bool Remove(const std::string& key);
 
  private:
    int fill_factor;
    int curr_fill;
    void grow();
    struct HashNode {
        std::string key;
        bool alive;
        HashNode() : alive(false) {}
    };
    std::vector<HashNode> table;
};

size_t hash_1(const std::string& key) { // полиномиальное хэширование
    const size_t param = 31;
    const size_t mod = 1e9+7;
    size_t param_in_pow = 1;
    size_t hash_val = 0;
    for (size_t i  = 0; i < key.length(); i++) {
        hash_val += ((key[i] - 'a' + 1) * param_in_pow) % mod;
        param_in_pow *= param;
    }
    return hash_val;
}

size_t hash_2(const std::string& key) { // метод горнера
    const size_t param = 30;
    size_t hash_val = 0;
    const size_t mod = 1e9+7;
    for (size_t i  = 0; i < key.length(); i++) {
        hash_val = (hash_val * param + key[i]) % mod;
    }
    hash_val = (hash_val * 2 + 1) % mod;
    if (hash_val == 0) {
        hash_val = key[0];
    }
    return hash_val;
}

bool HashTable::Add(const std::string& key) {
    size_t hash1 = hash_1(key) % table.size();
    size_t hash2 = hash_2(key) % table.size();
    bool found_place_to_add = false;
    size_t curr_hash = hash1;
    for (size_t counter = 0; counter < table.size(); counter++) {
        if (table[hash1].alive) {
            if (table[hash1].key == key) {
                return false;
            }
        } else if (!found_place_to_add) {
            curr_hash = hash1;
            found_place_to_add = true;
        } else if (table[hash1].key == "") {
            break;
        }
        hash1 = (hash1 + hash2) % table.size();
    }
    table[curr_hash].alive = true;
    if (table[curr_hash].key == "") {
        curr_fill++;
    }
    table[curr_hash].key = key;
    if (curr_fill == fill_factor * 0.75) {
        grow();
    }
    return true;
}

bool HashTable::Remove(const std::string& key) {
    size_t hash1 = hash_1(key) % table.size();
    size_t hash2 = hash_2(key) % table.size();
    for (size_t i  = 0; i < table.size(); i++) {
        if (table[hash1].alive) {
            if (table[hash1].key == key) {
                table[hash1].alive = false;
                return true;
            }
        } else if (table[hash1].key == "") {
            return false;
        } 
        hash1 = (hash1 + hash2) % table.size();
    }
    return false;
}

bool HashTable::Has(const std::string& key) const {
    size_t hash1 = hash_1(key) % table.size();
    size_t hash2 = hash_2(key) % table.size();
    for (size_t i  = 0; i < table.size(); i++) {
        if (table[hash1].alive) {
            if (table[hash1].key == key) {
                return true;
            }
        } else if (table[hash1].key == "") {
            return false;
        } 
        hash1 = (hash1 + hash2) % table.size();
    }
    return false;
}

void HashTable::grow() {
    HashTable new_table(fill_factor * 2);
    for (const auto& node : table) { 
        if (node.alive) {
            new_table.Add(node.key);
        }
    }
    *this = std::move(new_table);
}

int main() {
    HashTable my_table;
    char command = 0;
    std::string key;
    while (std::cin >> command >> key) {
        assert(command == '+' || command == '-' || command == '?');
        switch (command) {
        case '?':
            std::cout << (my_table.Has(key) ? "OK" : "FAIL") << std::endl;
            break;
        case '+':
            std::cout << (my_table.Add(key) ? "OK" : "FAIL") << std::endl;
            break;
        case '-':
            std::cout << (my_table.Remove(key) ? "OK" : "FAIL") << std::endl;
            break;
        }
    }
    return 0;
}
