#include <iostream>
#include <algorithm>
#include <cassert>

typedef unsigned index_t;
typedef long long value_t;

const value_t MODULO_MODIFIER = 1000000000;

struct Node;
index_t n, k, m;
index_t size;
Node* nodes;
index_t* sequence;
value_t* inversions;
index_t* leafIndexes;

Node& node(index_t);

value_t moduloAddition(value_t v1, value_t v2) {
	return ((v1%MODULO_MODIFIER) + (v2%MODULO_MODIFIER) % MODULO_MODIFIER);
}

struct Node {
	index_t index;
	index_t left, right;
	value_t value;

	Node() {
		value = -1;
	}

	bool isLeaf() {
		return left == right;
	}

	Node& parent() {
		return node(index/2);
	}

	Node& leftChild() {
		return node(2*index);
	}

	Node& rightChild() {
		return node(2*index + 1);
	}

	void build(index_t i, index_t start, index_t end) {
		index = i;
		left = start;
		right = end;
		value = 0;

		if (isLeaf()) {
			leafIndexes[start] = index;
		} else {
			index_t middle = (left+right) / 2;
			leftChild().build(2*index, start, middle);
			rightChild().build(2*index+1, middle + 1, end);
		}
	}

	void refresh() {
		if (!isLeaf()) {
			value = moduloAddition(leftChild().value, rightChild().value);
		}

		if (index != 1) {
			parent().refresh();
		}
	}

	void insert(value_t v) {
		assert(isLeaf());
		value = moduloAddition(0, v);
		parent().refresh();
	}

	value_t rangeSum(index_t start, index_t end) {
		if (start > right || end < left) {
			return 0;
		} else if (start == left && end == right) {
			return value;
		} else {
			index_t middle = (left+right)/2;

			if (end <= middle) {
				return leftChild().rangeSum(start, end);
			} else if (start > middle) {
				return rightChild().rangeSum(start, end);
			} else {
				return moduloAddition(leftChild().rangeSum(start, middle), rightChild().rangeSum(middle+1, end));
			}
		}
	}
};

Node& node(index_t index) {
	return nodes[index-1];
}

Node& leaf(index_t start) {
	return node(leafIndexes[start]);
}

Node& root() {
	return node(1);
}

void insert(index_t leafIndex, value_t value) {
	leaf(leafIndex).insert(value);
}


void buildTree() {
	nodes = new Node[size];
	root().build(1, 1, n);
}

value_t rangeSum(index_t start, index_t end) {
	return root().rangeSum(start, end);
}

void printLeafs() {

    std::cout << "\n";

    for (int i = 1; i <= n; i++) {
        std::cout << leaf(i).value << " ";
    }

    std::cout << "\n";
}

void printInversions() {

    std::cout << "\n";

    for (int i = 1; i <= n; i++) {
        std::cout << inversions[i] << " ";
    }

    std::cout << "\n";
}

void printTree() {
    std::cout << "\n";

    index_t powerOf2 = 2;

    for (int i = 1; i <= size; i++) {
        std::cout << node(i).value << ' ';

        if (i == powerOf2 - 1) {
            std::cout << "\n";
            powerOf2 *= 2;
        }
    }
}

int main() {
	std::ios::sync_with_stdio(false);

    std::cin >> n;
    std::cin >> k;

	sequence = new index_t[n+1];
	for (index_t i = 1; i <= n; i++) {
		std::cin >> sequence[i];
	}

	inversions = new value_t[n+1];
	leafIndexes = new index_t[n+1];

	size = 4*n - 1;
	buildTree();

	// printTree();
	// printLeafs();
	// printInversions();

	// preprocessing
	for (index_t i = 1; i <= n; i++) {
		index_t index = sequence[i];
		value_t sum = rangeSum(index+1, n);
		inversions[index] = sum;
		insert(index, 1);
	}

	// printTree();
	// printLeafs();
	// printInversions();

	for (index_t j = 2; j < k; j++) {
		for (index_t i = 1; i <= n; i++) {
			insert(i, inversions[i]);
		}

		// printTree();
		// printLeafs();
		// printInversions();

		for (index_t i = n; i >= 1; i--) {
			index_t index = sequence[i];
			value_t sum = rangeSum(index+1, n);
			inversions[index] = sum;
			insert(index, 0);
		}

		// printTree();
		// printLeafs();
		// printInversions();
	}

	value_t sum = 0;

	for (index_t i = 1; i <= n; i++) {
		sum = moduloAddition(sum, inversions[i]);
	}

	std::cout << sum << "\n";
}