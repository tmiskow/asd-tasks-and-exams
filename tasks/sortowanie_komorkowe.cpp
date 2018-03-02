#include <cstdio>

const long long MODULO_CONST = 1000000000;

int main() {

	int length;
	scanf("%d", &length);

	int* sequence = new int[length];
	for (int i = 0; i < length; i++) {
		scanf("%d", &sequence[i]);
	}

	if (length == 1) {
		printf("1\n");
	}
	else {
		long long* leftCache;
		long long* rightCache;

		long long* lastLeftCache = new long long[length-1];
		for (int i = 0; i < length - 1; i++) {
			lastLeftCache[i] = sequence[i] < sequence[i+1] ? 1 : 0;
		}

		long long* lastRightCache = new long long[length-1];
		for (int i = 0; i < length - 1; i++) {
			lastRightCache[i] = sequence[i+1] > sequence[i] ? 1 : 0;
		}

		for (int i = 2; i < length; i++) {
			leftCache = new long long[length-i];
			rightCache = new long long[length-i];

			for (int j = 0; j < length-i; j++) {

				leftCache[j] = 0;
				rightCache[j] = 0;

				if (sequence[j] < sequence[j+1]) {
					leftCache[j] = (leftCache[j] + lastLeftCache[j+1]) % MODULO_CONST;
				}

				if (sequence[j] < sequence[j+i]) {
					leftCache[j] = (leftCache[j] + lastRightCache[j+1]) % MODULO_CONST;
				}

				if (sequence[j+i] > sequence[j+i-1]) {
					rightCache[j] = (rightCache[j] + lastRightCache[j]) % MODULO_CONST;
				}

				if (sequence[j+i] > sequence[j]) {
					rightCache[j] = (rightCache[j] + lastLeftCache[j]) % MODULO_CONST;
				}
			}

			delete[] lastLeftCache;
			delete[] lastRightCache;
			lastLeftCache = leftCache;
			lastRightCache = rightCache;
		}

		printf("%lld\n", ((lastLeftCache[0] % MODULO_CONST) + (lastRightCache[0] % MODULO_CONST)) % MODULO_CONST);

		delete[] lastLeftCache;
		delete[] lastRightCache;
	}

	delete[] sequence;

	return 0;
}