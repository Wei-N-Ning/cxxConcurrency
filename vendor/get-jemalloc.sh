#!/usr/bin/env bash

# https://github.com/jemalloc/jemalloc/releases/download/5.2.1/jemalloc-5.2.1.tar.bz2

set -euo pipefail

# parameters
version="5.2.1"
dest=$(python -c "import os;print(os.path.realpath('${1:?missing dest}'))")

# static
thisfile=$(python -c "import os;print(os.path.realpath('${BASH_SOURCE[0]}'))")
thisdir=$(dirname "${thisfile}")

download_untar() {
    local filename="jemalloc-${version}"
    local fext="${filename}.tar.bz2"
    if [[ ! -f "${fext}" ]]; then
        local downloadurl="https://github.com/jemalloc/jemalloc/releases/download/${version}/${fext}"
        wget "${downloadurl}"
    fi
    if [[ ! -d "${filename}" ]]; then
        tar -xf "${filename}.tar.bz2"
    fi
    echo -n "${filename}"
}

do_install() {
    ./configure --prefix="${dest}"
    make -j8 && make install
}

cd /tmp
d=$(download_untar)
cd "${d}"
do_install

