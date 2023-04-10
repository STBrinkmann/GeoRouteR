#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <string>

class Graph {
public:
  // Constructor
  Graph(const std::vector<std::string>& edge_from,
        const std::vector<std::string>& edge_to,
        const std::vector<double>& edge_speed,
        const std::vector<double>& edge_length,
        const std::vector<std::string>& edge_oneway,
        const std::vector<std::string>& node_name,
        const std::vector<double>& node_x,
        const std::vector<double>& node_y,
        const std::string& crs);
  
  struct Edge {
    int from;
    int to;
    double cost;
    double speed;
    double length;
    std::string oneway;
  };
  
  struct Node {
    int id;
    double x;
    double y;
  };
  
  // Routing profiles
  static constexpr int ROUTING_PROFILE_DEFAULT = 0;
  static constexpr int ROUTING_PROFILE_FOOT = 1;
  static constexpr int ROUTING_PROFILE_BICYCLE = 2;
  static constexpr int ROUTING_PROFILE_CAR = 3;
  
  // Getters
  const std::vector<Edge>& edges() const;
  const std::vector<Node>& nodes() const;
  const std::unordered_map<std::string, int>& node_dict() const;
  std::string crs() const;
  std::string active_profile() const;
  
  // Methods
  void activate_routing_profile(int profile);

private:
  // Member variables
  std::vector<Edge> edges_;
  std::vector<Edge> original_edges_;
  std::vector<Node> nodes_;
  std::vector<Node> original_nodes_;
  std::unordered_map<std::string, int> node_dict_;
  std::string crs_;
  std::string active_profile_;
  
  // Helper methods
  void reset_edges();
  void reset_nodes();
  void findNodeById();
};

#endif //GRAPH_H
