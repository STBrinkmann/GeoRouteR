test_that("makegraph works", {
  edges <- data.frame(from = c("A", "A", "B", "C"),
                      to = c("B", "C", "C", "D"),
                      cost = c(1, 2, 3, 4),
                      dist = c(1, 2, 2, 1))
  
  nodes <- data.frame(node = c("A", "B", "C", "D"),
                      X = c(0, 1, 1, 2),
                      Y = c(0, 0, 1, 1))
  
  crs <- "EPSG:4326"
  
  graph <- makegraph(edges, nodes, crs, directed = TRUE)
  
  testthat::expect_is(graph, "Graph")
  testthat::expect_equal(sort(nodes$node),
                         sort(graph$node_dict()$key))
})

test_that("isochrone works", {
  edges <- data.frame(from = c("A", "A", "B", "C"),
                      to = c("B", "C", "C", "D"),
                      cost = c(1, 2, 3, 4),
                      dist = c(1, 2, 2, 1))
  
  nodes <- data.frame(node = c("A", "B", "C", "D"),
                      X = c(0, 1, 1, 2),
                      Y = c(0, 0, 1, 1))
  
  crs <- "EPSG:4326"
  
  graph <- makegraph(edges, nodes, crs, directed = TRUE)
  
  isochrones <- isochrone(graph, from = "A", lim = 10)
  
  testthat::expect_equal(isochrones$cost, c(1, 2, 6))
  testthat::expect(all(c(isochrones$from, isochrones$to) %in% graph$node_dict()$key),
                   failure_message = "All elements of isochrones$ref must be a subset of graph$node_dict()$key")
})

