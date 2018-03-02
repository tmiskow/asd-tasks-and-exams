#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>

using index_t = unsigned long long;

struct Node;
std::vector<Node> nodes;

struct Node {
    explicit Node(index_t index) : index(index) {}

    const index_t index;
    index_t size = 1;
    index_t sum = 0;
    std::vector<index_t> neighbours;

    void update(index_t value, index_t last, index_t lastSize) {
        sum += value;

        for (auto neighbour : neighbours) {
            if (neighbour != last) {
                index_t updateValue = sum - (nodes[neighbour].sum + nodes[neighbour].size) + (size - nodes[neighbour].size);
                nodes[neighbour].update(updateValue + lastSize, index, lastSize + size - nodes[neighbour].size);
            }
        }
    }

    void calculate(index_t last) {
        for (auto neighbour : neighbours) {
            if (neighbour != last) {
                nodes[neighbour].calculate(index);
                size += nodes[neighbour].size;
                sum += nodes[neighbour].sum + nodes[neighbour].size;
            }
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    index_t n;
    std::cin >> n;
    for (index_t i = 0; i < n; i++) {
        nodes.emplace_back(i);
    }

    for (index_t i = 0; i < n-1; i++) {
        index_t index1, index2;
        std::cin >> index1 >> index2;
        nodes[index1-1].neighbours.push_back(index2-1);
        nodes[index2-1].neighbours.push_back(index1-1);
    }

    nodes[0].calculate(0);
    nodes[0].update(0, 0, 0);

    for (index_t i = 0; i < n; i++) {
        std::cout << nodes[i].sum  << '\n';
    }

    return 0;
}