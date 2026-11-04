#include "TreeAnalyzer.h"

#include <algorithm>
#include <cstdlib>
#include <sstream>

namespace {

// true if the node has no children
bool isLeaf(const Node* node) {
    return node->children.empty();
}

// height of the subtree rooted at node, in edges (leaf = 0)
int heightOf(const Node* node) {
    int best = 0;
    for (std::size_t i = 0; i < node->children.size(); ++i) {
        best = std::max(best, 1 + heightOf(node->children[i]));
    }
    return best;
}

// preorder walk that collects either internal or external nodes
void collectNodes(const Node* node, bool wantInternal, std::vector<const Node*>& result) {
    if ((!isLeaf(node)) == wantInternal) {
        result.push_back(node);
    }
    for (std::size_t i = 0; i < node->children.size(); ++i) {
        collectNodes(node->children[i], wantInternal, result);
    }
}

// true if every node in the subtree has at most 2 children
bool allAtMostTwoChildren(const Node* node) {
    if (node->children.size() > 2) {
        return false;
    }
    for (std::size_t i = 0; i < node->children.size(); ++i) {
        if (!allAtMostTwoChildren(node->children[i])) {
            return false;
        }
    }
    return true;
}

// true if every node in the subtree has exactly 0 or 2 children
bool allZeroOrTwoChildren(const Node* node) {
    if (node->children.size() != 0 && node->children.size() != 2) {
        return false;
    }
    for (std::size_t i = 0; i < node->children.size(); ++i) {
        if (!allZeroOrTwoChildren(node->children[i])) {
            return false;
        }
    }
    return true;
}

// true if every leaf in the subtree is exactly targetDepth below the starting node
bool allLeavesAtDepth(const Node* node, int currentDepth, int targetDepth) {
    if (isLeaf(node)) {
        return currentDepth == targetDepth;
    }
    for (std::size_t i = 0; i < node->children.size(); ++i) {
        if (!allLeavesAtDepth(node->children[i], currentDepth + 1, targetDepth)) {
            return false;
        }
    }
    return true;
}

// true if every node's child subtrees differ in height by at most 1
bool subtreeBalanced(const Node* node) {
    if (node->children.size() == 2) {
        int leftHeight = heightOf(node->children[0]);
        int rightHeight = heightOf(node->children[1]);
        if (std::abs(leftHeight - rightHeight) > 1) {
            return false;
        }
    } else if (node->children.size() == 1) {
        // missing child has height -1, so the only child must be a leaf
        if (heightOf(node->children[0]) > 0) {
            return false;
        }
    }
    for (std::size_t i = 0; i < node->children.size(); ++i) {
        if (!subtreeBalanced(node->children[i])) {
            return false;
        }
    }
    return true;
}

// "Yes" or "No"
const char* yesNo(bool value) {
    return value ? "Yes" : "No";
}

}  // namespace

int countInternalNodes(const Node* root) {
    if (root == nullptr) {
        return 0;
    }
    return static_cast<int>(getInternalNodesPreorder(root).size());
}

int countExternalNodes(const Node* root) {
    if (root == nullptr) {
        return 0;
    }
    return static_cast<int>(getExternalNodesPreorder(root).size());
}

int treeHeight(const Node* root) {
    if (root == nullptr) {
        return -1;
    }
    return heightOf(root);
}

std::vector<const Node*> getInternalNodesPreorder(const Node* root) {
    std::vector<const Node*> result;
    if (root != nullptr) {
        collectNodes(root, true, result);
    }
    return result;
}

std::vector<const Node*> getExternalNodesPreorder(const Node* root) {
    std::vector<const Node*> result;
    if (root != nullptr) {
        collectNodes(root, false, result);
    }
    return result;
}

bool isBinaryTree(const Node* root) {
    return root != nullptr && allAtMostTwoChildren(root);
}

bool isProperTree(const Node* root) {
    return isBinaryTree(root) && allZeroOrTwoChildren(root);
}

bool isPerfectTree(const Node* root) {
    return isProperTree(root) && allLeavesAtDepth(root, 0, heightOf(root));
}

bool isBalancedTree(const Node* root) {
    return isBinaryTree(root) && subtreeBalanced(root);
}

std::string buildPropertiesText(const Node* root) {
    std::ostringstream out;
    if (root == nullptr) {
        return out.str();
    }

    out << "Tree Properties:\n";
    out << "Root node: " << root->content << "\n";
    out << "Number of internal nodes: " << countInternalNodes(root) << "\n";
    out << "Number of external nodes: " << countExternalNodes(root) << "\n";
    out << "Tree height: " << treeHeight(root) << "\n";

    out << "Internal nodes (preorder):\n";
    std::vector<const Node*> internalNodes = getInternalNodesPreorder(root);
    for (std::size_t i = 0; i < internalNodes.size(); ++i) {
        out << internalNodes[i]->content << "\n";
    }

    out << "External nodes (preorder):\n";
    std::vector<const Node*> externalNodes = getExternalNodesPreorder(root);
    for (std::size_t i = 0; i < externalNodes.size(); ++i) {
        out << externalNodes[i]->content << "\n";
    }

    out << "\nBinary Tree Analysis:\n";
    bool binary = isBinaryTree(root);
    out << "Binary Tree: " << yesNo(binary) << "\n";
    if (binary) {
        out << "Proper Tree: " << yesNo(isProperTree(root)) << "\n";
        out << "Perfect Tree: " << yesNo(isPerfectTree(root)) << "\n";
        out << "Balanced Tree: " << yesNo(isBalancedTree(root)) << "\n";
    }
    return out.str();
}