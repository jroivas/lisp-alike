#pragma once

#include "value.hh"

typedef Value *(symbolHandler)(std::string symbol, Value *v);
