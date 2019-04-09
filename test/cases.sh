#!/bin/bash

set -eu

MYDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
binary=$1

find "${MYDIR}/cases/"*.out | while read f; do
    b=$(basename "$f" .out)
    if ! diff -u <("${binary}" < "${MYDIR}/cases/$b") "$f"; then
        echo "^^ ERROR test case caused faulty output: $b"
    fi
done
