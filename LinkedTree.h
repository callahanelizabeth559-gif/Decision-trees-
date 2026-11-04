#ifndef LINKED_TREE_H
#define LINKED_TREE_H

#include "Node.h"

#include <cstddef>
#include <string>
#include <unordered_map>

class LinkedTree {
public:

    LinkedTree();
    ~LinkedTree();

    LinkedTree(const LinkedTree&) = delete;
    LinkedTree& operator=(const LinkedTree&) = delete;

    bool loadFromFile(
        const std::string& filename,
        std::string* error = nullptr
    );

    Node* getRoot();
    const Node* getRoot() const;

    Node* getNodeByPosition(int position);
    const Node* getNodeByPosition(int position) const;

    std::size_t getNodeCount() const;

    void clear();

private:

    Node* root;

    std::unordered_map<int, Node*> nodesByPosition;

    void deleteSubTree(Node* node);
};

#endif