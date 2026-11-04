#include <iostream>
#include <string>

#include "LinkedTree.h"
#include "TreeAnalyzer.h"
#include "TreeExplorer.h"

int main(int argc, char* argv[]) {
    // The tree file name must be given on the command line
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <tree-file>\n";
        return 1;
    }

    // Build the tree from the input file
    LinkedTree tree;
    std::string error;
    if (!tree.loadFromFile(argv[1], &error)) {
        std::cerr << "Error: " << error << "\n";
        return 1;
    }

    // Write the tree drawing and its properties to about_tree.txt
    if (!writeAboutTreeFile(tree.getRoot(), buildPropertiesText(tree.getRoot()), "about_tree.txt")) {
        std::cerr << "Error: could not write about_tree.txt\n";
        return 1;
    }

    // Start the interactive console until the user types "exit"
    exploreTree(tree.getRoot());

    return 0;
}