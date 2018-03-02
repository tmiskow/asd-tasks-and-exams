#include <iostream>

long mark_true(bool array[], long index) {
    if (array[index]) {
        array[index] = false;
        return mark_true(array, index + 1) - 1;
    } else {
        array[index] = true;
        return 1;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    long length;
    std::cin >> length;

    bool array[2000000];
    long counter = 0;
    for (long i = 0; i < length; i++) {
        long index;
        std::cin >> index;
        counter += mark_true(array, index);
        std::cout << counter << '\n';
    }

    return 0;
}