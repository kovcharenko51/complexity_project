#include "graph.hpp"

Graph::Graph(int n) {
    adj_list_.resize(n);
    colors_.resize(n, WHITE);
}

void Graph::AddUnorientedEdge(int from, int to, int64_t capacity) {
    adj_list_[from][to] = capacity;
    adj_list_[to][from] = capacity;
}

void Graph::RemoveEdge(int from, int to) {
  adj_list_[from].erase(adj_list_[from].find(to));
  adj_list_[to].erase(adj_list_[to].find(from));
}

void Graph::RememeberGraph() {
  init_adj_list_ = adj_list_;
}

void Graph::ResetGraph() {
  adj_list_ = init_adj_list_;
}

int64_t Graph::FindFlow(int start, int finish, std::vector<int>& parents) {
    parents.assign(adj_list_.size(), -1);
    std::queue<FlowElement> flow_queue;
    flow_queue.push({start, k_max_flow_});
    parents[start] = -2;
    while (!flow_queue.empty()) {
        auto flow_elem = flow_queue.front();
        flow_queue.pop();
        for (auto next : adj_list_[flow_elem.vertex]) {
            if ((parents[next.first] == -1) && (next.second != 0)) {
                parents[next.first] = flow_elem.vertex;
                int64_t new_flow = std::min(flow_elem.flow, next.second);
                if (next.first == finish) {
                    return new_flow;
                }
                flow_queue.push({next.first, new_flow});
            }
        }
    }
    return 0;
}

int64_t Graph::GetMaxFlow(int start, int finish) {
    int64_t flow = 0;
    std::vector<int> parents(adj_list_.size(), -1);
    flow = FindFlow(start, finish, parents);
    int64_t final_flow = 0;
    while (flow > 0) {
        final_flow += flow;
        int top = finish;
        while (top != start) {
            int prev = parents[top];
            adj_list_[prev][top] -= flow;
            adj_list_[top][prev] += flow;
            top = prev;
        }
        flow = FindFlow(start, finish, parents);
    }
    return final_flow;
}

std::vector<Edge> Graph::GetMinIsolatingCut(int vertex) {
  int n = adj_list_.size() - 1;
  for (int i = 0; i < n; i++) {
    if (i == vertex) {
      continue;
    }
    AddUnorientedEdge(i, n, k_max_flow_);
  }
  GetMaxFlow(vertex, n);
  std::vector<Edge> flow_edges;
  for (int i = 0; i < n; i++) {
    for (auto edge : adj_list_[i]) {
      if (edge.second < init_adj_list_[i][edge.first]) {
        flow_edges.push_back(Edge{i, edge.first});
      }
    }
  }
  ResetGraph();
  return flow_edges;
}

std::pair<int64_t, std::set<Edge>> Graph::GetMultiwayCut(std::vector<int>& terminals) {
  std::map<int64_t, std::vector<Edge>> cuts;
  for (auto terminal : terminals) {
    auto edges = GetMinIsolatingCut(terminal);
    int64_t weight = 0;
    for (auto& edge : edges) {
      weight += init_adj_list_[edge.from][edge.to];
    }
    cuts[weight] = edges;
  }
  std::set<Edge> result;
  int64_t res_weight = 0;
  for (int i = 0; i < terminals.size() - 1; i++) {
    auto& cut = *cuts.begin();
    for (auto& edge : cut.second) {
      result.insert(edge);
      res_weight += init_adj_list_[edge.from][edge.to];
    }
    cuts.erase(cuts.begin());
  }
  return std::make_pair(res_weight, result);
}

void Graph::Dfs(int vertex) {
    if (colors_[vertex] != WHITE) {
        return;
    }
    colors_[vertex] = GREY;
    for (auto& neighbour : adj_list_[vertex]) {
        if (colors_[neighbour.first] == WHITE) {
            Dfs(neighbour.first);
        }
    }
    colors_[vertex] = BLACK;
}

bool Graph::CheckIfReachable(int from, int to) {
  std::fill(colors_.begin(), colors_.end(), WHITE);
  Dfs(from);
  return colors_[to] == BLACK;
}