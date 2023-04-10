test_that("makegraph works", {
  edges <- data.frame(from = c("A", "A", "B", "C"),
                      to = c("B", "C", "C", "D"),
                      speed = c(10, 20, 40, 100),
                      length = c(1, 2, 2, 1),
                      oneway = c("FT", "B", "N", "TF"))
  
  nodes <- data.frame(node = c("A", "B", "C", "D"),
                      X = c(0, 1, 1, 2),
                      Y = c(0, 0, 1, 1))
  
  crs <- "EPSG:4326"
  
  graph <- makegraph(edges, nodes, crs, directed = TRUE)
  
  testthat::expect_is(graph, "Graph")
  testthat::expect_equal(sort(nodes$node),
                         sort(graph$node_dict()$node))
})

test_that("isochrone works", {
  edges <- data.frame(from = c("A", "A", "B", "C"),
                      to = c("B", "C", "C", "D"),
                      speed = c(10, 20, 40, 100),
                      length = c(1, 2, 2, 1),
                      oneway = c("FT", "B", "N", "TF"))
  
  nodes <- data.frame(node = c("A", "B", "C", "D"),
                      X = c(0, 1, 1, 2),
                      Y = c(0, 0, 1, 1))
  
  crs <- "EPSG:4326"
  
  graph <- makegraph(edges, nodes, crs, directed = TRUE)
  
  isochrones <- isochrone(graph, from = "A", lim = 10)
  
  testthat::expect_equal(isochrones$cost, c(0.0060, 0.0060, 0.0066))
  testthat::expect(all(c(isochrones$from, isochrones$to) %in% graph$node_dict()$node),
                   failure_message = "All elements of isochrones$ref must be a subset of graph$node_dict()$node")
})

test_that("isochrone works", {
  edges <- data.frame(from = c("A", "A", "B", "C"),
                      to = c("B", "C", "C", "D"),
                      speed = c(10, 20, 40, 100),
                      length = c(1, 2, 2, 1),
                      oneway = c("FT", "B", "N", "TF"))
  
  nodes <- data.frame(node = c("A", "B", "C", "D"),
                      X = c(0, 1, 1, 2),
                      Y = c(0, 0, 1, 1))
  
  crs <- "EPSG:4326"
  
  graph <- makegraph(edges, nodes, crs, directed = TRUE)
  
  graph$activate_profile(profile = "foot")
  testthat::expect_equal(graph$edges()$cost, c(0.012, 0.012, 0.024, 0.024, 0.024, 0.024))
  testthat::expect(nrow(graph$nodes()) == 3,
                   failure_message = "activate_profile(profile = \"foot\") did not clean nodes correctly")
  
  graph$activate_profile(profile = "bicycle")
  testthat::expect_equal(graph$edges()$cost, c(0.004, 0.008, 0.008))
  testthat::expect(nrow(graph$nodes()) == 3,
                   failure_message = "activate_profile(profile = \"foot\") did not clean nodes correctly")
  
  graph$activate_profile(profile = "car")
  testthat::expect_equal(graph$edges()$cost, c(0.006, 0.006, 0.006, 0.0006))
  testthat::expect(nrow(graph$nodes()) == 4,
                   failure_message = "activate_profile(profile = \"car\") did not clean nodes correctly")
  
})

