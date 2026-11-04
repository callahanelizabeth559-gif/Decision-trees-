#ifndef TREE_EXPLORER_H
#define TREE_EXPLORER_H

#include <string>
#include "Node.h"

// console exploration + about_tree.txt output.
// Only needs the Node struct and the root pointer (use tree.getRoot()).

// Finds the node with this preorder position, or nullptr if none.
Node* findNodeByPosition(Node* root, int position);

// One ancestor (the parent). nullptr for the root.
Node* findAncestor(Node* node);

// One descendant (first child). nullptr for a leaf.
Node* findDescendant(Node* node);

// One sibling (next one, or previous if it's the last child). nullptr if none.
Node* findSibling(Node* node);

// Prompt loop: asks for a position, prints the node info, stops on "exit".
void exploreTree(Node* root);

// Tree as dash-indented text (2 dashes per level, edge label in [ ]).
std::string buildTreeVisualization(Node* root);

// Returns false if the file can't be opened.
bool writeAboutTreeFile(Node* root,
                        const std::string& propertiesText,
                        const std::string& filename = "about_tree.txt");

#endif
