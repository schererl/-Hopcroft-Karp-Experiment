# -Hopcroft-Karp-Experiment
Assignment on Matching Problem - Advanced Algorithms Course (UFRGS)

Original Link: https://www.inf.ufrgs.br/~mrpritt/doku.php?id=inf05016:2024-1-trabalhos


# LAST-UPDATES

- Implementing BN decomposition: decomposition is not finding perfect matching in the binary search so it is not converging.

- changed BFS and DFS, it became simpler and seems more efficient.

- **augmenting pth not augmenting**: More problems.. (SOLVED)

- **serveral issues found**: I think I solved the problem, Hungarian Tree did not receive the necessary attention.

- **another here:** ./graph_generator 20 20 0.128 1000 HK finds 17 matching and EK finds 18 matchings. (SOLVED)

- **problem here:** ./graph_generator 11 11 0.128 1000 (SOLVED)


# Requirements

For generating the bipartite graph we need:
```
sudo apt install libfmt-dev
```  

Assume we have V vertices and the vertices indexed [0,n-1] are in M group and [n,2n-1] are in N group.
This requires a change into the [submodule](https://github.com/mrpritt/Emparelhamento_Maximo/):


We also use submodule git submodule https://github.com/schererl/EK-MaxFlowExperiment specifically with branch flow-matching-reduction.

Update the submodule with the branch mentioned to run it and compare with Hopcroft-karp execution.

```
  uniform_int_distribution<> p1(0, n1 - 1);
  uniform_int_distribution<> p2(0, n2-1);
```
use instead:
```
  uniform_int_distribution<> p1(0, n1 - 1);
  uniform_int_distribution<> p2(n1, 2*n2-1);
```

# Execution
```
  g++ Emparelhamento_Maximo/src/rand_bipartite.cpp -o graph_generator;
  g++ graph.cpp main.cpp -o max_matching; ./graph_generator 5 5 0.3 6 | ./max_matching    
```





# Goals

  * Implement the Hopcroft-Karp algorithm that solves the maximum matching problem in bipartite graphs.
  * Document the implementation, particularly the data structures for representing the problem.
  * Conduct tests that demonstrate the algorithm's complexity is $O(\sqrt{n}(n+m))$. Specifically: complexities $O(\sqrt{n})$ for the number of phases and $O(n+m)$ for extracting a maximal set of augmenting paths.
  * Decide experimentally: is the reduction approach to a flow problem more efficient?
  * Scalability test: find the "bottleneck matching" in selected large graphs (TBD).

# Test Cases
 * Generate random bipartite graphs with the probability of an edge 0 ⇐ p ⇐ 1.

# Conventions
 * The algorithm implementations should accept an undirected bipartite graph in DIMACS format on standard input (stdin) and print the cardinality of a maximum matching on standard output (stdout).
