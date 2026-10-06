#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "Buffer.hpp"

int main() {
    sjtu::Buffer normal(
        std::vector<std::string>{"abc", ""}
    );

    assert(normal.GetLineCount() == 2);
    assert(normal.GetLineAt(0) == "abc");
    assert(normal.GetLineAt(1).empty());

    sjtu::Buffer empty(
        std::vector<std::string>{}
    );

    assert(empty.GetLineCount() == 1);
    assert(empty.GetLineAt(0).empty());

    std::cout << "Buffer basics passed\n";
}