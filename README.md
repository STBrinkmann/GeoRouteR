
<!-- README.md is generated from README.Rmd. Please edit that file -->

# GeoRouteR

<!-- badges: start -->

[![R-CMD-check](https://github.com/STBrinkmann/GeoRouteR/actions/workflows/check-standard.yaml/badge.svg)](https://github.com/STBrinkmann/GeoRouteR/actions/workflows/check-standard.yaml)
<!-- badges: end -->

The goal of GeoRouteR is to provide easy-to-use functions for routing
and isochrone analysis on spatial data.

## Installation

You can install the development version of GeoRouteR from
[GitHub](https://github.com/) with:

``` r
# install.packages("devtools")
devtools::install_github("STBrinkmann/GeoRouteR")
```

## Example

This is a basic example which shows you how to solve a common problem:

``` r
library(GeoRouteR)

# Create a graph
from <- c(1, 1, 1, 2, 2, 3, 3, 4, 5, 6, 6, 7)
to <- c(2, 3, 4, 3, 5, 4, 6, 6, 7, 5, 7, 8)
cost <- c(5.6, 3.8, 2.0, 1.2, 6.1, 4.2, 2.5, 7.8, 3.3, 1.7, 6.9, 2.2)
coords <- data.frame(
  id = 1:8,
  lon = c(-73.987, -73.988, -73.986, -73.989, -73.984, -73.985, -73.990, -73.983),
  lat = c(40.753, 40.754, 40.755, 40.756, 40.757, 40.752, 40.758, 40.751)
)
graph <- makegraph(from, to, cost, coords = coords, crs = "epsg:4326")

# Generate isochrones
isochrones <- get_isodist(graph, from = 2, lim = 10)
```

This code creates a graph with `makegraph()`, using edge lists for
`from`, `to`, and `cost`, and spatial coordinates for `coords`. The
resulting `Graph` object is used to generate isochrones with
`get_isodist()`, specifying a starting point with `from` and a maximum
travel distance with `lim`.

With GeoRouteR, users can easily perform routing and isochrone analysis
on spatial data to better understand accessibility and travel time.
