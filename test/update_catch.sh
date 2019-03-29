#!/bin/bash

MYDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
curl -s "https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp" -o "${MYDIR}/catch.hpp"
