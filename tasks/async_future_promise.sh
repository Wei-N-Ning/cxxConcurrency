#!/usr/bin/env bash

# compare with async_events.sh

# 1048575, 65535
#        28,912,935      instructions
# 1048575, 65535
#         9,184,862      instructions

# this one yields a similar result:
# 1048575, 65535
#         9,186,002      instructions

source ../utilities.sh

# $1: exe path
perfStat() {
    perf stat ${1} 2>&1 | awk '
NR == 1 { print }
/instructions/ { print }
'
}

setUp
compile g++ ./async_future_promise.cpp /tmp/_ -lpthread
perfStat "/tmp/_"
