#ifndef TREE_ANALYZER_H
#define TREE_ANALYZER_H

#include <string>
#include <vector>
#include "Node.h"

// Tree properties and binary tree analysis.
// Only needs the Node struct and the root pointer (use tree.getRoot()).

// Number of nodes that have at least one child.
int countInternalNodes(const Node* root);

// Number of nodes with no children (leaves).
int countExternalNodes(const Node* root);

// Height of the tree in edges (a single root node has height 0).
// Returns -1 for an empty tree.
int treeHeight(const Node* root);

// Internal nodes in preorder.
std::vector<const Node*> getInternalNodesPreorder(const Node* root);

// External (leaf) nodes in preorder.
std::vector<const Node*> getExternalNodesPreorder(const Node* root);

// True if every node has at most 2 children.
bool isBinaryTree(const Node* root);

// True if it is binary and every node has 0 or 2 children.
bool isProperTree(const Node* root);

// True if it is proper and all leaves are on the same level.
bool isPerfectTree(const Node* root);

// True if it is binary and, at every node, the heights of the
// two child subtrees differ by at most 1 (a missing child counts as -1).
bool isBalancedTree(const Node* root);

// Builds the full "Tree Properties" + "Binary Tree Analysis" text
// that gets passed to writeAboutTreeFile().
std::string buildPropertiesText(const Node* root);

#endif