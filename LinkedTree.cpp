
#include "LinkedTree.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Stores a node's information while reading the file
struct NodeRecord {
    int level;
    int position;
    std::string edgeLabel;
    std::string content;
};

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
    clear();

    // Helper for reporting errors
    auto fail = [&](const std::string& message) {
        clear();

        if (error != nullptr) {
            *error = message;
        }

        return false;
    };

    // Open file
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        return fail("Could not open input file: " + filename);
    }

    std::string line;
    int lineNumber = 0;

    // Find first non-empty line
    while (std::getline(inputFile, line)) {
        lineNumber++;

        if (line.find_first_not_of(" \t\r") != std::string::npos) {
            break;
        }
    }

    if (line.find_first_not_of(" \t\r") == std::string::npos) {
        return fail("Input file is empty.");
    }

    // Read root information
    std::string rootContent = line;
    int rootPosition = 1;

    std::istringstream rootRecord(line);

    int possibleLevel = -1;
    int possiblePosition = -1;

    if (rootRecord >> possibleLevel >> possiblePosition
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

    if (rootPosition <= 0) {
        return fail("Root position must be positive.");
    }

    // Store all remaining nodes before building the tree
    std::vector<NodeRecord> records;

    while (std::getline(inputFile, line)) {
        lineNumber++;

        // Skip blank lines
        if (line.find_first_not_of(" \t\r") == std::string::npos) {
            continue;
        }

        std::istringstream record(line);

        NodeRecord newRecord;

        // Read level, position, and edge label
        if (!(record >> newRecord.level
                     >> newRecord.position
                     >> newRecord.edgeLabel)) {

            return fail(
                "Invalid record on line " +
                std::to_string(lineNumber)
            );
        }

        // Read remaining text as node content
        std::getline(record, newRecord.content);

        std::size_t first =
            newRecord.content.find_first_not_of(" \t\r");

        if (first == std::string::npos) {
            return fail(
                "Node content missing on line " +
                std::to_string(lineNumber)
            );
        }

        newRecord.content =
            newRecord.content.substr(first);

        // Validate level
        if (newRecord.level <= 0) {
            return fail(
                "Invalid node level on line " +
                std::to_string(lineNumber)
            );
        }

        // Validate position
        if (newRecord.position <= 0 ||
            newRecord.position == rootPosition) {

            return fail(
                "Invalid node position on line " +
                std::to_string(lineNumber)
            );
        }

        records.push_back(newRecord);
    }

    if (inputFile.bad()) {
        return fail("Error reading input file: " + filename);
    }

    inputFile.close();

    // Sort nodes by preorder position
    std::sort(
        records.begin(),
        records.end(),
        [](const NodeRecord& a, const NodeRecord& b) {
            return a.position < b.position;
        }
    );

    // Check for duplicate positions
    for (std::size_t i = 1; i < records.size(); i++) {
        if (records[i].position == records[i - 1].position) {
            return fail("Duplicate preorder position.");
        }
    }

    // Create root node
    root = new Node(
        rootContent,
        "",
        rootPosition,
        0
    );

    nodesByPosition[rootPosition] = root;

    // Track the most recent node at each level
    std::vector<Node*> pathByLevel;

    pathByLevel.push_back(root);

    // Build the tree in preorder order
    for (const NodeRecord& current : records) {

        int level = current.level;

        // Make sure the parent level exists
        if (static_cast<std::size_t>(level) >
            pathByLevel.size()) {

            return fail("Invalid level sequence.");
        }

        // Find parent
        Node* parentNode = pathByLevel[level - 1];

        // Create new node
        Node* newNode = new Node(
            current.content,
            current.edgeLabel,
            current.position,
            level,
            parentNode
        );

        // Connect parent and child
        parentNode->children.push_back(newNode);

        // Store node by preorder position
        nodesByPosition[current.position] = newNode;

        // Update level tracking
        pathByLevel.resize(level);

        pathByLevel.push_back(newNode);
    }

    if (error != nullptr) {
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

    if (found == nodesByPosition.end()) {
        return nullptr;
    }

    return found->second;
}

const Node* LinkedTree::getNodeByPosition(int position) const {
    auto found = nodesByPosition.find(position);

    if (found == nodesByPosition.end()) {
        return nullptr;
    }

    return found->second;
}

// Return total number of nodes
std::size_t LinkedTree::getNodeCount() const {
    return nodesByPosition.size();
}
