#include <iostream>
#include <vector>
#include <queue>

struct Project {
    unsigned index;
    unsigned cost;
    unsigned numberOfDependers = 0;

    std::vector<unsigned> dependees;
    explicit Project(unsigned index, unsigned cost) : index(index), cost(cost) {}
};

Project& getProject(std::vector<Project>& projects, unsigned index) {
    return projects[index-1];
}

int main() {
    std::vector<Project> projects;

    unsigned n, m ,k;
    std::cin >> n >> m >> k;
    projects.reserve(n);

    for (unsigned i = 1; i <= n; i++) {
        unsigned  cost;
        std::cin >> cost;
        projects.emplace_back(i, cost);
    }

    for (int i = 0; i < m; i++) {
        unsigned a, b;
        std::cin >> a >> b;

        getProject(projects, a).numberOfDependers++;
        getProject(projects, b).dependees.push_back(a);
    }

    std::priority_queue<std::pair<unsigned, unsigned>, std::vector<std::pair<unsigned, unsigned>>, std::greater<std::pair<unsigned, unsigned>>> priorityQueue;

    for (auto& project : projects) {
        if (project.numberOfDependers == 0) {
            priorityQueue.emplace(project.cost, project.index);
        }
    }


    std::pair<unsigned, unsigned> maxPair{0, 0};
    for (unsigned i = 0; i < k; i++) {
        auto pair = priorityQueue.top();
        priorityQueue.pop();

        maxPair = std::max(maxPair, pair);

        auto& project = getProject(projects, pair.second);
        for (auto dependeeIndex : project.dependees) {
            auto& dependee = getProject(projects, dependeeIndex);
            dependee.numberOfDependers--;

            if (dependee.numberOfDependers == 0) {
                priorityQueue.emplace(dependee.cost, dependee.index);
            }
        }

        project.dependees.clear();
    }

    std::cout << maxPair.first;

    return 0;
}