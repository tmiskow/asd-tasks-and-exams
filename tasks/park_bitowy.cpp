#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

class Tree;
class Node;

class Tree {
    friend class Node;
private:

    std::size_t size;
    std::size_t linksSize{};
    std::vector<Node> nodes;

    Node& getNode(unsigned index);
    Node& getRoot();
    Node& getLowestCommonAncestor(Node &node1, Node &node2);

public:
    explicit Tree(std::size_t size);

    void preprocessing();
    void setupRelations();
    void setupDepths();
    void setupLinks();

    void setupFarthestDown();
    void setupFarthestUp();
    void setupFarthest();

    unsigned int getNodeIndexByDistance(unsigned index, unsigned distance);
};

class Node {
    friend class Tree;
private:
    Tree& tree;
    std::vector<unsigned> links;

    unsigned index = 0;
    unsigned parentIndex = 0;
    unsigned leftChildIndex = 0;
    unsigned rightChildIndex = 0;

    unsigned farthestDownIndex = 0;
    unsigned farthestDownDistance = 0;

    unsigned farthestUpIndex = 0;
    unsigned farthestUpDistance = 0;

    unsigned farthestIndex = 0;
    unsigned farthestDistance = 0;

    unsigned depth = 0;

    bool hasLeftChild();
    bool hasRightChild();
    bool hasSibling();
    bool hasAncestor(unsigned distance);
    bool isLeaf();
    bool isRoot();

    Node& getParent();
    Node& getLeftChild();
    Node& getRightChild();
    Node& getSibling();
    Node& getAncestor(unsigned distance);

    void setParent(unsigned parentIndex);
    void setLeftChild(unsigned leftChildIndex);
    void setRightChild(unsigned rightChildIndex);
    void setDepthRecursively(unsigned depth);

    void setFarthestDownRecursively();
    void setFarthestUp();
    void setFarthest();

public:
    explicit Node(Tree& tree, unsigned index);
};

Tree::Tree(std::size_t size) : size(size), linksSize((size_t)(floor(log2(size)))) {
    for (unsigned i = 1; i <= size; i++) {
        nodes.emplace_back(*this, i);
    }
}

Node& Tree::getNode(unsigned index) {
    assert(1 <= index && index <= size);
    return nodes[index-1];
}

Node& Tree::getRoot() {
    return getNode(1);
}

Node& Tree::getLowestCommonAncestor(Node &node1, Node &node2) {
    auto index1 = node1.index;
    auto index2 = node2.index;
    auto depth1 = node1.depth;
    auto depth2 = node2.depth;

    if (depth1 > depth2) {
        auto& ancestor1 = node1.getAncestor(depth1 - depth2);
        index1 = ancestor1.index;
        depth1 = ancestor1.depth;
    } else if (depth1 < depth2) {
        auto& ancestor2 = node2.getAncestor(depth2 - depth1);
        index2 = ancestor2.index;
        depth2 = ancestor2.depth;
    }

    assert(depth1 == depth2);
    if (index1 == index2) {
        return getNode(index1);
    }

    auto i = static_cast<int>(linksSize);

    while (i >= 0) {
        auto linkedIndex1 = getNode(index1).links[i];
        auto linkedIndex2 = getNode(index2).links[i];

        if (linkedIndex1 != linkedIndex2) {
            index1 = linkedIndex1;
            index2 = linkedIndex2;
        }

        i--;
    }

    return getNode(index1).getParent();
}

void Tree::setupRelations() {
    for (unsigned i = 1; i <= size; i++) {
        auto& node = getNode(i);

        if (i == 1) {
            node.setParent(0);
        }

        // setup left child
        int leftChildIndex;
        std::cin >> leftChildIndex;

        if (leftChildIndex == -1) {
            node.setLeftChild(0);
        } else {
            node.setLeftChild(static_cast<unsigned>(leftChildIndex));

            auto& leftChild = getNode(static_cast<unsigned>(leftChildIndex));
            leftChild.setParent(i);
        }

        // setup right child
        int rightChildIndex;
        std::cin >> rightChildIndex;

        if (rightChildIndex == -1) {
            node.setRightChild(0);
        } else {
            node.setRightChild(static_cast<unsigned>(rightChildIndex));

            auto& rightChild = getNode(static_cast<unsigned>(rightChildIndex));
            rightChild.setParent(i);
        }
    }
}

void Tree::setupDepths() {
    getRoot().setDepthRecursively(0);
}

void Tree::setupLinks() {
    for (auto& node : nodes) {
        node.links.push_back(node.parentIndex);
    }

    for (unsigned i = 1; i <= linksSize; i++) {
        for (auto& node : nodes) {
            if (node.links[i-1] == 0) {
                node.links.push_back(0);
            } else {
                auto& linkedNode = getNode(node.links[i-1]);
                node.links.push_back(linkedNode.links[i-1]);
            }
        }
    }
}

void Tree::setupFarthestDown() {
    getRoot().setFarthestDownRecursively();
}

void Tree::setupFarthestUp() {
    for (auto& node : nodes) {
        node.setFarthestUp();
    }
}

void Tree::setupFarthest() {
    for (auto& node : nodes) {
        node.setFarthest();
    }
}

void Tree::preprocessing() {
    setupRelations();
    setupDepths();
    setupLinks();
    setupFarthestDown();
    setupFarthestUp();
    setupFarthest();
}

