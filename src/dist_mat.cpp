#include "dist_mat.h"
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>
#include <set>
#include <Rcpp.h>

// [[Rcpp::depends(RcppParallel)]]
#include <RcppParallel.h>

// RcppParallel worker
class DistMatWorker : public RcppParallel::Worker {
public:
  DistMatWorker(const Graph& graph,
                const std::vector<int>& start_nodes,
                const std::vector<int>& end_nodes,
                const std::string& mode,
                std::vector<std::vector<std::tuple<int, int, double>>>& results)
    : graph_(graph), start_nodes_(start_nodes), end_nodes_(end_nodes), mode_(mode), results_(results) {}
  
  // Process start nodes in parallel
  void operator()(std::size_t begin, std::size_t end) {
    for (std::size_t i = begin; i < end; ++i) {
      std::vector<std::tuple<int, int, double>> path = _dist_mat(graph_, {start_nodes_[i]}, end_nodes_, mode_)[0];
      results_[i] = path;
    }
  }
  
private:
  const Graph& graph_;
  const std::vector<int>& start_nodes_;
  const std::vector<int>& end_nodes_;
  const std::string& mode_;
  std::vector<std::vector<std::tuple<int, int, double>>>& results_;
};


// RcppParallel method
std::vector<std::vector<std::tuple<int, int, double>>> parallelCalculateDistMat(
    const Graph& graph, const std::vector<int>& start_nodes, const std::vector<int>& end_nodes, const std::string& mode) {
  
  std::vector<std::vector<std::tuple<int, int, double>>> results(start_nodes.size(), std::vector<std::tuple<int, int, double>>(end_nodes.size()));
  
  DistMatWorker worker(graph, start_nodes, end_nodes, mode, results);
  RcppParallel::parallelFor(0, start_nodes.size(), worker);
  
  return results;
}



// Internal dist_mat methods
std::vector<std::vector<std::tuple<int, int, double>>> _dist_mat(const Graph& graph, const std::vector<int>& start_nodes, const std::vector<int>& end_nodes, const std::string& mode) {
  std::vector<std::vector<std::tuple<int, int, double>>> result(start_nodes.size(), std::vector<std::tuple<int, int, double>>(end_nodes.size()));
  
  for (size_t i = 0; i < start_nodes.size(); ++i) {
    for (size_t j = 0; j < end_nodes.size(); ++j) {
      int start_node = start_nodes[i];
      int end_node = end_nodes[j];
      
      // Check if from and to are not equal
      if (start_node == end_node) {
        result[i][j] = std::make_tuple(start_node, end_node, 0.0);
        continue;
      }
      
      const std::vector<Graph::Edge>& edges = graph.edges();
      const std::vector<Graph::Node>& nodes = graph.nodes();
      int node_count = static_cast<int>(nodes.size());
      
      // Create an adjacency list from the edges
      std::vector<std::vector<Graph::Edge>> adjacencyList(node_count);
      for (const Graph::Edge& edge : edges) {
        adjacencyList[edge.from].push_back(edge);
      }
      
      // Initialize costs and heuristic values
      std::vector<double> costs(node_count, std::numeric_limits<double>::max());
      std::vector<double> heuristic_values(node_count, 0.0);
      
      // Calculate heuristic values
      for (int k = 0; k < node_count; ++k) {
        double dx = nodes[k].x - nodes[end_node].x;
        double dy = nodes[k].y - nodes[end_node].y;
        heuristic_values[k] = std::sqrt(dx * dx + dy * dy);
      }
      
      costs[start_node] = 0.0;
      
      using NodeCostPair = std::pair<double, int>;
      std::priority_queue<NodeCostPair, std::vector<NodeCostPair>, std::greater<NodeCostPair>> pq;
      pq.push({heuristic_values[start_node], start_node});
      
      std::vector<int> prev(node_count, -1);
      
      while (!pq.empty()) {
        int current_node = pq.top().second;
        pq.pop();
        
        if (current_node == end_node) {
          break;
        }
        
        for (const Graph::Edge& edge : adjacencyList[current_node]) {
          double edge_cost = mode == "time" ? edge.cost : edge.length;
          double new_cost = costs[current_node] + edge_cost;
          if (new_cost < costs[edge.to]) {
            costs[edge.to] = new_cost;
            prev[edge.to] = current_node;
            
            double f_cost = new_cost + heuristic_values[edge.to];
            pq.push({f_cost, edge.to});
          }
        }
      }
      
      std::tuple<int, int, double> path(-1, -1, std::numeric_limits<double>::max());
      double total_cost = std::numeric_limits<double>::max();
      if (prev[end_node] != -1) {
        int node = end_node;
        total_cost = 0.0;
        while (node != start_node) {
          int from = prev[node];
          int to = node;
          double cost = costs[node] - costs[from];
          total_cost += cost;
          node = from;
        }
        path = std::make_tuple(start_node, end_node, total_cost);
      }
      
      result[i][j] = path;
    }
  }
  
  return result;
}
