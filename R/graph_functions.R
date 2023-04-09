#' Create a Graph object
#'
#' This function takes edge and node data.frames and a CRS string to create a Graph object.
#'
#' @param edges data.frame with columns "from", "to", "cost", and optionally "dist".
#' @param nodes data.frame with columns "node", "X", and "Y".
#' @param crs character string representing the coordinate reference system.
#' @param directed logical value indicating whether the graph is directed (default is TRUE).
#'
#' @return A Graph object.
#' @export
#'
#' @examples
#' \dontrun{
#' edges <- data.frame(from = c("A", "A", "B", "C"),
#'                     to = c("B", "C", "C", "D"),
#'                     cost = c(1, 2, 3, 4),
#'                     dist = c(1, 2, 2, 1))
#'
#' nodes <- data.frame(node = c("A", "B", "C", "D"),
#'                     X = c(0, 1, 1, 2),
#'                     Y = c(0, 0, 1, 1))
#'
#' crs <- "EPSG:4326"
#'
#' graph <- makegraph(edges, nodes, crs, directed = TRUE)
#' print(graph)
#' }
#' 
#' @importFrom checkmate assert_data_frame
#' @importFrom checkmate assert_string
#' @importFrom checkmate assert_logical
#' @importFrom methods new
makegraph <- function(edges, nodes, crs, directed = TRUE) {
  # Input validation tests using checkmate
  checkmate::assert_data_frame(edges, min.cols = 3, max.cols = 4)
  checkmate::assert_data_frame(nodes, min.cols = 3)
  checkmate::assert_string(crs)
  checkmate::assert_logical(directed, len = 1)
  
  # Check if the edges data.frame has 3 or 4 columns
  if (ncol(edges) == 3) {
    edges$dist <- 0
  }
  
  # Check if column names of edges and nodes data.frames are as expected
  checkmate::assert_named(edges, .var.name = c("from", "to", "cost", "dist"))
  checkmate::assert_named(nodes, .var.name = c("node", "X", "Y"))
  
  if (any(is.na(edges))) stop("NAs are not allowed in the graph")
  if (any(edges$cost < 0)) stop("Negative costs are not allowed")
  if (any(!unique(c(edges$from, edges$to)) %in% unique(nodes$node))) {
    stop("Some nodes from edges are not represented in the nodes data.table")
  }
  nodes <- nodes[nodes$node %in% unique(c(edges$from, edges$to)),]
  
  # Add reverse edges if the graph is undirected
  if (!directed) {
    edges2 <- edges[, c("to", "from", "cost", "dist")]
    colnames(edges2) <- colnames(edges)
    edges <- rbind(edges, edges2)
  }
  
  # Extract the required columns from the input data.frames
  edge_from <- edges$from
  edge_to <- edges$to
  edge_cost <- edges$cost
  edge_dist <- edges$dist
  
  node_name <- nodes$node
  node_x <- nodes$X
  node_y <- nodes$Y
  
  # Initialize a new Graph object
  graph <- Graph$new(edge_from = edge_from, 
                     edge_to = edge_to, 
                     edge_cost = edge_cost, 
                     edge_dist = edge_dist, 
                     node_name = node_name, 
                     node_x = node_x, 
                     node_y = node_y, 
                     crs = crs)
  return(graph)
}