#ifndef DISTMAT_H
#define DISTMAT_H

#include "graph.h"
#include <vector>
#include <tuple>
#include <string>

// RcppParallel methods
std::vector<std::vector<std::tuple<int, int, double>>> parallelCalculateDistMat(
    const Graph& graph, const std::vector<int>& start_nodes, const std::vector<int>& end_nodes, const std::string& mode);

// Internal methods
std::vector<std::vector<std::tuple<int, int, double>>> _dist_mat(const Graph& graph, const std::vector<int>& start_nodes, const std::vector<int>& end_nodes, const std::string& mode);

#endif // DISTMAT_H
