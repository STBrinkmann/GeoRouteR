#include "graph.h"
#include <algorithm>
#include <stdexcept>
#include <map>
#include <unordered_map>

// Constructor
Graph::Graph(const std::vector<std::string>& edge_from,
             const std::vector<std::string>& edge_to,
             const std::vector<double>& edge_speed,
             const std::vector<double>& edge_length,
             const std::vector<std::string>& edge_oneway,
             const std::vector<std::string>& node_name,
             const std::vector<double>& node_x,
             const std::vector<double>& node_y,
             const std::string& crs) 
  : crs_(crs){
  // Set profile to default
  active_profile_ = "default";
  
  // Create a temporary node dictionary from node_name (name, id, valid)
  std::unordered_map<std::string, std::pair<int, bool>> tmp_node_dict;
  for (size_t i = 0; i < node_name.size(); ++i) {
    tmp_node_dict[node_name[i]] = {static_cast<int>(i), false};
  }
  
  // Fill in edges vector
  size_t m = edge_from.size();
  edges_.reserve(m);
  original_edges_.reserve(m);
  for(size_t i = 0; i < m; ++i) {
    auto from_it = tmp_node_dict.find(edge_from[i]);
    auto to_it = tmp_node_dict.find(edge_to[i]);
    if (from_it == tmp_node_dict.end() || to_it == tmp_node_dict.end()) {
      throw std::runtime_error("All nodes must be part of edges.");
    }
    
    int from = from_it->second.first;
    int to = to_it->second.first;
    double speed = edge_speed[i];
    double length = edge_length[i];
    double cost = (length / 1000.0) / (speed / 3600.0) / 60;
    std::string oneway = edge_oneway[i];
    
    Edge edge = {from, to, cost, speed, length, oneway};
    edges_.emplace_back(edge);
    original_edges_.emplace_back(edge);
    
    // Mark nodes as part of edges
    from_it->second.second = true;
    to_it->second.second = true;
  }
  
  // Build the final node dictionary and fill in nodes vector
  nodes_.reserve(node_name.size());
  original_nodes_.reserve(node_name.size());
  for (const auto& entry : tmp_node_dict) {
    if (entry.second.second) {
      int nodeId = entry.second.first;
      original_node_dict_[nodeId] = entry.first;
      node_dict_[nodeId] = entry.first;
      Node node = {nodeId, node_x[nodeId], node_y[nodeId]};
      nodes_.emplace_back(node);
      original_nodes_.emplace_back(node);
    } else {
      throw std::runtime_error("All nodes must be part of edges.");
    }
  }
  
  // Sort nodes and original_nodes vectors by id
  auto sortById = [](const Node& a, const Node& b) { return a.id < b.id; };
  std::sort(nodes_.begin(), nodes_.end(), sortById);
  std::sort(original_nodes_.begin(), original_nodes_.end(), sortById);
}


// Getters
const std::vector<Graph::Edge>& Graph::edges() const {
  return edges_;
}

const std::vector<Graph::Node>& Graph::nodes() const {
  return nodes_;
}

const std::map<int, std::string>& Graph::node_dict() const {
  return node_dict_;
}

std::string Graph::crs() const {
  return crs_;
}

std::string Graph::active_profile() const {
  return active_profile_;
}


// Methods
void Graph::activate_routing_profile(int profile) {
  reset_edges(); // Reset edges to the original data
  reset_nodes(); // Reset nodes to the original data
  reset_node_dict();  // Reset node_dict to the original data

  if(profile == ROUTING_PROFILE_DEFAULT) {
    active_profile_ = "default";
  } else {
    std::vector<Edge> updated_edges;
    std::vector<Node> updated_nodes;
    std::map<std::string, int> updated_node_dict;
    std::vector<bool> updated_node_ids(nodes_.size(), false);

    for (Edge& edge : edges_) {
      bool add_edge = true;
      bool add_reverse_edge = false;

      switch (profile) {
      case ROUTING_PROFILE_FOOT:
        active_profile_ = "foot";
        edge.oneway = (edge.speed > 90) ? "N" : "B";
        edge.speed = 5;
        break;
      case ROUTING_PROFILE_BICYCLE:
        active_profile_ = "bicycle";
        edge.oneway = (edge.speed > 90) ? "N" : edge.oneway;
        edge.speed = (edge.oneway == "foot_only") ? 4 : 15; // default 15 km/h; 4 km/h when walking is required
        break;
      case ROUTING_PROFILE_CAR:
        active_profile_ = "car";
        edge.oneway = (edge.oneway == "foot_only") ? "N" : edge.oneway;
        break;
      default:
        throw std::runtime_error("Invalid routing profile.");
      }

      // Update the cost based on the new speed
      edge.cost = (edge.length / 1000.0) / (edge.speed / 3600.0) / 60;

      // Adjust for the oneway parameter
      if (edge.oneway == "TF") {
        std::swap(edge.from, edge.to);
      } else if (edge.oneway == "B") {
        add_reverse_edge = true;
      } else if (edge.oneway == "N") {
        add_edge = false;
      }

      if (add_edge) {
        updated_edges.push_back(edge);
        updated_node_ids[edge.from] = true;
        updated_node_ids[edge.to] = true;

        if (add_reverse_edge) {
          Edge reverse_edge = edge;
          std::swap(reverse_edge.from, reverse_edge.to);
          updated_edges.push_back(reverse_edge);
        }
      }
    }

    // Reassign node ids and update node_dict
    int new_id = 0;
    for (const auto& pair : original_node_dict_) {
      int old_id = pair.first;
      const std::string& key = pair.second;
      if (updated_node_ids[old_id]) {
        Node& old_node = nodes_[old_id];
        Node new_node = old_node;
        new_node.id = new_id;
        updated_nodes.push_back(new_node);
        updated_node_dict[key] = new_id;
        new_id++;
      }
    }

    // Update edge node ids
    for (Edge& edge : updated_edges) {
      std::string from_key = original_node_dict_[edge.from];
      std::string to_key = original_node_dict_[edge.to];

      edge.from = updated_node_dict[from_key];
      edge.to = updated_node_dict[to_key];
    }

    // Assign the updated data to the class members
    edges_ = std::move(updated_edges);
    nodes_ = std::move(updated_nodes);

    // Create a new node_dict_ from updated_node_dict
    node_dict_.clear();
    for (const auto& pair : updated_node_dict) {
      node_dict_[pair.second] = pair.first;
    }
  }
}


// Helper methods
void Graph::reset_edges() {
  edges_ = original_edges_;
}

void Graph::reset_nodes() {
  nodes_ = original_nodes_;
}

void Graph::reset_node_dict() {
  node_dict_ = original_node_dict_;
}