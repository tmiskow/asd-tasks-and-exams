#include <iostream>
#include <algorithm>
#include <vector>

typedef unsigned index_t;
typedef long long value_t;

const value_t MINIMUM = 0;
const value_t MAXIMUM = 2000000000;

/* DECLARATIONS */

struct Tree;
struct Node;

index_t profits = 0;

struct Tree {
	std::vector<Node> nodes;
	index_t* leafIndexes;
	index_t size;
	index_t width;

	Tree(value_t*, index_t);
	Node& node(index_t index);
	Node& root();
	Node& leaf(index_t);
	void build(value_t*);
	void updateRange(index_t, index_t, value_t);
	void updateProfits(index_t, index_t, value_t);
	bool isPossibleToUpdateRange(index_t, index_t, value_t);
};

struct Node {
	Tree& tree;

	index_t index;
	index_t left, right;
	value_t min, max;
	value_t addAll;

	Node(Tree& tree) : tree(tree) {
		min = 0;
		max = 0;
		addAll = 0;
	}

	bool isLeaf();
	Node& parent();
	Node& leftChild();
	Node& rightChild();
	value_t value();
	value_t rangeMin(index_t, index_t);
	value_t rangeMax(index_t, index_t);
	void build(index_t, index_t, index_t, value_t*);
	void updateRange(index_t, index_t, value_t);
	void refreshDownToLeaf(index_t);
	void refresh();
};

/* DEFINITIONS */

/* TREE */

Tree::Tree(value_t array[], index_t length) {

	width = length;
	size = 4*width-1;

	nodes.reserve(size);
	for (index_t i = 0; i < size; i++) {
		nodes.push_back(Node(*this));
	}

	leafIndexes = new index_t[width];

	root().build(1, 1, width, array);
}

Node& Tree::node(index_t index) {
	return nodes[index-1];
}

Node& Tree::root() {
	return node(1);
}

Node& Tree::leaf(index_t number) {
	return node(leafIndexes[number-1]);
}

bool Tree::isPossibleToUpdateRange(index_t start, index_t end, value_t delta) {

	value_t rangeMin = root().rangeMin(start, end);
	value_t rangeMax = root().rangeMax(start, end);

	if (delta > 0) {
		return (rangeMax <= MAXIMUM - delta);
	} else {
		return (rangeMin >= MINIMUM - delta);
	}
}

void Tree::updateRange(index_t start, index_t end, value_t delta) {
	root().updateRange(start, end, delta);
}

void Tree::updateProfits(index_t start, index_t end, value_t delta) {

	root().refreshDownToLeaf(start);

	if (start != end) {
		root().refreshDownToLeaf(end);
	}

	Node& leftLeaf = leaf(start);
	Node& rightLeaf = leaf(end);

	if (start > 1) {
		root().refreshDownToLeaf(start-1);
		Node& previousLeaf = leaf(start-1);

		value_t previousLeafValue = previousLeaf.value();
		value_t leftLeafValue = leftLeaf.value();
		value_t oldLeftLeafValue = leftLeafValue - delta;

		if (previousLeafValue < oldLeftLeafValue) {
			profits -= previousLeafValue >= leftLeafValue;
		}

		if (previousLeafValue >= oldLeftLeafValue) {
			profits += previousLeafValue < leftLeafValue;
		}
	}

	if (end < width) {
		root().refreshDownToLeaf(end+1);
		Node& nextLeaf = leaf(end+1);

		value_t nextLeafValue = nextLeaf.value();
		value_t rightLeafValue = rightLeaf.value();
		value_t oldRightLeafValue = rightLeafValue - delta;

		if (oldRightLeafValue < nextLeafValue) {
			profits -= rightLeafValue >= nextLeafValue;
		}

		if (oldRightLeafValue >= nextLeafValue) {
			profits += rightLeafValue < nextLeafValue;
		}
	}
}

/* NODE */

bool Node::isLeaf() {
	return left == right;
}

value_t Node::value() {
	return min;
}

Node& Node::parent() {
	return tree.node(index/2);
}

Node& Node::leftChild() {
	return tree.node(2*index);
}

Node& Node::rightChild() {
	return tree.node(2*index+1);
}

void Node::refresh() {
	if (addAll != 0) {
		min += addAll;
		max += addAll;

		if (!isLeaf()) {
			leftChild().addAll += addAll;
			rightChild().addAll += addAll;
		}

		addAll = 0;
	}
}

void Node::refreshDownToLeaf(index_t number) {
	refresh();

	if (!isLeaf()) {
		index_t middle = (left + right) / 2;

		if (number <= middle) {
			leftChild().refreshDownToLeaf(number);
		} else {
			rightChild().refreshDownToLeaf(number);
		}
	}
}

void Node::build(index_t index, index_t left, index_t right, value_t array[]) {
	this->index = index;
	this->left = left;
	this->right = right;

	if (isLeaf()) {
		min = array[left-1];
		max = array[left-1];

		tree.leafIndexes[left-1] = index;
	} else {
		index_t middle = (left + right) / 2;

		leftChild().build(2*index, left, middle, array);
		rightChild().build(2*index+1, middle+1, right, array);

		min = std::min(leftChild().min, rightChild().min);
		max = std::max(leftChild().max, rightChild().max);
	}
}

value_t Node::rangeMin(index_t start, index_t end) {
	refresh();

	if (start <= left && right <= end) {
		return min;
	} else if (start <= right && left <= end) {
		value_t leftRangeMin = leftChild().rangeMin(start, end);
		value_t rightRangeMin = rightChild().rangeMin(start, end);

		return std::min(leftRangeMin, rightRangeMin);
	} else {
		return MAXIMUM;
	}
}

value_t Node::rangeMax(index_t start, index_t end) {
	refresh();

	if (start <= left && right <= end) {
		return max;
	} else if (start <= right && left <= end) {
		value_t leftRangeMax = leftChild().rangeMax(start, end);
		value_t rightRangeMax = rightChild().rangeMax(start, end);

		return std::max(leftRangeMax, rightRangeMax);
	} else {
		return MINIMUM;
	}
}

void Node::updateRange(index_t start, index_t end, value_t delta) {
	refresh();

	if (start <= left && right <= end) {
		min += delta;
		max += delta;

		if (!isLeaf()) {
			leftChild().addAll += delta;
			rightChild().addAll += delta;
		}
	} else if (start <= right && left <= end) {
		leftChild().updateRange(start, end, delta);
		rightChild().updateRange(start, end, delta);

		min = std::min(leftChild().min, rightChild().min);
		max = std::max(leftChild().max, rightChild().max);
	}
}

/* MAIN */

int main() {
	std::ios::sync_with_stdio(false);

	index_t n;
	std::cin >> n;

	value_t* array = new value_t[n];
	for (index_t i = 0; i < n; i++) {
		std::cin >> array[i];
	}

	for (index_t i = 1; i < n; i++) {
		profits += (array[i-1] < array[i]);
	}

	Tree tree = Tree(array, n);
	delete[] array;

	index_t m;
	std::cin >> m;

	for (index_t i = 0; i < m; i++) {
		index_t l, r;
		value_t delta;

		std::cin >> l >> r >> delta;

		if (tree.isPossibleToUpdateRange(l, r, delta)) {
			tree.updateRange(l, r, delta);
			tree.updateProfits(l, r, delta);

			std::cout << profits << '\n';
		} else {
			std::cout << "-1" << '\n';
		}
	}
}