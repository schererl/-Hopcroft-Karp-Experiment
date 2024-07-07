# -Hopcroft-Karp-Experiment
Assignment on Matching Problem - Advanced Algorithms Course (UFRGS)

Original Link: https://www.inf.ufrgs.br/~mrpritt/doku.php?id=inf05016:2024-1-trabalhos

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
