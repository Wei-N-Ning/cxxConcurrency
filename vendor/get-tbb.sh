#!/usr/bin/env bash
set -euo pipefail

thisfile=$(python -c "import os;print(os.path.realpath('${BASH_SOURCE[0]}'))")
thisdir=$(dirname "${thisfile}")
url="https://github.com/oneapi-src/oneTBB/archive/v2020.2.tar.gz"

cd /tmp
wget "${url}"
tar -xf 'v2020.2.tar.gz'
rm -rf "${thisdir}/oneTBB" && cp -r 'oneTBB-2020.2' "${thisdir}/oneTBB"
