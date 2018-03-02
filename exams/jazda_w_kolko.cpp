#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>
#include <unordered_set>

using index_t = std::size_t;

struct Node;
std::vector<Node> nodes;

struct Node {
    explicit Node(index_t index) : index(index) {}

    index_t index;
    bool processed = false;
    bool visited = false;
    std::vector<index_t> neighbours;

    bool search(index_t lastIndex) {
        processed = true;

        for (index_t neighbourIndex : neighbours) {
            if (neighbourIndex != lastIndex) {
                auto& neighbour = nodes[neighbourIndex];

                if (neighbour.visited) {
                    return true;
                } else {
                    neighbour.visited = true;

                    if (!neighbour.processed) {
                        if (neighbour.search(index)) {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    index_t n, m;
    std::cin >> n >> m;

    nodes.reserve(n);
    for (index_t i = 0; i < n; i++) {
        nodes.emplace_back(i);
    }

    for (index_t i = 0; i < m; i++) {
        index_t index1, index2;
        std::cin >> index1 >> index2;

        nodes[index1-1].neighbours.push_back(index2-1);
        nodes[index2-1].neighbours.push_back(index1-1);
    }

    for (auto& node : nodes) {
        if (!node.processed) {
            node.visited = true;
            if (node.search(node.index)) {
                std::cout << "TAK\n";
                return 0;
            }
        }
    }
    std::cout << "NIE\n";

    return 0;
}