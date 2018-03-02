#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>

using index_t = unsigned long long;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    index_t length;
    std::cin >> length;

    index_t counters[4][4];
    for (index_t i = 0; i <= 3; i++) {
        for (index_t j = 0; j <= 3; j++) {
            counters[i][j] = 0;
        }
    }

    for (index_t i = 0; i < length; i++) {
        index_t value;
        std::cin >> value;

        switch (value) {
            case 1:
                counters[1][1]++;
                break;

            case 2:
                counters[1][2]++;
                counters[2][2] += counters[1][1];
                break;

            case 3:
                counters[1][3]++;
                counters[2][3] += counters[1][1];
                counters[2][3] += counters[1][2];
                counters[3][3] += counters[2][2];
        }
    }

    index_t sum = 0;
    for (index_t i = 0; i <= 3; i++) {
        for (index_t j = 0; j <= 3; j++) {
            sum += counters[i][j];
        }
    }

    std::cout << sum << '\n';

    return 0;
}