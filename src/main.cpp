#include <iostream>
#include <string>
#include <sstream>

#include "history.hh"
#include "tokenize.hh"
#include "parse.hh"

int repl()
{
    History history;
    std::string line;
    while (!std::cin.eof()) {
        std::cout << "lips> ";
        std::getline(std::cin, line);
        history.add(line);

        Tokenize tok(line);
        Parse p(tok);
        Value *v = p.readForm();
        if (v != nullptr)
            std::cout << v->toString() << "\n";
    }
    return 0;
}

int main(int argc, char **argv)
{
    return repl();
}
