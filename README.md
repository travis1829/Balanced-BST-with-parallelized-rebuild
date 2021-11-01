# Balanced-BST-with-parallelized-rebuild

This repository includes balanced binary search trees, and its variants that uses **parallelized** rebuilds to improve its performance.

The amortized weight balanced tree or the scapegoat tree uses the partial rebuild algorithm to rebalance itself. However, note that it is very easy to parallelize the partial rebuild algorithm. In fact, you just need to change a few lines! This repository includes some examples that shows how to do it.
* WBTree.h : Amortized weight balanced tree
* WBTreeP.h : Amortized weight balanced tree with parallelized rebuilds
* WBTreeTP.h : Amortized weight balanced tree with parallelized rebuilds (uses thread pool from https://github.com/progschj/ThreadPool)
* Scapegoat.h : Scapegoat tree
* ScapegoatP.h : Scapegoat tree with parallelized rebuilds

In the non-parallelized trees, the trees use the `_getCopy` and `_buildTree` methods to rebuild itself. On contrast, the trees with parallelized rebuilds additionally use the `_getCopyP` and `_buildTreeP` methods, which are only slightly different with the original `_getCopy` and `_buildTree` methods.