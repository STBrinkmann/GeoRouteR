#' Make a Graph
#'
#' @description Create a graph object from edge data.
#' @param from Vector of node IDs representing the start of an edge.
#' @param to Vector of node IDs representing the end of an edge.
#' @param cost Vector of edge costs.
#' @param directed Logical indicating whether the graph is directed or not.
#' @param coords An optional data.frame containing the node coordinates.
#' @param crs A character string specifying the coordinate reference system (CRS) of the coordinates
#' @return A Graph object.
#' @examples
#' \dontrun{
#' # Create a graph
#' from  <- c(1, 1, 1, 2, 2, 3, 3, 4, 5, 6, 6, 7)
#' to    <- c(2, 3, 4, 3, 5, 4, 6, 6, 7, 5, 7, 8)
#' cost  <- c(5.6, 3.8, 2.0, 1.2, 6.1, 4.2, 2.5, 7.8, 3.3, 1.7, 6.9, 2.2)
#' 
#' graph <- makegraph(from, to, cost)
#' 
#' coords <- data.frame(
#' id = graph@dict$ref,
#' lon = c(-73.987, -73.988, -73.986, -73.989, -73.984, -73.985, -73.990, -73.983),
#' lat = c(40.753, 40.754, 40.755, 40.756, 40.757, 40.752, 40.758, 40.751)
#' )
#' graph2 <- makegraph(from, to, cost, coords = coords, crs = "epsg:4326")
#' 
#' }
#' @export
#' @importFrom checkmate assert_multi_class
#' @importFrom checkmate assert_class
#' @importFrom checkmate assert_set_equal
#' @importFrom methods new

makegraph <- function(from, to, cost, directed = TRUE, coords = NULL, crs = NA_character_) {
  # Check input consistency
  if (!(length(from) == length(to) && length(from) == length(cost))) {
    stop("from, to, and cost must have the same length")
  }

  checkmate::assert_multi_class(from, c("integer", "numeric", "character"))
  checkmate::assert_multi_class(to, c("integer", "numeric", "character"))
  checkmate::assert_multi_class(cost, c("integer", "numeric"))

  # Create graph data.frame
  df <- data.frame(
    from = as.integer(from),
    to = as.integer(to),
    cost = as.numeric(cost)
  )

  if (any(is.na(df))) stop("NAs are not allowed in the graph")
  if (any(df$cost < 0)) stop("Negative costs are not allowed")

  # Add reverse edges if the graph is undirected
  if (!directed) {
    df2 <- df[, c(2, 1, 3)]
    colnames(df2) <- colnames(df)
    df <- rbind(df, df2)
  }

  # Validate and preprocess coordinates (if provided)
  if (!is.null(coords)) {
    if (ncol(coords) != 3) stop("coords should have 3 columns")

    checkmate::assert_multi_class(coords[[1]], c("integer", "numeric", "character"))
    checkmate::assert_multi_class(coords[[2]], c("integer", "numeric"))
    checkmate::assert_multi_class(coords[[3]], c("integer", "numeric"))

    coords[[1]] <- as.character(coords[[1]])
    coords[[2]] <- as.numeric(coords[[2]])
    coords[[3]] <- as.numeric(coords[[3]])
    colnames(coords) <- c("id", "X", "Y")

    if (any(is.na(coords))) stop("NAs are not allowed in coordinates")
    if (sum(duplicated(coords[[1]])) > 0) stop("node_ID should be unique in the coordinates data frame")

    nodes <- unique(c(df$from, df$to))
    if (sum(nodes %in% coords[[1]]) < length(nodes)) stop("Some nodes are missing in coordinates data")

    coords <- coords[coords[[1]] %in% nodes,]
  } else {
    coords <- data.frame(NULL)
  }

  # Create a dictionary to map node references to unique IDs
  nodes <- unique(c(df$from, df$to))
  dict <- data.frame(ref = nodes,
                     id = 0:(length(nodes) - 1),
                     stringsAsFactors = FALSE)

  # Replace node references with unique IDs in the graph data.frame
  df$from <- dict$id[match(df$from, dict$ref)]
  df$to <- dict$id[match(df$to, dict$ref)]
  dict$ref <- as.character(dict$ref)

  if (nrow(coords) > 0) {
    coords <- coords[match(nodes, coords[[1]]),]
  }

  # Return the graph object as a list
  return(methods::new("Graph", data = df, coords = coords, dict = dict, directed = directed, crs = crs))
}


#' Calculate isochrone using Dijkstra's algorithm
#'
#' @description This function calculates the isochrone for a set of starting nodes in a directed
#' graph using Dijkstra's algorithm. The isochrone is defined as the set of nodes that can be
#' reached from the starting nodes within a certain cost limit.
#' @param Graph A Graph object.
#' @param from A vector of node IDs representing the starting node(s).
#' @param lim A numeric value or vector of values representing the maximum cost(s) of the isochrone.
#' @return a data frame with three columns: "start_node" (the starting node), "node"
#' (a node in the isochrone), and "cost" (the cost of the path from the starting node to the node)
#' @examples
#' \dontrun{
#' # Create a graph
#' from <- c(1, 1, 1, 2, 2, 3, 3, 4, 5, 6, 6, 7)
#' to <- c(2, 3, 4, 3, 5, 4, 6, 6, 7, 5, 7, 8)
#' cost <- c(5.6, 3.8, 2.0, 1.2, 6.1, 4.2, 2.5, 7.8, 3.3, 1.7, 6.9, 2.2)
#'
#' graph <- makegraph(from, to, cost, directed = TRUE, coords = NULL)
#'
#' # Calculate isochrones for a graph object
#' isochrones <- get_isodist(graph, from = 2, lim = 10)
#' }
#' @export
#'
get_isodist <- function(Graph, from, lim) {
  # Check input consistency
  checkmate::assert_class(Graph, "Graph")
  if (any(is.na(from))) stop("NAs are not allowed in origin nodes")

  from <- as.character(from)
  if (sum(from %in% Graph@dict$ref) < length(from)) stop("Some nodes are not in the graph")
  from_id <- Graph@dict$id[match(from, Graph@dict$ref)]

  lim <- as.numeric(lim)
  if (any(is.na(lim))) stop("NAs are not allowed in cost value(s)")

  # Calculate isochrones using C++ function (Dijkstra)
  res <- calculateIsochroneRcpp(from = Graph@data$from,
                                to = Graph@data$to,
                                cost = Graph@data$cost,
                                start_nodes = from_id,
                                lim = lim)

  # Add ref
  res <- merge(res, Graph@dict, by.x = "node", by.y = "id")

  # Reorder and rename
  res <- data.frame(from = res$start_node,
                    to = res$node,
                    ref = res$ref,
                    cost = res$cost,
                    threshold = as.character(res$threshold))

  # Order the result by 'from', 'cost', and 'to'
  res <- res[with(res, order(from, cost, to)), ]
  rownames(res) <- NULL

  return(res)
}
