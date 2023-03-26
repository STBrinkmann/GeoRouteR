#' @description Graph Constructor Method
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

