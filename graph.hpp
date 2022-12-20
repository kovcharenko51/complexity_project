#pragma once

#include <iostream>
#include <queue>
#include <unordered_map>
#include <map>
#include <set>
#include <vector>

enum Color {
    WHITE,
    GREY,
    BLACK
};

struct FlowElement {
    int vertex;
    int64_t flow;
};

struct Edge {
  int from;
  int to;

  Edge() = default;
  Edge(int from, int to) : from(std::min(from, to)), to(std::max(from, to)) {
  }
  bool operator<(const Edge& other) const {
    if (from < other.from) {
      return true;
    }
    return to < other.to;
  }
};

class Graph {
public:
    explicit Graph(int);

    void AddUnorientedEdge(int, int, int64_t);
    void RemoveEdge(int, int);
    void Dfs(int);
    bool CheckIfReachable(int, int);

    void RememeberGraph();
    void ResetGraph();

    int64_t FindFlow(int, int, std::vector<int>&);
    int64_t GetMaxFlow(int, int);

    std::vector<Edge> GetMinIsolatingCut(int);
    std::pair<int64_t, std::set<Edge>> GetMultiwayCut(std::vector<int>&);

private:
    const int64_t k_max_flow_ = INT64_MAX;
    std::vector<std::unordered_map<int, int64_t>> adj_list_;
    std::vector<std::unordered_map<int, int64_t>> init_adj_list_;
    std::vector<Color> colors_;
};