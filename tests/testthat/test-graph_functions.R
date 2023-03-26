test_that("makegraph works", {
  from  <- c(1, 1, 1, 2, 2, 3, 3, 4, 5, 6, 6, 7)
  to    <- c(2, 3, 4, 3, 5, 4, 6, 6, 7, 5, 7, 8)
  cost  <- c(5.6, 3.8, 2.0, 1.2, 6.1, 4.2, 2.5, 7.8, 3.3, 1.7, 6.9, 2.2)
  
  # First test a non-spatial graph
  graph  <- makegraph(from, to, cost)
  
  testthat::expect_s4_class(graph, "Graph")
  testthat::expect_equal(sort(unique(c(from, to))),
                         sort(unique(as.numeric(graph@dict$ref))))
  
  # Test with coordinates
  coords <- data.frame(
    id = graph@dict$ref,
    lon = c(-73.987, -73.988, -73.986, -73.989, -73.984, -73.985, -73.990, -73.983),
    lat = c(40.753, 40.754, 40.755, 40.756, 40.757, 40.752, 40.758, 40.751)
  )
  graph2 <- makegraph(from, to, cost, coords = coords, crs = "epsg:4326")
  
  testthat::expect_s4_class(graph2, "Graph")
  testthat::expect_equal(coords$lon, graph2@coords$X)
  testthat::expect_equal(coords$lat, graph2@coords$Y)
})

test_that("get_isodist works", {
  from  <- c(1, 1, 1, 2, 2, 3, 3, 4, 5, 6, 6, 7)
  to    <- c(2, 3, 4, 3, 5, 4, 6, 6, 7, 5, 7, 8)
  cost  <- c(5.6, 3.8, 2.0, 1.2, 6.1, 4.2, 2.5, 7.8, 3.3, 1.7, 6.9, 2.2)
  
  # First test a non-spatial graph
  graph  <- makegraph(from, to, cost)
  
  isochrones <- get_isodist(graph, from = 2, lim = 10)
  
  testthat::expect_equal(isochrones$cost, c(1.2, 3.7, 5.4, 6.1))
  testthat::expect(all(isochrones$ref %in% graph@dict$ref),
                   failure_message = "All elements of isochrones$ref must be a subset of graph@dict$ref")
  testthat::expect(all(c(isochrones$from, isochrones$to) %in% graph@dict$id),
                   failure_message = "All elements of isochrones$from and isochrones$to must be a subset of graph@dict$id")
})

