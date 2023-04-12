#include "isochrone.h"
#include <set>
#include <queue>
#include <limits>
#include <functional>
#include <string>
#include <sstream>
#include <algorithm>
#include <Rcpp.h>

// [[Rcpp::depends(RcppParallel)]]
#include <RcppParallel.h>

// RcppParallel worker
class IsochroneWorker : public RcppParallel::Worker {
public:
  IsochroneWorker(const Graph& graph,
                  const std::vector<int>& start_nodes,
                  const std::vector<double>& lim,
                  std::vector<std::vector<std::tuple<int, int, double, double>>>& results)
    : graph_(graph), start_nodes_(start_nodes), lim_(lim), results_(results) {}
  
  // Process start nodes in parallel
  void operator()(std::size_t begin, std::size_t end) {
    for (std::size_t i = begin; i < end; ++i) {
      //NOT Rcpp::checkUserInterrupt();
      results_[i] = _calculateIsochrone(graph_, {start_nodes_[i]}, lim_);
    }
  }
  
private:
  const Graph& graph_;
  const std::vector<int>& start_nodes_;
  const std::vector<double>& lim_;
  std::vector<std::vector<std::tuple<int, int, double, double>>>& results_;
};


// RcppParallel methods
std::vector<std::vector<std::tuple<int, int, double, double>>> parallelCalculateIsochrone(
    const Graph& graph, const std::vector<int>& start_nodes, const std::vector<double>& lim) {
  
  std::size_t num_start_nodes = start_nodes.size();
  std::vector<std::vector<std::tuple<int, int, double, double>>> results(num_start_nodes);
  
  IsochroneWorker worker(graph, start_nodes, lim, results);
  RcppParallel::parallelFor(0, num_start_nodes, worker);
  
  return results;
}


// Helper function
double assign_thresholds(const double& cost, const std::vector<double>& lim) {
  double assigned_limit_value = std::numeric_limits<double>::max();
  
  for (const auto& threshold : lim) {
    if (cost <= threshold && threshold < assigned_limit_value) {
      assigned_limit_value = threshold;
    }
  }
  
  return assigned_limit_value;
}

// Internal isochrone methods
std::vector<std::tuple<int, int, double, double>> _calculateIsochrone(const Graph& graph, const std::vector<int>& start_nodes, const std::vector<double>& lim) {
  
  std::vector<std::tuple<int, int, double, double>> result;
  std::set<std::pair<int, int>> visitedEdges;
  
  const std::vector<Graph::Edge>& edges = graph.edges();
  const std::vector<Graph::Node>& nodes = graph.nodes();
  int node_count = static_cast<int>(nodes.size());
  double max_lim = *std::max_element(lim.begin(), lim.end());
  
  // Create an adjacency list from the edges
  std::vector<std::vector<Graph::Edge>> adjacencyList(node_count);
  for (const Graph::Edge& edge : edges) {
    adjacencyList[edge.from].push_back(edge);
  }
  
  for (auto start : start_nodes) {
    result.push_back(std::make_tuple(start, start, 0.0, *std::min_element(lim.begin(), lim.end())));
    
    std::vector<double> costs(node_count, std::numeric_limits<double>::max());
    costs[start] = 0.0;
    
    using NodeCostPair = std::pair<double, int>;
    std::priority_queue<NodeCostPair, std::vector<NodeCostPair>, std::greater<NodeCostPair>> pq;
    pq.push({0.0, start});
    
    while (!pq.empty()) {
      double currentCost = pq.top().first;
      int currentNode = pq.top().second;
      pq.pop();
      
      if (currentCost > max_lim) {
        continue;
      }
      
      for (const Graph::Edge& edge : adjacencyList[currentNode]) {
        double newCost = currentCost + edge.cost;
        if (newCost < costs[edge.to]) {
          costs[edge.to] = newCost;
          
          if (visitedEdges.find({start, edge.to}) == visitedEdges.end()) {
            visitedEdges.insert({start, edge.to});
            
            if (start != edge.to && newCost <= max_lim) {
              double threshold = assign_thresholds(newCost, lim);
              result.push_back(std::make_tuple(start, edge.to, newCost, threshold));
            }
          }
          
          pq.push({newCost, edge.to});
        }
      }
    }
  }
  
  return result;
}