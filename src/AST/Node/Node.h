//
// Created by creed on 1/5/26.
//

#ifndef CIPR_NODE_H
#define CIPR_NODE_H

#include <utility>
#include <vector>
#include <variant>
#include <string>
#include "../../Token/Token.h"

enum class NodeType {
    BINARY,
    GROUPING,
    LITERAL,
    UNARY,
    STMT_LIST,
    STMT_ECHO,
    STMT_EXPR,
    STMT_VAR_DECL,
    VAR_EXPR
};

struct Node {
    NodeType type;
    Token op;
    Literal value;

    std::vector<int> children;

    Node(const NodeType type, Token op, Literal value, std::vector<int> children)
        : type(type), op(std::move(op)), value(std::move(value)),
          children(std::move(children)) {}
};

class Arena {
public:
    int addNode(NodeType type, Token op, Literal value, const std::initializer_list<int> children) {
        nodes.emplace_back(type, std::move(op), std::move(value), children);

        return static_cast<int>(nodes.size()) - 1;
    }

    int addNode(NodeType type, Token op, Literal value, const std::vector<int> &children) {
        nodes.emplace_back(type, std::move(op), std::move(value), children);

        return static_cast<int>(nodes.size()) - 1;
    }

    Node& get(const int index) {
        return nodes[index];
    }

    const Node& get(const int index) const {
        return nodes[index];
    }

    size_t size() const {
        return nodes.size();
    }

    void clear() {
        nodes.clear();
    }

private:
    std::vector<Node> nodes;
};

#endif //CIPR_NODE_H