#!/usr/bin/env bash

setUp() {
    set -e
}

disassemble() {
    local func=${2-"main"}
    gdb -batch \
    -ex "set disassembly-flavor intel" \
    -ex "file ${1:?missing binary path}" \
    -ex "disassemble /rs ${func}"
}

# $1: compiler exe
# $2: source file path
# $3: optional binary file path; the default is /tmp/_
# $4: optional linker arguments
compile() {
    ${1:?"missing compiler exe"} -std=c++14 -g -Wall -Werror \
${2:?"missing source file path"} \
-fno-stack-check -fno-stack-protector -fomit-frame-pointer \
-o ${3:-/tmp/_} ${4}
}