#include <Rcpp.h>
#include "graph_isochrone.h"

using namespace Rcpp;


// Helper function
std::string assign_thresholds(const double& cost, const CharacterVector& lim) {
  std::string assigned_limit = "";
  double assigned_limit_value = std::numeric_limits<double>::max();
  
  for (const auto& threshold_str : lim) {
    double threshold = std::stod(Rcpp::as<std::string>(threshold_str));
    if (cost <= threshold && threshold < assigned_limit_value) {
      assigned_limit = Rcpp::as<std::string>(threshold_str);
      assigned_limit_value = threshold;
    }
  }
  
  return assigned_limit;
}

// [[Rcpp::export]]
DataFrame calculateIsochroneRcpp(IntegerVector from, IntegerVector to, NumericVector cost, NumericVector start_nodes, CharacterVector lim) {
  
  // Convert Rcpp vectors to std::vectors
  std::vector<int> from_vec(from.begin(), from.end());
  std::vector<int> to_vec(to.begin(), to.end());
  std::vector<double> cost_vec(cost.begin(), cost.end());
  std::vector<int> start_nodes_vec(start_nodes.begin(), start_nodes.end());
  
  // Create the GraphIsochrone object
  GraphIsochrone graphIsochrone(from_vec, to_vec, cost_vec);
  
  // Call the calculateIsochrone function for the maximum lim
  double lim_max = std::numeric_limits<double>::min();
  for (const auto& threshold_str : lim) {
    double threshold = std::stod(Rcpp::as<std::string>(threshold_str));
    if (threshold > lim_max) {
      lim_max = threshold;
    }
  }
  
  std::vector<std::tuple<int, int, double>> isochrone = graphIsochrone.calculateIsochrone(start_nodes_vec, lim_max);
  
  // Convert the result to Rcpp vectors
  NumericVector start_nodes_out(isochrone.size());
  NumericVector nodes(isochrone.size());
  NumericVector costs(isochrone.size());
  CharacterVector thresholds(isochrone.size());
  
  for (size_t i = 0; i < isochrone.size(); i++) {
    start_nodes_out[i] = std::get<0>(isochrone[i]);
    nodes[i] = std::get<1>(isochrone[i]);
    costs[i] = std::get<2>(isochrone[i]);
    thresholds[i] = assign_thresholds(costs[i], lim);
  }
  
  // Return the result as an Rcpp DataFrame
  return DataFrame::create(Named("start_node") = start_nodes_out, Named("node") = nodes, Named("cost") = costs, Named("threshold") = thresholds);
}
