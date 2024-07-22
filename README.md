# Hopcroft-Karp-Experiment
Assignment on Matching Problem - Advanced Algorithms Course (UFRGS)

Original Link: https://www.inf.ufrgs.br/~mrpritt/doku.php?id=inf05016:2024-1-trabalhos

### Goals
  * Implement the Hopcroft-Karp algorithm that solves the maximum matching problem in bipartite graphs.
  * Document the implementation, particularly the data structures for representing the problem.
  * Conduct tests that demonstrate the algorithm's complexity is $O(\sqrt{n}(n+m))$. Specifically: complexities $O(\sqrt{n})$ for the number of phases and $O(n+m)$ for extracting a maximal set of augmenting paths.
  * Decide experimentally: is the reduction approach to a flow problem more efficient?
  * Scalability test: find the "bottleneck matching" in selected large graphs (TBD).

### Test Cases
 * Generate random bipartite graphs with the probability of an edge 0 ⇐ p ⇐ 1.

### Conventions
 * The algorithm implementations should accept an undirected bipartite graph in DIMACS format on standard input (stdin) and print the cardinality of a maximum matching on standard output (stdout).

## FLOW-COMPARISON

Flow comparison experiment is in 'FlowComparison' folder. 

Scripts for comparing Hopcroft-Karp with Edmonds-Karp is the 'compare_algorithms' script, and 'run_only_hopcroft_karp'.

Hopcroft-Karp implementation is in 'graph.cpp'.

### Requirements

For generating bipartite graph generator we need:

```
sudo apt install libfmt-dev
```  

Assume we have V vertices and the vertices indexed [0,n-1] are in M group and [n,2n-1] are in N group.

This requires a change into the [submodule](https://github.com/mrpritt/Emparelhamento_Maximo/):

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

We also use submodule git submodule https://github.com/schererl/EK-MaxFlowExperiment specifically with branch flow-matching-reduction.

## BvN DECOMPOSITION

Solution implemented in 'BvNDecomposition' folder.

Requires 'Emparelhamento_Maximo' submodule, specifically the matrices from it.

For executing it, use 'execute_bnd' script and select a matrix from 'Emparelhamento_Maximo' submodule in 'bm' folder.

### Considerations

The requirements could not be entirely achieved. For some reason, the implementation could not decompose the matrix until the highest value was 0 using e-05 precision. At a certain point, our implementation stops finding a perfect matchings.

#### Validation Functions
To identify possible mistakes, two functions were implemented:
- **validateMatching:** Checks if the matching is valid after finding a bottleneck. Ensures the matching (1) is perfect, (2) respects the bottleneck, and (3) the 'MATCH' structure is marked for both groups, m and n vertices.
- **boost_check:** Uses the Boost library to run the maximum matching. After each iteration in binary search, it verifies if the implementation's answer matches the Edmonds-Karp result from Boost. It checks for both perfect and imperfect matchings.

Still I could not find out why the decomposition wasn't possible


## LAST-UPDATES
- **(22.07)** Found a floating-point problem, still some examples are not converging 'bcspwr10.dsm.mtx.gz'.
- **(21.07)** Continued efforts to solve why BvN decomposition does not converge and stops finding perfect matching; no solution yet.
- **(21.07)** Added Edmonds-Karp for maximum matching using *Boost* lib as a verifier. It seems to lead to the same answers as the Hopcroft-Karp implementation.
- **(19.07)** BvN decomposition implementation: not finding perfect matching in the binary search, so it is not converging.
- **(19.07)** BFS and DFS simplified and improved in efficiency.
- **(19.07)** Augmenting path issue resolved.
- **(18.07)** Several issues found with the Hungarian Tree implementation.
- **(17.07)** Problem with graph_generator: Hopcroft-Karp finds 17 matchings, and Edmonds-Karp finds 18 matchings (SOLVED).
- **(17.07)** Problem with graph_generator (SOLVED).