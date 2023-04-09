#' @useDynLib GeoRouteR, .registration = TRUE
NULL

#' Graph Class
#'
#' This class represents a graph with nodes and edges.
#'
#' @section Usage:
#' \preformatted{
#' graph <- Graph$new(edge_from, edge_to, edge_cost, edge_dist, node_name, node_x, node_y, crs)
#' }
#'
#' @section Methods:
#' \describe{
#'   \item{edges()}{Returns a list of edges in the graph.}
#'   \item{nodes()}{Returns a list of nodes in the graph.}
#'   \item{node_dict()}{Returns a named list of node indices in the graph.}
#'   \item{crs()}{Returns the CRS string of the graph.}
#' }
#'
#' @examples
#' \dontrun{
#' edge_from <- c("A", "A", "B", "C")
#' edge_to <- c("B", "C", "C", "D")
#' edge_cost <- c(1, 2, 3, 4)
#' edge_dist <- c(1, 2, 2, 1)
#' node_name <- c("A", "B", "C", "D")
#' node_x <- c(0, 1, 1, 2)
#' node_y <- c(0, 0, 1, 1)
#' crs <- "EPSG:4326"
#'
#' graph <- Graph$new(edge_from, edge_to, edge_cost, edge_dist, node_name, node_x, node_y, crs)
#' print(graph$edges())
#' print(graph$nodes())
#' print(graph$node_dict())
#' print(graph$crs())
#' }
#' @importFrom R6 R6Class
#' @importFrom Rcpp sourceCpp
#' @importFrom Rcpp evalCpp
#' @export
Graph <- R6::R6Class("Graph",
                     public = list(
                       #' @field pointer Pointer to the Graph object in C++.
                       pointer = NULL,
                       
                       #' Initialize Graph Object
                       #'
                       #' Creates a new Graph object.
                       #'
                       #' @param edge_from character vector of edge sources.
                       #' @param edge_to character vector of edge targets.
                       #' @param edge_cost numeric vector of edge costs.
                       #' @param edge_dist numeric vector of edge distances.
                       #' @param node_name character vector of node names.
                       #' @param node_x numeric vector of node x-coordinates.
                       #' @param node_y numeric vector of node y-coordinates.
                       #' @param crs character string of the CRS (coordinate reference system).
                       initialize = function(edge_from, edge_to, edge_cost, edge_dist, node_name, node_x, node_y, crs) {
                         self$pointer <- graph_create(edge_from, edge_to, edge_cost, edge_dist, node_name, node_x, node_y, crs)
                       },
                       
                       #' Get Edges
                       #'
                       #' Returns a list of edges in the graph.
                       #'
                       #' @return A list of edges with columns "from", "to", "cost", and "dist".
                       edges = function() {
                         graph_edges(self$pointer)
                       },
                       
                       #' Get Nodes
                       #'
                       #' Returns a list of nodes in the graph.
                       #'
                       #' @return A list of nodes with columns "id", "x", and "y".
                       nodes = function() {
                         graph_nodes(self$pointer)
                       },
                       
                       #' Get Node Dictionary
                       #'
                       #' Returns a named list of node indices in the graph.
                       #'
                       #' @return A named list of node indices, with node names as the names and node indices as the values.
                       node_dict = function() {
                         graph_node_dict(self$pointer)
                       },
                       
                       #' Get CRS
                       #'
                       #' Returns the CRS (coordinate reference system) string of the graph.
                       #'
                       #' @return A character string of the CRS.
                       crs = function() {
                         graph_crs(self$pointer)
                       },
                       
                       #' Print Graph Summary
                       #'
                       #' Prints a summary of the graph object, including the number of nodes and edges.
                       #' @return Print
                       print = function() {
                         cat("Graph summary:\n")
                         cat("-----------------\n")
                         cat("Number of nodes:", nrow(self$nodes()), "\n")
                         cat("Number of edges:", nrow(self$edges()), "\n")
                         cat("CRS:", self$crs(), "\n")
                       }
                     )
)