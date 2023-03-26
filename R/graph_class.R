#' Graph Class
#'
#' This class represents a graph with nodes, edges, and costs.
#'
#' @slot data A data.frame containing the graph edges and costs.
#' @slot coords A data.frame containing the coordinates for each node (optional).
#' @slot dict A data.frame mapping the node reference to its integer ID.
#' @slot directed A logical value indicating whether the graph is directed (default: TRUE).
#' @slot crs A character string representing the coordinate reference system (CRS) of the graph (default: NA_character_).
#'
#' @name Graph
#' @export
#' @importFrom methods setClass
#' @keywords classes
#' @docType class
#' @aliases Graph-class
#' 
#' @title Graph Class
#' @description This class represents a graph with nodes, edges, and costs.
#' 
#' @section Slots:
#' \describe{
#'   \item{\code{data}:}{A data.frame containing the graph edges and costs.}
#'   \item{\code{coords}:}{A data.frame containing the coordinates for each node (optional).}
#'   \item{\code{dict}:}{A data.frame mapping the node reference to its integer ID.}
#'   \item{\code{directed}:}{A logical value indicating whether the graph is directed (default: TRUE).}
#'   \item{\code{crs}:}{A character string representing the coordinate reference system (CRS) of the graph (default: NA_character_).}
#' }
#' 
#' @examples
#' \dontrun{
#' # Create a new Graph object
#' g <- new("Graph", data = my_data, coords = my_coords, dict = my_dict)
#' 
#' # Access the slots of the Graph object
#' g@data
#' g@coords
#' g@dict
#' g@directed
#' g@crs
#' }
#' 
#' @seealso \code{\link{initialize}}
#' @keywords classes
#' @export
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
