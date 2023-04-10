
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

This is a basic example which shows you how to build a basic Graph:

``` r
library(GeoRouteR)

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
graph
#> Graph summary:
#> -----------------
#> Number of nodes: 4 
#> Number of edges: 4 
#> CRS: EPSG:4326 
#> Active profile: default
```

This code creates a graph with `makegraph()`, using the edge
`data.frame` for `from`, `to`, and `cost`, and spatial coordinates from
the `nodes` `data.frame` containing the `node`, `X` and `Y` coordinates.
The resulting `Graph` object is stored in C++ and a pointer is used for
calling the relevant methods. We can now use use the `isochrone`
function to generate isochrones.

``` r
isochrones <- isochrone(graph, from = "A", lim = c(2, 6))
isochrones
#>   from to  cost threshold
#> 1    A  B 0.360         2
#> 2    A  C 0.360         2
#> 3    A  D 0.396         2
```

You can also change the speed profile to foot, bicylce and car.

- foot: sets speed to 5 km/h, ignores oneway rules and disables roads
  with speed \> 90 km/h

- bicycle: sets speed to 15 km/h, disables roads with speed \> 90 km/h

- car: speed and oneway rules are taken from user input (edges)

``` r
graph$activate_profile(profile = "foot")
graph
#> Graph summary:
#> -----------------
#> Number of nodes: 3 
#> Number of edges: 6 
#> CRS: EPSG:4326 
#> Active profile: foot
```

``` r
graph$edges()
#>   from to cost speed length oneway
#> 1    0  1 0.72     5      1      B
#> 2    1  0 0.72     5      1      B
#> 3    0  2 1.44     5      2      B
#> 4    2  0 1.44     5      2      B
#> 5    1  2 1.44     5      2      B
#> 6    2  1 1.44     5      2      B
```

  
  
