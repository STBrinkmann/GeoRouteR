
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

### Create a Graph

This is a basic example which shows you how to build a basic S6 Graph
object:

``` r
library(GeoRouteR)

edges <- data.frame(from = c("A", "A", "C", "B", "A"),
                    to = c("B", "C", "D", "C", "E"),
                    speed = c(10, 20, 40, 100, 20),
                    length = c(1, 2, 2, 1, 3),
                    oneway = c("FT", "B", "N", "TF", "foot_only"))

nodes <- data.frame(node = c("A", "B", "D", "C", "E"),
                    X = c(0, 1, 1, 2, 3),
                    Y = c(0, 0, 1, 1, 2))

crs <- "EPSG:4326"

graph <- makegraph(edges, nodes, crs, directed = TRUE)
graph
#> Graph summary:
#> -----------------
#> Number of nodes: 5 
#> Number of edges: 5 
#> CRS: EPSG:4326 
#> Active profile: default
```

This code creates a graph with `makegraph()`, using the edge
`data.frame` for `from`, `to`, and `cost`, and spatial coordinates from
the `nodes` `data.frame` containing the `node`, `X` and `Y` coordinates.
The resulting `Graph` object is stored in C++ and a pointer is used for
calling the relevant methods.

### Adjust speed profile

You can also change the speed profile to foot, bicylce and car. This
will adjust the edge costs and change the graph based on the oneway
rules.

- “foot”  
  Sets speed to 5 km/h, ignores oneway rules and disables roads with
  speed \> 90 km/h

- “bicycle”  
  Sets speed to 15 km/h, 4 km/h for foor_only edges, disables roads with
  speed \> 90 km/h

- “car”  
  Speed and oneway rules are taken from user input (edges)

- “default”  
  Restores default settings

``` r
graph$activate_profile(profile = "foot")
graph
#> Graph summary:
#> -----------------
#> Number of nodes: 5 
#> Number of edges: 8 
#> CRS: EPSG:4326 
#> Active profile: foot
```

``` r
graph$edges()
#>   from to  cost speed length oneway
#> 1    0  1 0.012     5      1      B
#> 2    1  0 0.012     5      1      B
#> 3    0  3 0.024     5      2      B
#> 4    3  0 0.024     5      2      B
#> 5    3  2 0.024     5      2      B
#> 6    2  3 0.024     5      2      B
#> 7    0  4 0.036     5      3      B
#> 8    4  0 0.036     5      3      B
```

### Calculate isochrones and distance matrix

We can now use use the `isochrone` function to generate isochrones with
the “foot” active.

``` r
isochrones <- isochrone(graph, from = "A", lim = c(0.02, 0.03))
isochrones
#>   from to  cost threshold
#> 1    A  A 0.000      0.02
#> 2    A  B 0.012      0.02
#> 3    A  C 0.024      0.03
```

We can also use the `distance_matrix` function to calculate the the
shortest paths between mutliple nodes in our Graph.

``` r
dist_mat <- distance_matrix(Graph = graph,
                            from = c("A", "B", "C", "E"),
                            to = c("A", "B", "C", "E"))
dist_mat
#>    from to  cost
#> 1     A  B 0.012
#> 2     A  C 0.024
#> 3     A  E 0.036
#> 4     B  A 0.012
#> 5     B  C 0.036
#> 6     B  E 0.048
#> 7     C  A 0.024
#> 8     C  B 0.036
#> 9     C  E 0.060
#> 10    E  A 0.036
#> 11    E  B 0.048
#> 12    E  C 0.060
```
