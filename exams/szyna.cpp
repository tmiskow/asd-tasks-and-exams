#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using index_t = size_t;
using value_t = long long;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    index_t n;
    std::cin >> n;

    std::set<value_t> set;

    index_t counter = 0;
    for (index_t i = 0; i < n; i++) {
        value_t value;
        std::cin >> value;

        if (set.count(value)) {
            counter++;
            set.clear();
        } else {
            set.insert(value);
        }
    }

    std::cout << counter << '\n';

    return 0;
}