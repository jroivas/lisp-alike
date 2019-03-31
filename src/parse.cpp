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

Value *Parse::processMacro(std::string symbol)
{
    Value *v = new SymbolValue(symbol);
    v->setNext(readForm());
    return v;
}

Value *Parse::readMacro(std::string token)
{
    if (token == "'") return processMacro("quote");

    return nullptr;
}

Value *Parse::readAtom()
{
    std::string token = tokenize.next();
    if (token.empty()) return nullptr;

    if (token.length() >= 2 && token[0] == '"' &&
            token[token.length() -1] == '"')
        return new StringValue(token.substr(1, token.length() - 2));

    if (std::regex_match(token, intRegex))
        return new IntValue(atoll(token.c_str()));

    Value *macro = readMacro(token);
    if (macro != nullptr)
        return macro;

    return new SymbolValue(token);
}

Value *Parse::readList(std::string endMark)
{
    Value *first = nullptr;
    Value *current = nullptr;
    while (!tokenize.eof() && tokenize.peek() != endMark) {
        Value *v = readForm();
        if (v == nullptr) break;

        if (first == nullptr) first = v;
        else current->addLast(v);

        current = v;
    }
    tokenize.next();
    return first;
}
