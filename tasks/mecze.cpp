#include <cstdio>
#include <algorithm>
#include <bitset>

int main() {

	long long* playerMatchesArray;

	int matchesAmount, playersAmount;

	scanf("%d", &playersAmount);
	scanf("%d", &matchesAmount);

	playerMatchesArray = new long long[playersAmount];

	for (int i = 0; i < playersAmount; i++) {
		playerMatchesArray[i] = 0;
	}

	long long bitMatchIndex = 1;

	for (int i = 0; i < matchesAmount; i++) {
		for (int j = 0; j < playersAmount; j++) {
			int index;
			scanf("%d", &index);

			if (j < playersAmount/2) {
				playerMatchesArray[index-1] += bitMatchIndex;
			}
		}

		bitMatchIndex *= 2;
	}

	std::sort(playerMatchesArray, playerMatchesArray + playersAmount);

	int index = 0;
	while (index < playersAmount - 1 && playerMatchesArray[index] != playerMatchesArray[index+1]) {
		index++;
	}

	if (index == playersAmount - 1) {
		printf("TAK\n");
	} else {
		printf("NIE\n");
	}

	delete[] playerMatchesArray;

	return 0;
}