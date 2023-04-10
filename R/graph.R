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
#' @examples
#' \dontrun{
#' edge_from <- c("A", "A", "B", "C")
#' edge_to <- c("B", "C", "C", "D")
#' edge_speed <- c(10, 20, 40, 100)
#' edge_length <- c(1, 2, 2, 1)
#' edge_oneway <- c("FT", "B", "N, "TF")
#' node_name <- c("A", "B", "C", "D")
#' node_x <- c(0, 1, 1, 2)
#' node_y <- c(0, 0, 1, 1)
#' crs <- "EPSG:4326"
#'
#' graph <- Graph$new(edge_from, edge_to, edge_speed, edge_length, edge_oneway, node_name, node_x, node_y, crs)
#' graph$edges())
#' graph$nodes())
#' graph$node_dict())
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
                       #' @param edge_speed numeric vector of edge speeds.
                       #' @param edge_length numeric vector of edge lengths.
                       #' @param edge_oneway character vector of edge oneway types (one-way: from-to = "FT", one-way: to-from = "TF", two-way = "B", or restricted = "N").
                       #' @param node_name character vector of node names.
                       #' @param node_x numeric vector of node x-coordinates.
                       #' @param node_y numeric vector of node y-coordinates.
                       #' @param crs character string of the CRS (coordinate reference system).
                       initialize = function(edge_from, edge_to, edge_speed, edge_length, edge_oneway, node_name, node_x, node_y, crs) {
                         self$pointer <- graph_create(edge_from, edge_to, edge_speed, edge_length, edge_oneway, node_name, node_x, node_y, crs)
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
                         dict <- graph_node_dict(self$pointer)
                         nodes <- graph_nodes(self$pointer)
                         return(dict[dict$value %in% nodes$id,])
                       },
                       
                       #' Get CRS
                       #'
                       #' Returns the CRS (coordinate reference system) string of the graph.
                       #'
                       #' @return A character string of the CRS.
                       crs = function() {
                         graph_crs(self$pointer)
                       },
                       
                       #' Get CRS
                       #'
                       #' Returns the active routing profile string of the graph.
                       #'
                       #' @return A character string of the CRS.
                       profile = function() {
                         graph_profile(self$pointer)
                       },
                       
                       #' Activate Routing Profile
                       #'
                       #' Activates a routing profile for the graph.
                       #' @param profile A character string specifying the routing profile ("default" = 0, "foot" = 1, "bicycle" = 2, "car" = 3).
                       activate_profile = function(profile = "default") {
                         checkmate::assert_choice(profile, c("default", "foot", "bicycle", "car"))
                         
                         mapping <- c("default" = 0, "foot" = 1, "bicycle" = 2, "car" = 3)
                         graph_activate_routing_profile(self$pointer, as.integer(mapping[profile]))
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
                         cat("Active profile:", self$profile(), "\n")
                       }
                     )
)