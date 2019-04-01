#include <iostream>
#include <string>
#include <sstream>

#include "history.hh"
#include "tokenize.hh"
#include "parse.hh"
#include "symbols.hh"
#include "builtin.hh"
#include "eval.hh"

int repl()
{
    History history;
    std::string line;
    Symbols s;
    Builtin b(s);
    while (!std::cin.eof()) {
        std::cout << "lisp> ";
        std::getline(std::cin, line);
        history.add(line);

        try {
            Tokenize tok(line);
            Parse p(tok);
            Eval e(p, s);
            Value *ev = e.eval();
            if (ev != nullptr)
                std::cout << ev->toString() << "\n";
        }
        catch (std::string s) {
            std::cerr << s << "\n";
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    return repl();
}
