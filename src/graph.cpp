#include "graph.h"
#include <algorithm>
#include <stdexcept>
#include <map>

// Constructor
Graph::Graph(const std::vector<std::string>& edge_from,
             const std::vector<std::string>& edge_to,
             const std::vector<double>& edge_cost,
             const std::vector<double>& edge_dist,
             const std::vector<std::string>& node_name,
             const std::vector<double>& node_x,
             const std::vector<double>& node_y,
             const std::string& crs) 
  : crs_(crs){
  
  // Create a temporary node dictionary from node_name
  std::unordered_map<std::string, std::pair<int, bool>> tmp_node_dict;
  for (size_t i = 0; i < node_name.size(); ++i) {
    tmp_node_dict[node_name[i]] = {static_cast<int>(i), false};
  }
  
  // Fill in edges vector
  size_t m = edge_from.size();
  edges_.reserve(m);
  for (size_t i = 0; i < m; ++i) {
    auto from_it = tmp_node_dict.find(edge_from[i]);
    auto to_it = tmp_node_dict.find(edge_to[i]);
    
    if (from_it == tmp_node_dict.end() || to_it == tmp_node_dict.end()) {
      throw std::runtime_error("All nodes must be part of edges.");
    }
    
    int from = from_it->second.first;
    int to = to_it->second.first;
    double cost = edge_cost[i];
    double dist = edge_dist.size() > 0 ? edge_dist[i] : 0.0;
    Edge edge = {from, to, cost, dist};
    edges_.emplace_back(edge);
    
    // Mark nodes as part of edges
    from_it->second.second = true;
    to_it->second.second = true;
  }
  
  // Build the final node dictionary and fill in nodes vector
  nodes_.reserve(node_name.size());
  for (const auto& entry : tmp_node_dict) {
    if (entry.second.second) {
      node_dict_[entry.first] = entry.second.first;
      Node node = {entry.second.first, node_x[entry.second.first], node_y[entry.second.first]};
      nodes_.emplace_back(node);
    } else {
      throw std::runtime_error("All nodes must be part of edges.");
    }
  }
}


// Getters
const std::vector<Graph::Edge>& Graph::edges() const {
  return edges_;
}

const std::vector<Graph::Node>& Graph::nodes() const {
  return nodes_;
}

const std::unordered_map<std::string, int>& Graph::node_dict() const {
  return node_dict_;
}

std::string Graph::crs() const {
  return crs_;
}