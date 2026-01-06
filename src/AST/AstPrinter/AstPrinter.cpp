//
// Created by creed on 1/5/26.
//

#include "AstPrinter.h"

std::string AstPrinter::print(const int nodeIdx) {
    if (nodeIdx == -1) return "";

    switch (Node& node = arena.get(nodeIdx); node.type) {
        case NodeType::LITERAL: {
            if (std::holds_alternative<std::monostate>(node.value))
                return "nil";

            if (std::holds_alternative<double>(node.value)) {
                std::string s = std::to_string(std::get<double>(node.value));
                // Remove trailing zeros
                s.erase(s.find_last_not_of('0') + 1, std::string::npos);
                if (s.back() == '.') s.pop_back();
                return s;
            }

            if (std::holds_alternative<std::string>(node.value))
                return std::get<std::string>(node.value);

            return "";
        }
        case NodeType::UNARY:
            return parenthesize(node.op.lexeme, {node.right});
        case NodeType::BINARY:
            return parenthesize(node.op.lexeme, {node.left, node.right});
        case NodeType::GROUPING:
            return parenthesize("group", {node.left});
    }
    return "";
}

std::string AstPrinter::parenthesize(const std::string& name, const std::vector<int>& indices) {
    std::string result = "(" + name;
    for (int index : indices) {
        if (index != -1) result += " " + print(index);
    }
    result += ")";
    return result;
}