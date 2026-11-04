#include "TreeExplorer.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {

// strips spaces from both ends
std::string trim(const std::string& text) {
    std::size_t start = 0;
    std::size_t end = text.size();
    while (start < end && std::isspace(static_cast<unsigned char>(text[start]))) {
        ++start;
    }
    while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1]))) {
        --end;
    }
    return text.substr(start, end - start);
}

// true only if the whole string is a whole number
bool parsePosition(const std::string& input, int& position) {
    if (input.empty() || input.size() > 9) {
        return false;
    }
    for (std::size_t i = 0; i < input.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(input[i]))) {
            return false;
        }
    }
    position = std::stoi(input);
    return true;
}

// content text, or "None" if there's no node
std::string contentOrNone(const Node* node) {
    return node ? node->content : "None";
}

// adds this node and its subtree to the output in preorder
void appendSubtree(const Node* node, std::ostringstream& out) {
    out << std::string(2 * node->level, '-');
    if (!node->edgeLabel.empty()) {
        out << "[" << node->edgeLabel << "] ";
    }
    out << node->content << "\n";
    for (std::size_t i = 0; i < node->children.size(); ++i) {
        appendSubtree(node->children[i], out);
    }
}

}  // namespace

Node* findNodeByPosition(Node* root, int position) {
    if (root == nullptr) {
        return nullptr;
    }
    if (root->position == position) {
        return root;
    }
    for (std::size_t i = 0; i < root->children.size(); ++i) {
        Node* found = findNodeByPosition(root->children[i], position);
        if (found != nullptr) {
            return found;
        }
    }
    return nullptr;
}

Node* findAncestor(Node* node) {
    return node ? node->parent : nullptr;
}

Node* findDescendant(Node* node) {
    if (node == nullptr || node->children.empty()) {
        return nullptr;
    }
    return node->children[0];
}

Node* findSibling(Node* node) {
    if (node == nullptr || node->parent == nullptr) {
        return nullptr;
    }
    const std::vector<Node*>& siblings = node->parent->children;
    for (std::size_t i = 0; i < siblings.size(); ++i) {
        if (siblings[i] == node) {
            if (i + 1 < siblings.size()) {
                return siblings[i + 1];
            }
            if (i > 0) {
                return siblings[i - 1];  // last child, so use the previous
            }
        }
    }
    return nullptr;
}

void exploreTree(Node* root) {
    std::string input;
    while (true) {
        std::cout << "Which node would you like to explore "
                     "(enter position or \"exit\"): ";
        if (!std::getline(std::cin, input)) {
            std::cout << "\nGoodbye!\n";  // input ended
            return;
        }
        input = trim(input);

        if (input == "exit") {
            std::cout << "Goodbye!\n";
            return;
        }

        int position = 0;
        Node* node = nullptr;
        if (parsePosition(input, position)) {
            node = findNodeByPosition(root, position);
        }
        if (node == nullptr) {
            std::cout << "Invalid input. Please try again.\n";
            continue;
        }

        std::cout << "Node's content: " << node->content << "\n";
        std::cout << "Ancestor: " << contentOrNone(findAncestor(node)) << "\n";
        std::cout << "Descendant: " << contentOrNone(findDescendant(node)) << "\n";
        std::cout << "Sibling: " << contentOrNone(findSibling(node)) << "\n";
    }
}

std::string buildTreeVisualization(Node* root) {
    std::ostringstream out;
    if (root != nullptr) {
        appendSubtree(root, out);
    }
    return out.str();
}

bool writeAboutTreeFile(Node* root,
                        const std::string& propertiesText,
                        const std::string& filename) {
    std::ofstream file(filename.c_str());
    if (!file.is_open()) {
        return false;
    }
    file << buildTreeVisualization(root) << "\n" << propertiesText;
    if (!propertiesText.empty() && propertiesText.back() != '\n') {
        file << "\n";
    }
    return file.good();
}
