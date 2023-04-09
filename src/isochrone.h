#ifndef ISOCHRONE_H
#define ISOCHRONE_H

#include "graph.h"
#include <vector>
#include <tuple>

// RcppParallel methods
std::vector<std::vector<std::tuple<int, int, double, double>>> parallelCalculateIsochrone(
    const Graph& graph, const std::vector<int>& start_nodes, const std::vector<double>& lim);

// Internal methods
std::vector<std::tuple<int, int, double, double>> _calculateIsochrone(const Graph& graph, const std::vector<int>& start_nodes, const std::vector<double>& lim);

#endif // ISOCHRONE_H
