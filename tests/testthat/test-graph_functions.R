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

test_that("activate_profile works", {
  edges <- data.frame(from = c("A", "A", "B", "C", "A"),
                      to = c("B", "C", "C", "D", "E"),
                      speed = c(10, 20, 40, 100, 20),
                      length = c(1, 2, 2, 1, 3),
                      oneway = c("FT", "B", "N", "TF", "foot_only"))
  
  nodes <- data.frame(node = c("A", "B", "C", "D", "E"),
                      X = c(0, 1, 1, 2, 3),
                      Y = c(0, 0, 1, 1, 2))
  
  crs <- "EPSG:4326"
  
  graph <- makegraph(edges, nodes, crs, directed = TRUE)
  
  graph$activate_profile(profile = "foot")
  testthat::expect_equal(graph$edges()$cost, c(0.012, 0.012, 0.024, 0.024, 0.024, 0.024, 0.036, 0.036))
  testthat::expect(nrow(graph$nodes()) == 4,
                   failure_message = "activate_profile(profile = \"foot\") did not clean nodes correctly")
  
  graph$activate_profile(profile = "bicycle")
  testthat::expect_equal(graph$edges()$cost, c(0.004, 0.008, 0.008, 0.045))
  testthat::expect(nrow(graph$nodes()) == 4,
                   failure_message = "activate_profile(profile = \"bicycle\") did not clean nodes correctly")
  
  graph$activate_profile(profile = "car")
  testthat::expect_equal(graph$edges()$cost, c(0.006, 0.006, 0.006, 0.0006))
  testthat::expect(nrow(graph$nodes()) == 4,
                   failure_message = "activate_profile(profile = \"car\") did not clean nodes correctly")
  
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
  
  testthat::expect_equal(isochrones$cost, c(0, 0.0060, 0.0060, 0.0066))
  testthat::expect(all(c(isochrones$from, isochrones$to) %in% graph$node_dict()$node),
                   failure_message = "All elements of isochrones must be a subset of graph$node_dict()$node")
})

test_that("distance_matrix works", {
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
  
  distance_matrix <- distance_matrix(graph, from = LETTERS[1:4], to = LETTERS[1:4])
  
  testthat::expect_equal(distance_matrix$cost, c(0.0060, 0.0060, 0.0066, 0.0030, 0.0036, 0.0006))
  testthat::expect(all(c(distance_matrix$from, distance_matrix$to) %in% graph$node_dict()$node),
                   failure_message = "All elements of distance_matrix must be a subset of graph$node_dict()$node")
})

