#include <iostream>
#include <string>
#include <sstream>

#include "history.hh"
#include "tokenize.hh"

int repl()
{
    History history;
    std::string line;
    while (!std::cin.eof()) {
        std::cout << "lips> ";
        std::getline(std::cin, line);
        history.add(line);

#ifdef DEBUG
        std::cout << "LL: " << line << " " << line.size()
            << " " << history.size() << "\n";
#endif
        Tokenize tok(line);
        while (!tok.eof()) {
            std::cout << "Token: " << tok.next() << "\n";
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    return repl();
}
