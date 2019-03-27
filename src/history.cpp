#include "history.hh"

History::History()
{
}

void History::add(std::string s)
{
    // Skip empty lines
    if (s.empty()) return;

    lines.push_back(s);
}
