#' Graph Class
#'
#' This class represents a graph with nodes, edges, and costs.
#'
#' @slot data A data.frame containing the graph edges and costs.
#' @slot coords A data.frame containing the coordinates for each node (optional).
#' @slot dict A data.frame mapping the node reference to its integer ID.
#'
#' @name Graph
#' @export
#' @importFrom methods setClass
#' @keywords classes
#' @docType class
#' @aliases Graph-class

setClass(
  "Graph",
  slots = list(
    data = "data.frame",
    coords = "data.frame",
    dict = "data.frame",
    directed = "logical",
    crs = "character"
  ),
  prototype = list(
    directed = TRUE,
    crs = NA_character_
  )
)


