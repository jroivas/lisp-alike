#include "parse.hh"

#include <iostream>
#include <regex>

static const std::regex intRegex("^[-+]?\\d+$");

Value *Parse::readForm()
{
    std::string token = tokenize.peek();
    if (token == "(") {
        tokenize.next();
        return readList(")");
    }
    return readAtom();
}

Value *Parse::readAtom()
{
    std::string token = tokenize.next();
    if (token.length() >= 2 && token[0] == '"' &&
            token[token.length() -1] == '"')
        return new StringValue(token.substr(1, token.length() - 2));

    if (std::regex_match(token, intRegex))
        return new IntValue(atoll(token.c_str()));

    return nullptr;
}

Value *Parse::readList(std::string endMark)
{
    Value *first = nullptr;
    Value *current = nullptr;
    while (!tokenize.eof() && tokenize.peek() != endMark) {
        tokenize.next();
        Value *v = readForm();
        if (v == nullptr) {
            break;
        }
        if (first == nullptr) {
            first = v;
        } else current->addLast(v);
        current = v;
    }
    tokenize.next();
    return first;
}
