#include <iostream>
#include <vector>
#include <cassert>

enum class Color : char {
    WHITE = 'W',
    BLACK = 'B',
    MIXED = 'M',
    NONE = 'N'
};

Color addColors(Color color1, Color color2) {
    assert(color1 != Color::NONE && color2 != Color::NONE);

    if (color1 == color2 && color1 != Color::MIXED) {
        return color1;
    } else {
        return Color::MIXED;
    }
}

class Tree;
class Node;

class Tree {

    friend class Node;

private:
    std::size_t size;
    std::size_t width;
    std::vector<Node> nodes;

    Node& getNode(unsigned index);
    Node& getRoot();
public:
    explicit Tree(std::size_t numberOfLeafs);

    void paint(unsigned start, unsigned end, Color color);
    unsigned getWhiteKilometers();

    void print();
};

class Node {

    friend class Tree;

private:
    Tree& tree;

    Color color = Color::NONE;
    Color toPaint = Color::NONE;
    unsigned index;
    unsigned left, right;
    unsigned whiteKilometers = 0;

    bool isLeaf();
    bool isRoot();
    bool representsRange(unsigned start, unsigned end);
    bool containsRange(unsigned start, unsigned end);
    bool shouldRefresh();

    unsigned middle();
    unsigned representedKilometers();

    Node& getParent();
    Node& getLeftChild();
    Node& getRightChild();

    void refresh();
    void updateUpwards();
    void build(unsigned left, unsigned right);
    void paint(unsigned left, unsigned right, Color color);
public:
    Node(unsigned index, Tree& tree);
};

Tree::Tree(std::size_t width) : width(width) {
    size = static_cast<std::size_t>(4 * width - 1);

    nodes.reserve(size);
    for (auto index = 1; index <= size; index++) {
        nodes.emplace_back(index, *this);
    }

    getRoot().build(1, static_cast<unsigned>(width));
}

Node &Tree::getNode(unsigned index) {
    return nodes[index - 1];
}

Node &Tree::getRoot() {
    return getNode(1);
}

void Tree::paint(unsigned start, unsigned end, Color color) {
    getRoot().paint(start, end, color);
}

unsigned Tree::getWhiteKilometers() {
    getRoot().refresh();
    return getRoot().whiteKilometers;
}

void Tree::print() {
    std::cout << "\n";

    unsigned powerOf2 = 2;

    for (unsigned i = 1; i <= size; i++) {
        std::cout << (char) getNode(i).color << ' ';

        if (i == powerOf2 - 1) {
            std::cout << "\n";
            powerOf2 *= 2;
        }
    }
}

Node::Node(unsigned index, Tree& tree) : index(index), tree(tree) {}

bool Node::isLeaf() {
    return left == right;
}

bool Node::isRoot() {
    return index == 1;
}


bool Node::representsRange(unsigned start, unsigned end) {
    return left == start && end == right;
}

bool Node::containsRange(unsigned start, unsigned end) {
    return left <= start && end <= right;
}

bool Node::shouldRefresh() {
    return toPaint != Color::NONE;
}

unsigned Node::middle() {
    return (left + right) / 2;
}

unsigned Node::representedKilometers() {
    return right - left + 1;
}

Node &Node::getParent() {
    return tree.getNode(index/2);
}

Node &Node::getLeftChild() {
    return tree.getNode(2*index);
}

Node &Node::getRightChild() {
    return tree.getNode(2*index+1);
}

void Node::refresh() {
    if (shouldRefresh()) {
        color = toPaint;

        if (color == Color::WHITE) {
            whiteKilometers = representedKilometers();
        } else if (color == Color::BLACK) {
            whiteKilometers = 0;
        }

        if (!isLeaf()) {
            getLeftChild().toPaint = toPaint;
            getRightChild().toPaint = toPaint;
        }

        toPaint = Color::NONE;
    }
}

void Node::updateUpwards() {
    if (!isLeaf()) {
        getLeftChild().refresh();
        getRightChild().refresh();
        whiteKilometers = getLeftChild().whiteKilometers + getRightChild().whiteKilometers;
        color = addColors(getLeftChild().color, getRightChild().color);
    }

    if (!isRoot()) {
        getParent().updateUpwards();
    }
}

void Node::build(unsigned start, unsigned end) {
    left = start;
    right = end;
    color = Color::BLACK;

    if (!isLeaf()) {
        getLeftChild().build(left, middle());
        getRightChild().build(middle() + 1, right);
    }
}

void Node::paint(unsigned start, unsigned end, Color color) {
    refresh();

    if (start <= end) {
        if (representsRange(start, end)) {
            toPaint = color;
            refresh();

            if (!isLeaf()) {
                getLeftChild().refresh();
                getRightChild().refresh();
            }

            updateUpwards();
        } else if (containsRange(start, end)) {
            getLeftChild().paint(start, std::min(middle(), end), color);
            getRightChild().paint(std::max(start, middle() + 1), end, color);
        }
    }
}

unsigned nextUnsigned() {
    unsigned tempInt;
    std::cin >> tempInt;
    return tempInt;
}

Color nextColor() {
    char tempChar;
    std::cin >> tempChar;

    switch (tempChar) {
        case 'B':
            return Color::WHITE;

        case 'C':
            return Color::BLACK;

        default:
            return Color::NONE;
    }
}

int main() {
    std::ios::sync_with_stdio(false);

    int n = nextUnsigned();
    Tree tree = Tree(static_cast<size_t>(n));

    int m = nextUnsigned();
    for (auto i = 0; i < m; i++) {
        auto start = nextUnsigned();
        auto end  = nextUnsigned();
        auto color = nextColor();
        assert(color != Color::NONE);

        tree.paint(start, end, color);
        std::cout << tree.getWhiteKilometers() << '\n';
    }


    return 0;
}