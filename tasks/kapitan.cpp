#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <queue>
#include <unordered_set>

struct Island {
    std::size_t index;
    int x;
    int y;

    Island(size_t index, int x, int y) : index(index), x(x), y(y) {}

    static bool compareX(const Island& island1, const Island& island2) {
//        assert(island1.index != island2.index);

        if (island1.x != island2.x) {
            return island1.x < island2.x;
        } else if (island1.y != island2.y) {
            return compareY(island1, island2);
        } else {
            return island1.index < island2.index;
        }
    }

    static bool compareY(const Island& island1, const Island& island2) {
//        assert(island1.index != island2.index);

        if (island1.y != island2.y) {
            return island1.y < island2.y;
        } else if (island1.x != island2.x) {
            return compareX(island1, island2);
        } else {
            return island1.index < island2.index;
        }
    }
};

int main() {
    std::ios::sync_with_stdio(false);

    int n;
    std::cin >> n;

    std::vector<Island> islands;
    islands.reserve(static_cast<std::size_t>(n));

    for (std::size_t i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;

        islands.emplace_back(i, x, y);
    }

    std::vector<Island> xSortedIslands(islands);
    std::sort(xSortedIslands.begin(), xSortedIslands.end(), Island::compareX);

    std::vector<Island> ySortedIslands(islands);
    std::sort(ySortedIslands.begin(), ySortedIslands.end(), Island::compareY);

//    std::cout << "\nX-SORTED ISLANDS:\n";
//    for (auto& island : xSortedIslands) {
//        std::cout << island.index << ": " << island.x << ", " << island.y << "\n";
//    }
//
//    std::cout << "\nY-SORTED ISLANDS:\n";
//    for (auto& island : ySortedIslands) {
//        std::cout << island.index << ": " << island.x << ", " << island.y << "\n";
//    }

    using DistancePair = std::pair<int, std::size_t>;

    std::priority_queue<DistancePair, std::vector<DistancePair>, std::greater<DistancePair>> dijkstraQueue;
    dijkstraQueue.emplace(0, 0);

    std::unordered_set<std::size_t> calculatedIslandsIndexes;

    int bestDistance = 0;
    bool continueFlag = true;

    while (continueFlag) {
        auto pair = dijkstraQueue.top();
        dijkstraQueue.pop();

        auto currentDistance = pair.first;
        auto currentIndex = pair.second;

        if (currentIndex == n-1) {
            bestDistance = currentDistance;
            continueFlag = false;
        } else if (calculatedIslandsIndexes.count(currentIndex) == 0) {
            auto& currentIsland = islands[currentIndex];
            calculatedIslandsIndexes.insert(currentIndex);

            std::vector<DistancePair> neighbours;
            neighbours.reserve(4);

            auto xIterator = std::lower_bound(xSortedIslands.begin(), xSortedIslands.end(), currentIsland, Island::compareX);

            if (xIterator != xSortedIslands.begin()) {
                auto& leftNeighbour = *(xIterator - 1);
                neighbours.emplace_back(currentIsland.x - leftNeighbour.x + currentDistance, leftNeighbour.index);
            }

            if (xIterator != xSortedIslands.end()) {
                auto& rightNeighbour = *(xIterator + 1);
                neighbours.emplace_back(rightNeighbour.x - currentIsland.x + currentDistance, rightNeighbour.index);
            }

            auto yIterator = std::lower_bound(ySortedIslands.begin(), ySortedIslands.end(), currentIsland, Island::compareY);

            if (yIterator != ySortedIslands.begin()) {
                auto& bottomNeighbour = *(yIterator - 1);
                neighbours.emplace_back(currentIsland.y - bottomNeighbour.y + currentDistance, bottomNeighbour.index);
            }

            if (yIterator != ySortedIslands.end()) {
                auto& topNeighbour = *(yIterator + 1);
                neighbours.emplace_back(topNeighbour.y - currentIsland.y + currentDistance, topNeighbour.index);
            }

            for (auto neighbourPair : neighbours) {
                dijkstraQueue.push(neighbourPair);
            }
        }
    }

    std::cout << bestDistance << "\n";

    return 0;
}