unsigned int Tree::getNodeIndexByDistance(unsigned index, unsigned distance) {
    auto& node = getNode(index);

    auto maxDistance = node.farthestDistance;
    auto& farthestNode = getNode(node.farthestIndex);

    if (distance > maxDistance) {
        return 0;
    } else {
        auto& lowestCommonAncestor = getLowestCommonAncestor(node, farthestNode);
        auto distance1 = node.depth - lowestCommonAncestor.depth;
//        auto distance2 = farthestNode.depth - lowestCommonAncestor.depth;

        if (distance <= distance1) {
            return node.getAncestor(distance).index;
        } else {
            return farthestNode.getAncestor(maxDistance - distance).index;
        }
    }
}

Node::Node(Tree& tree, unsigned index) : tree(tree), index(index) {}

bool Node::hasLeftChild() {
    return leftChildIndex != 0;
}

bool Node::hasRightChild() {
    return rightChildIndex != 0;
}

bool Node::hasSibling() {
    if (isRoot()) {
        return false;
    } else {
        auto& parent = getParent();

        if (parent.hasLeftChild()) {
            if (parent.getLeftChild().index == index) {
                return parent.hasRightChild();
            } else {
                assert(parent.hasRightChild() && parent.getRightChild().index == index);
                return true;
            }
        } else {
            assert(parent.hasRightChild() && parent.getRightChild().index == index);
            return false;
        }
    }
}

bool Node::hasAncestor(unsigned distance) {
    return 0 <= distance && distance <= depth;
}

bool Node::isLeaf() {
    return !hasLeftChild() && !hasRightChild();
}

bool Node::isRoot() {
    return index == 1;
}

Node &Node::getParent() {
    assert(!isRoot());
    return tree.getNode(parentIndex);
}

Node &Node::getLeftChild() {
    assert(hasLeftChild());
    return tree.getNode(leftChildIndex);
}

Node &Node::getRightChild() {
    assert(hasRightChild());
    return tree.getNode(rightChildIndex);
}

Node &Node::getSibling() {
    assert(hasSibling());

    auto& parent = getParent();
    if (parent.getLeftChild().index == index) {
        return parent.getRightChild();
    } else {
        return parent.getLeftChild();
    }
}

Node &Node::getAncestor(unsigned distance) {
    auto currentIndex = index;
    auto i = tree.linksSize;

    while (distance > 0) {
        if (pow(2, i) > distance) {
            i--;
        } else {
            assert(currentIndex != 0);
            auto& currentNode = tree.getNode(currentIndex);
            auto& linkedNode = tree.getNode(currentNode.links[i]);

            currentIndex = linkedNode.index;
            distance -= pow(2, i);
        }
    }

    return tree.getNode(currentIndex);
}

void Node::setParent(unsigned parentIndex) {
    this->parentIndex = parentIndex;
}

void Node::setLeftChild(unsigned leftChildIndex) {
    this->leftChildIndex = leftChildIndex;
}

void Node::setRightChild(unsigned rightChildIndex){
    this->rightChildIndex = rightChildIndex;
}

void Node::setDepthRecursively(unsigned depth) {
    this->depth = depth;

    if (hasLeftChild()) {
        getLeftChild().setDepthRecursively(depth + 1);
    }

    if (hasRightChild()) {
        getRightChild().setDepthRecursively(depth + 1);
    }
}

void Node::setFarthestDownRecursively() {
    if (farthestDownIndex == 0) {
        farthestDownIndex = index;
        farthestDownDistance = 0;

        if (!isLeaf()) {
            if (hasLeftChild()) {
                getLeftChild().setFarthestDownRecursively();

                unsigned leftDistance = getLeftChild().farthestDownDistance + 1;
                if (leftDistance > farthestDownDistance) {
                    farthestDownDistance = leftDistance;
                    farthestDownIndex = getLeftChild().farthestDownIndex;
                }
            }

            if (hasRightChild()) {
                getRightChild().setFarthestDownRecursively();

                unsigned rightDistance = getRightChild().farthestDownDistance + 1;
                if (rightDistance > farthestDownDistance) {
                    farthestDownDistance = rightDistance;
                    farthestDownIndex = getRightChild().farthestDownIndex;
                }
            }
        }
    }
}

void Node::setFarthestUp() {
    if (farthestUpIndex == 0) {
        farthestUpIndex = index;
        farthestUpDistance = 0;

        if (!isRoot()) {
            auto& parent = getParent();
            parent.setFarthestUp();

            unsigned parentDistance = parent.farthestUpDistance + 1;
            if (parentDistance > farthestUpDistance) {
                farthestUpDistance = parentDistance;
                farthestUpIndex = parent.farthestUpIndex;
            }
        }

        if (hasSibling()) {
            auto& sibling = getSibling();

            unsigned siblingDistance = sibling.farthestDownDistance + 2;
            if (siblingDistance > farthestUpDistance) {
                farthestUpDistance = siblingDistance;
                farthestUpIndex = sibling.farthestDownIndex;
            }
        }
    }
}

void Node::setFarthest() {
    assert(farthestIndex == 0 && farthestDistance == 0);

    if (farthestDownDistance > farthestUpDistance) {
        farthestDistance = farthestDownDistance;
        farthestIndex = farthestDownIndex;
    } else {
        farthestDistance = farthestUpDistance;
        farthestIndex = farthestUpIndex;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);

    unsigned n;
    std::cin >> n;

    Tree tree = Tree(n);
    tree.preprocessing();

    unsigned m;
    std::cin >> m;

    for (unsigned i = 0; i < m; i++) {
        unsigned index;
        unsigned distance;

        std::cin >> index;
        std::cin >> distance;

        unsigned foundIndex = tree.getNodeIndexByDistance(index, distance);

        if (foundIndex != 0) {
            std::cout << foundIndex << "\n";
        } else {
            std::cout << -1 << "\n";
        }
    }

    return 0;
}