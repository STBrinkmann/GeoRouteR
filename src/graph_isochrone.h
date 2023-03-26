#ifndef GRAPH_ISOCHRONE_H
#define GRAPH_ISOCHRONE_H

#include <vector>
#include <utility>

class GraphIsochrone {
public:
  GraphIsochrone(const std::vector<int>& from, const std::vector<int>& to, const std::vector<double>& cost);

  std::vector<std::tuple<int, int, double>> calculateIsochrone(const std::vector<int>& start_nodes, double lim) const;

private:
  struct Edge {
    int to;
    double cost;
  };

  std::vector<std::vector<Edge>> adjacencyList;
};

#endif // GRAPH_ISOCHRONE_H
