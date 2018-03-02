#include <iostream>
#include <vector>
#include <algorithm>

using index_t = size_t;
using value_t = long long;

enum class Color {
    NONE,
    RED,
    BLACK
};

Color switchColor(Color color) {
    switch (color) {
        case Color::RED:
            return Color::BLACK;

        case Color::BLACK:
            return Color::RED;

        default:
            return Color::NONE;
    }
}

struct Node;
std::vector<Node> nodes;

struct Node {
    Color color = Color::NONE;
    std::vector<index_t> neighbours;
    
    bool colorify(Color assignedColor);
};

bool colorifyNodes() {
    for (auto& node : nodes) {
        if (node.color == Color::NONE) {
            bool result = node.colorify(Color::RED);
            
            if (!result) {
                return false;
            }
        }
    }

    return true;
}

bool Node::colorify(Color assignedColor) {
    if (color == Color::NONE) {
        color = assignedColor;
        
        bool result = true;
        for (auto neighbourIndex : neighbours) {
            auto& neighbour = nodes[neighbourIndex];
            result = neighbour.colorify(switchColor(color));
            
            if (!result) {
                return false;
            }
        }

        return true;
    } else {
        return color == assignedColor;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    index_t n, m;
    std::cin >> n >> m;
    nodes.reserve(n);
    
    for (index_t i = 0; i < n; i++) {
        nodes.emplace_back();
    }
    
    for (index_t i = 0; i < m; i++) {
        index_t index1, index2;
        std::cin >> index1 >> index2;
        
        nodes[index1 - 1].neighbours.push_back(index2 - 1);
        nodes[index2 - 1].neighbours.push_back(index1 - 1);
    }
    
    bool result = colorifyNodes();
    std::cout << (result ? "TAK" : "NIE") << '\n';

    return 0;
}