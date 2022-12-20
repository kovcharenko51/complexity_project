#include "graph.hpp"

int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    Graph graph = Graph(n + 1);

    for (int i = 0; i < m; ++i) {
        int from = 0;
        int to = 0;
        int64_t capacity = 0;

        std::cin >> from >> to >> capacity;
        graph.AddUnorientedEdge(--from, --to, capacity);
    }
    int k = 0;
    std::cin >> k;    
    std::vector<int> terminals(k);
    for (int i = 0; i < k; i++) {
      int terminal = 0;
      std::cin >> terminal;
      terminals[i] = terminal - 1;
    }
    graph.RememeberGraph();

    auto result = graph.GetMultiwayCut(terminals);
    std::cout << "The multiway cut of weight " << result.first << " is:" << std::endl;
    for (auto& edge : result.second) {
      std::cout << edge.from + 1 << " " << edge.to + 1 << std::endl;
    }
    return 0;
}

