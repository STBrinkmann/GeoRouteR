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
        const std::vector<double>& edge_cost,
        const std::vector<double>& edge_dist,
        const std::vector<std::string>& node_name,
        const std::vector<double>& node_x,
        const std::vector<double>& node_y,
        const std::string& crs);
  
  struct Edge {
    int from;
    int to;
    double cost;
    double dist;
  };
  
  struct Node {
    int id;
    double x;
    double y;
  };
  
  // Getters
  const std::vector<Edge>& edges() const;
  const std::vector<Node>& nodes() const;
  const std::unordered_map<std::string, int>& node_dict() const;
  std::string crs() const;
  
private:
  // Member variables
  std::vector<Edge> edges_;
  std::vector<Node> nodes_;
  std::unordered_map<std::string, int> node_dict_;
  std::string crs_;
};

#endif //GRAPH_H