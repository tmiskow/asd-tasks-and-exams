#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

using index_t = size_t;
using value_t = long long;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    index_t n;
    std::cin >> n;

    std::vector<std::tuple<value_t, value_t, bool>> vector;
    vector.reserve(2 * n);

    for (index_t i = 0; i < n; i++) {
        value_t x, start, end;
        std::cin >> x >> start >> end;

        vector.emplace_back(x, start, true);
        vector.emplace_back(x, end, false);
    }

    std::sort(vector.begin(), vector.end());

    value_t counter = 0;
    value_t active = 0;

    for (auto& tuple : vector) {
        if (std::get<2>(tuple)) {
            active++;
        } else {
            active--;
            counter += active;
        }
    }

    std::cout << counter << '\n';

    return 0;
}