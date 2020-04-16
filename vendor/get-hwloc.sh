#!/usr/bin/env bash

# url=https://download.open-mpi.org/release/hwloc/v2.2/hwloc-2.2.0.tar.bz2

set -euo pipefail

# parameters
version="2.2.0"
dest=$(python -c "import os;print(os.path.realpath('${1:?missing dest}'))")

# static
thisfile=$(python -c "import os;print(os.path.realpath('${BASH_SOURCE[0]}'))")
thisdir=$(dirname "${thisfile}")

download_untar() {
    local filename="hwloc-${version}"
    local fext="${filename}.tar.bz2"
    if [[ ! -f "${fext}" ]]; then
        local downloadurl="https://download.open-mpi.org/release/hwloc/v2.2/${fext}"
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

