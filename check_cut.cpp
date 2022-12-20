#include "graph.hpp"

int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    Graph graph = Graph(n);

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
    bool is_correct = true;
    for (auto from : terminals) {
      for (auto to : terminals) {
        if (from == to) {
          continue;
        }
        if (graph.CheckIfReachable(from, to)) {
          is_correct = false;
          break;
        }
      }
    }
    std::cout << is_correct;
    return 0;
}