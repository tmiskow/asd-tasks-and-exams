#include <iostream>
#include <algorithm>
#include <vector>

using index_t = size_t;
using value_t = long long;

int main() {
    index_t n;
    std::cin >> n;

    value_t d;
    std::cin >> d;

    bool isNegative = d < 0;
    d = std::abs(d);

    std::vector<value_t> vector;
    for (index_t i = 0; i < n; i++) {
        value_t temp;
        std::cin >> temp;
        vector.push_back(temp);
    }

    std::sort(vector.begin(), vector.end());

    index_t i  = 0;
    index_t j = 0;

    bool continueFlag = true;
    while (continueFlag && j < n) {
        if (vector[j] - vector[i] == d && i != j) {
            continueFlag = false;
        } else if (vector[j] - vector[i] < d || i == j) {
            j++;
        } else {
            i++;
        }
    }

    if (continueFlag) {
        std::cout << "NIE\n";
    } else if (!isNegative) {
        std::cout <<  vector[j] <<  ' ' <<vector[i] <<'\n';
    } else {
        std::cout <<  vector[i] <<  ' ' <<vector[j] <<'\n';
    }
}