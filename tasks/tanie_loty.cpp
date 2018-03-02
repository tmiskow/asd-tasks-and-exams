#include <iostream>
#include <queue>
#include <limits>
#include <cstddef>
#include <tuple>

/* TYPES */
using index_t = std::size_t;
using value_t = unsigned long long;

using DijkstraTuple = std::tuple<value_t, index_t, index_t>;
using DijkstraQueue = std::priority_queue<DijkstraTuple, std::vector<DijkstraTuple>, std::greater<DijkstraTuple>>;

/* CLASSES DECLARATIONS */
class City;
class Connection;

class City {
private:
    value_t cost = std::numeric_limits<value_t>::max();
    bool visited = false;

    const index_t index;
    const index_t layer;
    std::vector<Connection> connections;

public:
    explicit City(index_t index, index_t layer) : index(index), layer(layer) {};
    void addConnection(City& destination, value_t cost);
    index_t getIndex();
    index_t getLayer();
    value_t getCost();
    bool isVisited();
    void markVisited();
    void setCost(value_t cost);
    std::vector<Connection>& getConnections();
};

class Connection {
private:
    City& destination;
    value_t cost;

public:
    Connection(City &destination, value_t cost) : destination(destination), cost(cost) {}

    City& getDestination();
    value_t getCost();
};

/* CLASSES IMPLEMENTATION */
void City::addConnection(City& destination, value_t cost) {
    connections.emplace_back(destination, cost);
}

index_t City::getIndex() {
    return index;
}

index_t City::getLayer() {
    return layer;
}

value_t City::getCost() {
    return cost;
}

bool City::isVisited() {
    return visited;
}

void City::markVisited() {
    visited  = true;
}

void City::setCost(value_t cost) {
    this->cost = cost;
}

std::vector<Connection>& City::getConnections() {
    return connections;
}

City& Connection::getDestination() {
    return destination;
}

value_t Connection::getCost() {
    return cost;
}

/* GLOBALS */
std::vector<City> cities;
std::size_t numberOfCities, numberOfConnections, numberOfCoupons, numberOfLayers;

/* AUXILIARY FUNCTIONS */
template <class Type>
Type scan() {
    Type temp;
    std::cin >> temp;
    return temp;
}

template <class T>
void println(T t) {
    std::cout << t << "\n";
}

City& getCity(index_t index, index_t layer) {
    return cities[index + layer * numberOfCities];
}

/* DIJKSTRA */
bool dijkstra(value_t& answer) {
    DijkstraQueue dijkstraQueue;

    // push source city into a queue
    dijkstraQueue.emplace(0, 0, 0);

    while (!dijkstraQueue.empty()) {
        auto currentCost = std::get<0>(dijkstraQueue.top());
        auto currentIndex = std::get<1>(dijkstraQueue.top());
        auto currentLayer = std::get<2>(dijkstraQueue.top());
        dijkstraQueue.pop();

        auto& currentCity = getCity(currentIndex, currentLayer);

        // ignore already visited cities
        if (!currentCity.isVisited()) {
            currentCity.markVisited();
            currentCity.setCost(currentCost);

            for (auto& connection : currentCity.getConnections()) {
                auto& destinationCity = connection.getDestination();
                auto destinationCost = connection.getCost();

                auto destinationIndex = destinationCity.getIndex();
                auto destinationLayer = destinationCity.getLayer();
                auto totalCost = currentCost + destinationCost;

                if (!destinationCity.isVisited()) {
                    dijkstraQueue.emplace(totalCost, destinationIndex, destinationLayer);
                }
            }
        }
    }

    // check if final city was visited
    bool result = false;
    for (index_t layer = 0; layer < numberOfLayers; layer++) {
        if(getCity(numberOfCities-1, layer).isVisited()) {
            result = true;
        }
    }

    // get least expensive path across layers
    if (result) {
        value_t minCost = std::numeric_limits<value_t>::max();
        for (index_t i = 0; i < numberOfLayers; i++) {
            minCost = std::min(minCost, getCity(numberOfCities-1, i).getCost());
        }

        answer = minCost;
    }

    return result;
}

/* MAIN */
int main() {
    std::ios::sync_with_stdio(false);

    numberOfCities = scan<index_t>();
    numberOfConnections = scan<index_t>();
    numberOfCoupons = scan<index_t>();
    numberOfLayers = numberOfCoupons + 1;

    // add cities
    cities.reserve(numberOfCities * numberOfLayers);
    for (index_t layer = 0; layer < numberOfLayers; layer++) {
        for (index_t index = 0; index < numberOfCities; index++) {
            cities.emplace_back(index, layer);
        }
    }

    //add connections
    for (index_t i = 0; i < numberOfConnections; i++) {
        auto sourceIndex = scan<index_t>();
        auto destinationIndex = scan<index_t>();

        auto discount = scan<value_t>();
        auto cost = scan<value_t>();

        for (index_t layer = 0; layer < numberOfLayers; layer++) {
            // don't add connections to itself
            if (destinationIndex != sourceIndex) {
                auto& sourceCity = getCity(sourceIndex, layer);

                auto& sameLayerDestinationCity = getCity(destinationIndex, layer);
                sourceCity.addConnection(sameLayerDestinationCity, cost);

                if (layer + 1 < numberOfLayers) {
                    auto& nextLayerDestinationCity = getCity(destinationIndex, layer + 1);
                    sourceCity.addConnection(nextLayerDestinationCity, cost - discount);
                }
            }
        }
    }

    value_t answer;
    bool result = dijkstra(answer);

    if (result) {
        println(answer);
    } else {
        println(-1);
    }

    return 0;
}