#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>

using index_t = std::size_t;
using color_t = std::size_t;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    index_t n, l;
    std::cin >> n >> l;

    std::vector<std::pair<color_t, index_t>> pairs;
    pairs.reserve(n * l);

    for (index_t i = 0; i < n; i++) {
        for (index_t j = 0; j < l; j++) {
            color_t color;
            std::cin >> color;
            pairs.emplace_back(color, j);
        }
    }

    std::sort(pairs.begin(), pairs.end());

    index_t result = 0;
    index_t current = 1;

    for (index_t i = 0; i < pairs.size()-1; i++) {
        if (pairs[i].first == pairs[i+1].first) {
            if (pairs[i+1].second - pairs[i].second == 1) {
                current++;
            } else if (pairs[i+1].second - pairs[i].second > 1) {
                result = std::max(result, current);
                current = 1;
            }
        } else {
            result = std::max(result, current);
            current = 1;
        }
    }

    std::cout << std::max(result, current) << '\n';

    return 0;
}