# -Hopcroft-Karp-Experiment
Assignment on Matching Problem - Advanced Algorithms Course (UFRGS)

Original Link: https://www.inf.ufrgs.br/~mrpritt/doku.php?id=inf05016:2024-1-trabalhos

# Execution
```
  g++ graph.cpp main.cpp -o max_matching; ./graph_generator 5 5 0.3 6 | ./max_matching    
```
# Requirements

For generating the bipartite graph we need:
```
sudo apt install libfmt-dev
```  

Assume we have V vertices and the vertices indexed [0,n-1] are in M group and [n,2n-1] are in N group.
This requires a change into the [submodule](https://github.com/mrpritt/Emparelhamento_Maximo/):

```
  uniform_int_distribution<> p1(0, n1 - 1);
  uniform_int_distribution<> p2(0, n2-1);
```
use instead:
```
  uniform_int_distribution<> p1(0, n1 - 1);
  uniform_int_distribution<> p2(n1, 2*n2-1);
```
# Goals

  * Implement the Hopcroft-Karp algorithm that solves the maximum matching problem in bipartite graphs.
  * Document the implementation, particularly the data structures for representing the problem.
  * Conduct tests that demonstrate the algorithm's complexity is \(O(\sqrt{n}(n+m))\). Specifically: complexities \(O(\sqrt{n})\) for the number of phases and \(O(n+m)\) for extracting a maximal set of augmenting paths.
  * Decide experimentally: is the reduction approach to a flow problem more efficient?
  * Scalability test: find the "bottleneck matching" in selected large graphs (TBD).

# Test Cases
 * Generate random bipartite graphs with the probability of an edge 0 ⇐ p ⇐ 1.

# Conventions
 * The algorithm implementations should accept an undirected bipartite graph in DIMACS format on standard input (stdin) and print the cardinality of a maximum matching on standard output (stdout).
