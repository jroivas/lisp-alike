#pragma once

#include <string>
#include <vector>

class History
{
public:
    History();
    void add(std::string);
    size_t size() const {
        return lines.size();
    }

private:
    std::vector<std::string> lines;
};
