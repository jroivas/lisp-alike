#!/bin/bash

MYDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
curl -s "https://raw.githubusercontent.com/jroivas/catch-me2-if-you-can/master/include/catch-me2.hpp" -o "${MYDIR}/catch.hpp"
