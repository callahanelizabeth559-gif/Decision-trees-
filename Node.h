#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

struct Node {
    std::string content;
    std::string edgeLabel;

    int position;
    int level;

    Node* parent;
    std:: vector<Node*> children;

    Node(const std::string& text,
         const std::string& label,
         int pos,
         int nodeLevel,
        Node* parentNode = nullptr)

        : content(text),
          edgeLabel(label),
          position(pos),
          level(nodeLevel),
          parent(parentNode), {
          }
};

#endif
