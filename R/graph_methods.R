#' Initialize a Graph object
#'
#' @description Initializes a new Graph object with the specified data, coordinates, dictionary, directionality, and coordinate reference system (CRS).
#'
#' @param .Object An object of class "Graph".
#' @param data A data frame or matrix representing the edges of the graph.
#' @param coords A data frame or matrix representing the coordinates of the nodes in the graph.
#' @param dict A named character vector mapping node names to IDs.
#' @param directed A logical value indicating whether the graph is directed (default: TRUE).
#' @param crs A character string representing the coordinate reference system (CRS) of the graph (default: NA_character_).
#' 
#' @return An object of class "Graph".
#' @rdname Graph
#' @importFrom methods setMethod
#'
setMethod("initialize", "Graph", function(.Object, data, coords, dict, directed = TRUE, crs = NA_character_) {
  .Object@data <- data
  .Object@coords <- coords
  .Object@dict <- dict
  .Object@directed <- directed
  .Object@crs <- crs
  return(.Object)
})

# Print method
setMethod("show", "Graph", function(object) {
  cat("Graph object\n")
  cat("Number of nodes: ", nrow(object@dict), "\n")
  cat("Number of edges: ", nrow(object@data), "\n")
  cat("Directed: ", ifelse(object@directed, "TRUE", "FALSE"), "\n")
  if (nrow(object@coords) > 0) {
    cat("Coordinates available\n")
  } else {
    cat("No coordinates available\n")
  }
  cat("CRS: ", object@crs, "\n")
})

