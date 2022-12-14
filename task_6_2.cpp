/*
В большой IT-фирме есть только одна переговорная комната. Желающие посовещаться заполняют заявки с желаемым временем начала и конца.
Ваша задача определить максимальное количество заявок, которое может быть удовлетворено. Число заявок ≤ 100000.

Формат ввода
Вход содержит только пары целых чисел — начала и концы заявок.

Формат вывода
Выход должен содержать натуральное число — максимальное число заявок.
*/

#include <iostream>
#include <queue>

struct time_line {
    int start;
    int end;
    explicit time_line(int start, int end) : start(start), end(end) {}
};

class comp {
  public:
    bool operator()(const time_line& first, const time_line& second) {
        if (first.end == second.end) {
            return second.start < first.start;
        } else {
            return second.end < first.end;
        }
    }
};

int main() {
    std::priority_queue<time_line, std::vector<time_line>, comp> requests;
    int start = 0;
    int end = 0;
    while (std::cin >> start >> end) {
        requests.push(time_line(start, end));
    }
    int counter = 0;
    int nearest_start = requests.top().start;
    while (!requests.empty()) {
        if (nearest_start <= requests.top().start) {
            nearest_start = requests.top().end;
            counter++;
        }
        requests.pop();
    }
    std::cout << counter << std::endl;
    return 0;
}
