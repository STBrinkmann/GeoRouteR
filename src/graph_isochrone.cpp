#include "graph_isochrone.h"
#include <queue>
#include <limits>
#include <tuple>
#include <map>
#include <set>
#include <Rcpp.h>

#ifdef _OPENMP
#include <omp.h>
#endif

// [[Rcpp::plugins(openmp)]]

GraphIsochrone::GraphIsochrone(const std::vector<int>& from, const std::vector<int>& to, const std::vector<double>& cost) {
  int maxNode = -1;
  for (int node : from) {
    if (node > maxNode) {
      maxNode = node;
    }
  }
  for (int node : to) {
    if (node > maxNode) {
      maxNode = node;
    }
  }

  adjacencyList.resize(maxNode + 1);

  for (size_t i = 0; i < from.size(); i++) {
    adjacencyList[from[i]].push_back({to[i], cost[i]});
  }
}

std::vector<std::tuple<int, int, double>> GraphIsochrone::calculateIsochrone(const std::vector<int>& start_nodes, double lim, int num_cores) const {
  std::vector<std::tuple<int, int, double>> result;
  std::set<std::pair<int, int>> visitedEdges;

  // Set the number of threads to be used by OpenMP
  omp_set_num_threads(num_cores);

  // Use OpenMP parallel for directive to parallelize the outer loop
#pragma omp parallel for shared(result, visitedEdges)
  for (int i = 0; i < start_nodes.size(); i++) {
    int start = start_nodes[i];
    std::vector<double> costs(adjacencyList.size(), std::numeric_limits<double>::max());
    costs[start] = 0.0;

    using NodeCostPair = std::pair<double, int>;
    std::priority_queue<NodeCostPair, std::vector<NodeCostPair>, std::greater<NodeCostPair>> pq;
    pq.push({0.0, start});

    while (!pq.empty()) {
      double currentCost = pq.top().first;
      int currentNode = pq.top().second;
      pq.pop();

      if (currentCost > lim) {
        continue;
      }

      for (const Edge& edge : adjacencyList[currentNode]) {
        double newCost = currentCost + edge.cost;
        if (newCost < costs[edge.to]) {
          costs[edge.to] = newCost;

          // Use OpenMP critical section to avoid race conditions when modifying shared data
#pragma omp critical
          {
            if (visitedEdges.find({start, edge.to}) == visitedEdges.end()) {
              visitedEdges.insert({start, edge.to});

              if (start != edge.to && newCost <= lim) {
                result.push_back(std::make_tuple(start, edge.to, newCost));
              }
            }
          }
          pq.push({newCost, edge.to});
        }
      }
    }
  }

  return result;
}

