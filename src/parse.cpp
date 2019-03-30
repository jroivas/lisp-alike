#include "parse.hh"

Value *Parse::readForm()
{
    std::string token = tokenize.peek();
    if (token == "(") {
        tokenize.next();
        return readList(")");
    }
    return nullptr;
}

Value *Parse::readList(std::string endMark)
{
    while (!tokenize.eof() && tokenize.peek() != endMark) {
        tokenize.next();
    }
    tokenize.next();
    return nullptr;
}
