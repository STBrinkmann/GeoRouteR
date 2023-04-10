#include <Rcpp.h>
#include "graph.h"
#include "isochrone.h"

using namespace Rcpp;

// Declare Rcpp pointer class
RCPP_EXPOSED_CLASS_NODECL(Graph)
  
// Graph class constructor wrapper
// [[Rcpp::export]]
RcppExport SEXP graph_create(SEXP edge_from, SEXP edge_to, SEXP edge_speed, SEXP edge_length, SEXP edge_oneway, SEXP node_name, SEXP node_x, SEXP node_y, SEXP crs) {
    BEGIN_RCPP
    CharacterVector edge_from_str(edge_from), edge_to_str(edge_to), node_name_str(node_name), edge_oneway_str(edge_oneway);
    NumericVector edge_speed_num(edge_speed), edge_length_num(edge_length), node_x_num(node_x), node_y_num(node_y);
    std::string crs_str = as<std::string>(crs);
    
    std::vector<std::string> edge_from_std(edge_from_str.begin(), edge_from_str.end());
    std::vector<std::string> edge_to_std(edge_to_str.begin(), edge_to_str.end());
    std::vector<double> edge_speed_std(edge_speed_num.begin(), edge_speed_num.end());
    std::vector<double> edge_length_std(edge_length_num.begin(), edge_length_num.end());
    std::vector<std::string> node_name_std(node_name_str.begin(), node_name_str.end());
    std::vector<std::string> edge_oneway_std(edge_oneway_str.begin(), edge_oneway_str.end());
    std::vector<double> node_x_std(node_x_num.begin(), node_x_num.end());
    std::vector<double> node_y_std(node_y_num.begin(), node_y_num.end());
    
    XPtr<Graph> ptr(new Graph(edge_from_std, edge_to_std, edge_speed_std, edge_length_std, edge_oneway_std, node_name_std, node_x_std, node_y_std, crs_str));
    return ptr;
    END_RCPP
  }

// Getters
// [[Rcpp::export]]
RcppExport SEXP graph_edges(SEXP p) {
  BEGIN_RCPP
  XPtr<Graph> ptr(p);
  const auto& edges = ptr->edges();
  
  size_t n = edges.size();
  IntegerVector from(n);
  IntegerVector to(n);
  NumericVector cost(n);
  NumericVector speed(n);
  NumericVector length(n);
  CharacterVector oneway(n);
  
  for (size_t i = 0; i < n; ++i) {
    from[i] = edges[i].from;
    to[i] = edges[i].to;
    cost[i] = edges[i].cost;
    speed[i] = edges[i].speed;
    length[i] = edges[i].length;
    oneway[i] = edges[i].oneway;
  }
  
  return DataFrame::create(_["from"] = from,
                           _["to"] = to,
                           _["cost"] = cost,
                           _["speed"] = speed,
                           _["length"] = length,
                           _["oneway"] = oneway);
  END_RCPP
}

// [[Rcpp::export]]
RcppExport SEXP graph_nodes(SEXP p) {
  BEGIN_RCPP
  XPtr<Graph> ptr(p);
  const auto& nodes = ptr->nodes();
  
  size_t n = nodes.size();
  IntegerVector id(n);
  NumericVector x(n);
  NumericVector y(n);
  
  for (size_t i = 0; i < n; ++i) {
    id[i] = nodes[i].id;
    x[i] = nodes[i].x;
    y[i] = nodes[i].y;
  }
  
  return DataFrame::create(_["id"] = id,
                           _["X"] = x,
                           _["Y"] = y);
  END_RCPP
}

// [[Rcpp::export]]
RcppExport SEXP graph_node_dict(SEXP p) {
  BEGIN_RCPP
  XPtr<Graph> ptr(p);
  const auto& node_dict = ptr->node_dict();
  
  size_t n = node_dict.size();
  CharacterVector key(n);
  IntegerVector value(n);
  
  size_t i = 0;
  for (const auto& kv : node_dict) {
    key[i] = kv.first;
    value[i] = kv.second;
    i++;
  }
  
  return DataFrame::create(_["node"] = key,
                           _["id"] = value);
  END_RCPP
}

// [[Rcpp::export]]
RcppExport SEXP graph_crs(SEXP p) {
  BEGIN_RCPP
  XPtr<Graph> ptr(p);
  return wrap(ptr->crs());
  END_RCPP
}

// [[Rcpp::export]]
RcppExport SEXP graph_profile(SEXP p) {
  BEGIN_RCPP
  XPtr<Graph> ptr(p);
  return wrap(ptr->active_profile());
  END_RCPP
}

// Methods
// [[Rcpp::export]]
void graph_activate_routing_profile(SEXP p, SEXP profile) {
  BEGIN_RCPP
  XPtr<Graph> ptr(p);
  int routing_profile = as<int>(profile);
  ptr->activate_routing_profile(routing_profile);
  VOID_END_RCPP
}

RCPP_MODULE(graph_module) {
  using namespace Rcpp;
  // Getters
  function("graph_create", &graph_create);
  function("graph_edges", &graph_edges);
  function("graph_nodes", &graph_nodes);
  function("graph_node_dict", &graph_node_dict);
  function("graph_crs", &graph_crs);
  function("graph_profile", &graph_profile);
  //Methods
  function("graph_activate_routing_profile", &graph_activate_routing_profile);
}

// Methods
// [[Rcpp::export]]
RcppExport SEXP calculate_isochrone(SEXP graph_ptr, SEXP start_nodes_sexp, SEXP lim_sexp) {
  BEGIN_RCPP
  XPtr<Graph> graph(graph_ptr);
  std::vector<int> start_nodes = Rcpp::as<std::vector<int>>(start_nodes_sexp);
  std::vector<double> lim = Rcpp::as<std::vector<double>>(lim_sexp);
  
  auto all_isochrones = parallelCalculateIsochrone(*graph, start_nodes, lim);
  
  size_t total_size = 0;
  for (const auto& isochrones : all_isochrones) {
    total_size += isochrones.size();
  }
  
  IntegerVector start(total_size);
  IntegerVector end(total_size);
  NumericVector cost(total_size);
  NumericVector threshold(total_size);
  
  size_t index = 0;
  for (const auto& isochrones : all_isochrones) {
    for (const auto& iso : isochrones) {
      start[index] = std::get<0>(iso);
      end[index] = std::get<1>(iso);
      cost[index] = std::get<2>(iso);
      threshold[index] = std::get<3>(iso);
      ++index;
    }
  }
  
  return DataFrame::create(_["start"] = start,
                           _["end"] = end,
                           _["cost"] = cost,
                           _["threshold"] = threshold);
  END_RCPP
}