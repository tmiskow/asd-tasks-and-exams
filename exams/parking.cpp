#include <iostream>
#include <vector>
#include <algorithm>

using index_t = size_t;
using value_t = long long;

struct Node;
std::vector<Node> nodes;
std::vector<index_t> reachedCars;

struct Node {
    Node() = default;
    Node(index_t index, bool car) : index(index), car(car), visited(false) {}

    bool car;
    bool visited;
    index_t index;
    std::vector<index_t> neighbours;

    void dfs();
};

void Node::dfs() {
    if (!visited) {
        visited = true;

        if (car) {
            reachedCars.push_back(index);
        } else {
            for (auto neighbourIndex : neighbours) {
                nodes[neighbourIndex].dfs();
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    index_t n, m;
    std::cin >> n >> m;

    nodes.reserve(n+1);
    nodes.emplace_back();

    for (index_t i = 1; i <= n; i++) {
        bool car;
        std::cin >> car;
        nodes.emplace_back(i, car);
    }

    for (index_t i = 0; i < m; i++) {
        index_t index1, index2;
        std::cin >> index1 >> index2;
        nodes[index1].neighbours.push_back(index2);
        nodes[index2].neighbours.push_back(index1);
    }

    nodes[1].dfs();
    std::sort(reachedCars.begin(), reachedCars.end());

    for (auto car : reachedCars) {
        std::cout << car << '\n';
    }

    return 0;
}