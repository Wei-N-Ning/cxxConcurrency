#!/usr/bin/env bash

# https://github.com/STEllAR-GROUP/hpx/archive/1.4.1.tar.gz

set -euo pipefail

thisfile=$(python -c "import os;print(os.path.realpath('${BASH_SOURCE[0]}'))")
thisdir=$(dirname "${thisfile}")
version="1.4.1"
dest=$(python -c "import os;print(os.path.realpath('${1:?missing dest!}'))")
boost_dir="${thisdir}/$(ls "${thisdir}" | perl -wnl -E 'say if /^boost/i')"
hwloc_dir="${thisdir}/$(ls "${thisdir}" | perl -wnl -E 'say if /^hwloc/i')"
jemalloc_dir="${thisdir}/$(ls "${thisdir}" | perl -wnl -E 'say if /^jemalloc/i')"

do_download() {
    local fname="${version}"
    local fext="${fname}.tar.gz"
    local ofname="/tmp/${fext}"
    local odir="/tmp/hpx-${version}"
    if [[ ! -d "${odir}" ]]; then
        wget -O "${ofname}" "https://github.com/STEllAR-GROUP/hpx/archive/${fext}"
        ( cd /tmp && tar -xf "${ofname}" )
    fi
    echo "${odir}"
}

do_build() {
    local odir="${1}"
    cd "${odir}"
    mkdir -p build && cd build
    cmake -DBoost_NO_SYSTEM_PATHS=TRUE \
        -DBoost_USE_STATIC_LIBS=TRUE \
        -DBOOST_ROOT="${boost_dir}" \
        -DHWLOC_ROOT="${hwloc_dir}" \
        -DHPX_WITH_MALLOC=jemalloc \
        -DJEMALLOC_ROOT="${jemalloc_dir}" \
        -DCMAKE_INSTALL_PREFIX=${dest} ..
    make -j8
    make install
}

odir=$(do_download)
do_build "${odir}"

