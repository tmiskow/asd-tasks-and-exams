#include <cstdio>
#include <algorithm>

bool isLetter(char c) {
	return c >= 'A' && c <= 'Z';
}

int main() {

	int length = 0;
	int distance = 1000000;
	int distanceCandidate = -1;
	char startingLetter;
	bool differentLetterTypes = false;

	char c = getchar();

	while (c == '*' || isLetter(c)) {
		if (isLetter(c)) {

			if (distanceCandidate == -1) {
				distanceCandidate = 0;
				startingLetter = c;
			}
			else if (startingLetter == c) {
				distanceCandidate = 0;
			}
			else {
				distance = std::min(distance, distanceCandidate);
				distanceCandidate = 0;
				startingLetter = c;

				differentLetterTypes = true;
			}
		}
		else {
			if (distanceCandidate != -1) {
				distanceCandidate++;
			}
		}

		length++;
		c = getchar();
	}

	int answer;

	if (!differentLetterTypes) {
		answer = 1;
	}
	else {
		answer = length - distance;
	}
	printf("%d\n", answer);
}