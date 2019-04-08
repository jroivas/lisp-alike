#!/bin/bash

MYDIR=$(pwd)

#MYDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if [ ! -e "${MYDIR}/catch.hpp" ] || [ "${1:-}" == "--force" ]; then
    curl -s "https://raw.githubusercontent.com/jroivas/catch-me2-if-you-can/master/include/catch-me2.hpp" -o "${MYDIR}/catch.hpp"
fi
