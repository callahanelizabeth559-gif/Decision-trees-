#include "LinkedTree.h"

#include <fstream>
#include <sstream>
#include <vector>

// Constructor
LinkedTree::LinkedTree() : root(nullptr) {
}

// Destructor
LinkedTree::~LinkedTree() {
    clear();
}

// Delete tree recursively
void LinkedTree::deleteSubtree(Node* node) {

    if (node == nullptr) {
        return;
    }

    // Delete all child nodes first
    for (Node* child : node->children) {
        deleteSubtree(child);
    }

    delete node;
}

// Clear tree
void LinkedTree::clear() {

    deleteSubtree(root);

    root = nullptr;

    nodesByPosition.clear();
}

// Read file and build tree
bool LinkedTree::loadFromFile(
    const std::string& filename,
    std::string* error
) {

    // Remove previously loaded tree
    clear();

    // Helper for reporting errors safely
    auto fail = [&](const std::string& message) {
        clear();

        if (error != nullptr) {
            *error = message;
        }

        return false;
    };

    // Open input file
    std::ifstream inputFile(filename);

    if(!inputFile.is_open()) {
        return fail(
            "Could not open input file: " + filename
        );
    }

    std::string line;
    int lineNumber = 0;

    // Read root node
    
    // Skip empty lines
    while(std::getline(inputFile, line)) {
        lineNumber++;

        if(line.find_first_not_of(" \t\r")
            != std::string::npos) {

            break;
        }
    }

    if(line.find_first_not_of(" \t\r")
        == std::string::npos) {
        return fail("Input file is empty.");
    }

    std::string rootContent = line;

    int rootPosition = 1;

    std::istringstream rootRecord(line);

    int possibleLevel = -1;
    int possiblePosition = -1;

    if(rootRecord >> possibleLevel >> possiblePosition
        && possibleLevel == 0) {

        std::string remainingContent;

        std::getline(rootRecord, remainingContent);

        std::size_t first =
            remainingContent.find_first_not_of(" \t\r");

        if (first == std::string::npos) {
            return fail("Root content is missing.");
        }

        rootContent = remainingContent.substr(first);

        rootPosition = possiblePosition;
    }

    if(rootPosition <= 0) {
        return fail("Root position must be positive.");
    }

    // Create root node
    root = new Node(
        rootContent,
        "",
        rootPosition,
        0
    );

    nodesByPosition[rootPosition] = root;

    // Track nodes by level
    std::vector<Node*> pathByLevel;

    pathByLevel.push_back(root);

    // Read remaining tree nodes
    while (std::getline(inputFile, line)) {

        lineNumber++;

        // Skip blank lines
        if(line.find_first_not_of(" \t\r")
            == std::string::npos) {

            continue;
        }

        std::istringstream record(line);

        int level = -1;
        int position = -1;

        std::string edgeLabel;

        // Read the first three fields
        if(!(record >> level >> position >> edgeLabel)) {

            return fail(
                "Invalid record on line " +
                std::to_string(lineNumber)
            );
        }

        // Read remaining text as node content
        std::string content;

        std::getline(record, content);

        std::size_t first =
            content.find_first_not_of(" \t\r");

        if(first == std::string::npos) {
            return fail(
                "Node content missing on line " +
                std::to_string(lineNumber)
            );
        }

        content = content.substr(first);

        // Validate input
        if(level <= 0) {
            return fail(
                "Invalid node level on line " +
                std::to_string(lineNumber)
            );
        }

        if(position <= 0) {
            return fail(
                "Invalid node position on line " +
                std::to_string(lineNumber)
            );
        }

        if(nodesByPosition.count(position) != 0) {
            return fail(
                "Duplicate preorder position on line " +
                std::to_string(lineNumber)
            );
        }

        // Make sure the parent level exists
        if(static_cast<std::size_t>(level)
            > pathByLevel.size()) {

            return fail(
                "Invalid level sequence on line " +
                std::to_string(lineNumber)
            );
        }

        // Find parent
        Node* parentNode = pathByLevel[level - 1];

        // Create new node
        Node* newNode = new Node(
            content,
            edgeLabel,
            position,
            level,
            parentNode
        );

        // Connect parent and child
        parentNode->children.push_back(newNode);

        // Store node by preorder position
        nodesByPosition[position] = newNode;

        // Update level tracking
        pathByLevel.resize(level);

        pathByLevel.push_back(newNode);
    }

    // Check file reading
    if(inputFile.bad()) {

        return fail(
            "Error reading input file: " + filename
        );
    }

    inputFile.close();

    if(error != nullptr) {
        error->clear();
    }

    return true;
}

// Return root node
Node* LinkedTree::getRoot() {

    return root;
}

const Node* LinkedTree::getRoot() const {

    return root;
}

// Find node by preorder position
Node* LinkedTree::getNodeByPosition(int position) {

    auto found = nodesByPosition.find(position);

    if(found == nodesByPosition.end()) {

        return nullptr;
    }

    return found->second;
}

const Node* LinkedTree::getNodeByPosition(int position) const {

    auto found = nodesByPosition.find(position);

    if(found == nodesByPosition.end()) {

        return nullptr;
    }

    return found->second;
}

// Return total number of nodes
std::size_t LinkedTree::getNodeCount() const {

    return nodesByPosition.size();
}