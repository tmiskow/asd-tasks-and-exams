#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>

using index_t = std::size_t;
using value_t = unsigned long long;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    index_t n;
    std::cin >> n;

    std::vector<value_t> allowedVector;
    std::vector<value_t> notAllowedVector;
    value_t laster = 0;
    value_t last = 0;
    for (index_t i = 0; i < n; i++) {
        value_t current;
        std::cin >> current;

        if (i > 3) {
            value_t notAllowed = notAllowedVector[i-1];
            notAllowed = std::max(notAllowed, notAllowedVector[i-2] + current);
            notAllowed = std::max(notAllowed, notAllowedVector[i-3] + last + current);
            notAllowedVector.push_back(notAllowed);

            value_t allowed = allowedVector[i-1];
            allowed = std::max(allowed, notAllowedVector[i-1] + current);
            allowed = std::max(allowed, allowedVector[i-2] + current);
            allowed = std::max(allowed, allowedVector[i-3] + last + current);
            allowed = std::max(allowed, notAllowedVector[i-4] + laster + last + current);
            allowed = std::max(allowed, notAllowed);
            allowedVector.push_back(allowed);
        } else if (i == 3) {
            value_t notAllowed = notAllowedVector[i-1];
            notAllowed = std::max(notAllowed, notAllowedVector[i-2] + current);
            notAllowed = std::max(notAllowed, notAllowedVector[i-3] + last + current);
            notAllowedVector.push_back(notAllowed);

            value_t allowed = allowedVector[i-1];
            allowed = std::max(allowed, notAllowedVector[i-1] + current);
            allowed = std::max(allowed, allowedVector[i-2] + current);
            allowed = std::max(allowed, allowedVector[i-3] + last + current);
            allowed = std::max(allowed, notAllowed);
            allowedVector.push_back(allowed);
        } else if (i == 2) {
            value_t notAllowed = std::max(laster + last, last + current);
            notAllowed = std::max(notAllowed, laster + current);
            notAllowedVector.push_back(notAllowed);
            allowedVector.push_back(laster + last + current);
        } else if (i == 1) {
            notAllowedVector.push_back(current + last);
            allowedVector.push_back(current + last);
        } else {
            notAllowedVector.push_back(current);
            allowedVector.push_back(current);
        }

        laster = last;
        last = current;
    }

//    for (index_t i = 0; i < n; i++) {
//        std::cout << notAllowedVector[i] << " " << allowedVector[i] << "\n";
//    }

    std::cout << std::max(notAllowedVector.back(), allowedVector.back()) << '\n';

    return 0;
}

//7
//7 9 9 6 5 5 5