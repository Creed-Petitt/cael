//
// Created by creed on 1/5/26.
//

#ifndef CIPR_ASTPRINTER_H
#define CIPR_ASTPRINTER_H

#include "../Node/Node.h"
#include <string>
#include <vector>

class AstPrinter {
public:
    explicit AstPrinter(Arena& arena) : arena(arena) {}

    std::string print(int nodeIdx);

private:
    Arena& arena;
    std::string parenthesize(const std::string& name, const std::vector<int>& indices);
};

#endif //CIPR_ASTPRINTER_H