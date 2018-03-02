#include <cstdio>
#include <algorithm>

int nextOddNumberIndex(int startIndex, int* array, int length);
int nextEvenNumberIndex(int startIndex, int* array, int length);

int main() {

	int productsAmount;
	scanf("%d", &productsAmount);

	int* products = new int[productsAmount];

	for (int i = productsAmount - 1; i >= 0; i--) {
		scanf("%d", &products[i]);
	}

	long long* sums = new long long[productsAmount];
	int* lowestOddNumbers = new int[productsAmount];
	int* nextOddNumberIndexes = new int[productsAmount];
	int* lowestEvenNumbers = new int[productsAmount];
	int* nextEvenNumberIndexes = new int[productsAmount];

	sums[0] = products[0];

	if (products[0] % 2 == 0) {
		lowestEvenNumbers[0] = products[0];
		nextEvenNumberIndexes[0] = nextEvenNumberIndex(0, products, productsAmount);

		lowestOddNumbers[0] = 0;
		nextOddNumberIndexes[0] = nextOddNumberIndex(-1, products, productsAmount);

	} else {
		lowestOddNumbers[0] = products[0];
		nextOddNumberIndexes[0] = nextOddNumberIndex(0, products, productsAmount);

		lowestEvenNumbers[0] = 0;
		nextEvenNumberIndexes[0] = nextEvenNumberIndex(-1, products, productsAmount);
	}


	for (int i = 1; i < productsAmount; i++) {

		sums[i] = sums[i-1] + products[i];

		 if (products[i] % 2 == 0) {
			 lowestEvenNumbers[i] = products[i];
			 nextEvenNumberIndexes[i] = nextEvenNumberIndex(i, products, productsAmount);

			 lowestOddNumbers[i] = lowestOddNumbers[i-1];
			 nextOddNumberIndexes[i] = nextOddNumberIndexes[i-1];

		 }
		 else {
			 lowestOddNumbers[i] = products[i];
			 nextOddNumberIndexes[i] = nextOddNumberIndex(i, products, productsAmount);

			 lowestEvenNumbers[i] = lowestEvenNumbers[i-1];
			 nextEvenNumberIndexes[i] = nextEvenNumberIndexes[i-1];
		 }
	}

	int daysAmount;
	scanf("%d", &daysAmount);

	for (int i = 0; i < daysAmount; i++) {
		int productsToBuy;
		scanf("%d", &productsToBuy);

		int k = productsToBuy - 1;

		long long answer;

		if (sums[k] % 2 == 1) {
			answer = sums[k];
		}
		else {

			long long firstCandidate;

			if (nextEvenNumberIndexes[k] != -1 && lowestOddNumbers[k]) {
				firstCandidate = sums[k] - lowestOddNumbers[k] + products[nextEvenNumberIndexes[k]];
			}
			else {
				firstCandidate = -1;
			}

			long long secondCandidate;

			if (nextOddNumberIndexes[k] != -1 && lowestEvenNumbers[k]) {
				secondCandidate = sums[k] - lowestEvenNumbers[k] + products[nextOddNumberIndexes[k]];
			}
			else {
				secondCandidate = -1;
			}

			answer = std::max(firstCandidate, secondCandidate);
		}

		printf("%lld\n", answer);
	}

	// clearing memory
	delete[] products;
	delete[] sums;
	delete[] lowestEvenNumbers;
	delete[] lowestOddNumbers;
	delete[] nextEvenNumberIndexes;
	delete[] nextOddNumberIndexes;

	return 0;
}

int nextOddNumberIndex(int startIndex, int* array, int length) {

	int i = startIndex + 1;

	while (i < length && array[i] % 2 == 0) {
		i++;
	}

	if (i == length) {
		return -1;
	}
	else {
		return i;
	}
}

int nextEvenNumberIndex(int startIndex, int* array, int length) {

	int i = startIndex + 1;

	while (i < length && array[i] % 2 == 1) {
		i++;
	}

	if (i == length) {
		return -1;
	}
	else {
		return i;
	}
}