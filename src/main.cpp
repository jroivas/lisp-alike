#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>

#include "history.hh"
#include "tokenize.hh"
#include "parse.hh"
#include "symbols.hh"
#include "builtin.hh"
#include "eval.hh"
#include "env.hh"

int repl(bool terminal)
{
    History history;
    std::string line;
    Symbols s;
    Env env;
    Builtin b(s);
    while (!std::cin.eof()) {
        if (terminal) std::cout << "lisp> ";
        std::getline(std::cin, line);
        if (std::cin.eof()) break;
        history.add(line);

        try {
            Value *ev = evalLine(s, env, line);
            if (ev != nullptr)
                std::cout << ev->toString() << "\n";
            else std::cout << "nil\n";
        }
        catch (std::string s) {
            std::cerr << s << "\n";
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    return repl(isatty(fileno(stdin)));
}
