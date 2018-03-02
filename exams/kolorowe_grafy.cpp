#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <tuple>

using index_t = long;

struct Node;
struct Edge;

struct Edge {
    Edge(index_t destination, bool color) : destination(destination), color(color) {}
    index_t destination;
    bool color;
};

struct Node {
    Node() = default;
    explicit Node(index_t index) : index(index) {}
    index_t index;
    std::vector<Edge> edges;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    index_t n, m;
    std::cin >> n >> m;

    std::vector<Node> nodes;
    nodes.reserve(static_cast<size_t>(n + 1));
    nodes.emplace_back();

    for (index_t i = 1; i <= n; i++) {
        nodes.emplace_back(i);
    }

    for (index_t i = 0; i < m; i++) {
        bool isGreen;
        index_t index1, index2;
        std::cin >> index1 >> index2 >> isGreen;
        nodes[index1].edges.emplace_back(index2, isGreen);
        nodes[index2].edges.emplace_back(index1, isGreen);
    }

    bool visited[n+1][2];
    index_t distanceArray[n+1];
    for (index_t i = 0; i <= n; i++) {
        distanceArray[i] = std::numeric_limits<index_t>::max();
        visited[i][false] = false;
        visited[i][true] = false;
    }

    std::queue<std::tuple<index_t, bool, index_t>> queue;
    queue.emplace(1, false, 0);
    queue.emplace(1, true, 0);
    visited[1][false] = false;
    visited[1][true] = false;

    while (!queue.empty()) {
        index_t currentIndex = std::get<0>(queue.front());
        bool currentColor = std::get<1>(queue.front());
        index_t currentDistance = std::get<2>(queue.front());
        queue.pop();

        auto& node = nodes[currentIndex];
        distanceArray[currentIndex] = std::min(distanceArray[currentIndex], currentDistance);

        for (const auto& edge : node.edges) {
            if (edge.color != currentColor && !visited[edge.destination][edge.color]) {
                queue.emplace(edge.destination, edge.color, currentDistance + 1);
                visited[edge.destination][edge.color] = true;
            }
        }
    }

    for (index_t i = 2; i <= n; i++) {
        std::cout << (distanceArray[i] == std::numeric_limits<index_t>::max() ? -1 : distanceArray[i]) << '\n';
    }

    return 0;
}