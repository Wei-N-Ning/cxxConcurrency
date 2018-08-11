#!/usr/bin/env bash

# EMC++ Item 39 P265

# (for the while(!flag) approach)
# less good is the cost of polling in the reacting task
# during the time the task is waiting for the flag to
# be set, the task is essentially blocked, yet it is
# still running.

# As such, it occupies a hardware thread that another
# task might be able to make use of, it incurs the
# cost of a context switch each time it starts or completes
# its time-slice, and it could keep a core running that
# might otherwise be shut down to save power
# a truly blocked task would do none of these things

# a task in a wait call is truly blocked

source ../utilities.sh

# $1: exe path
perfStat() {
    perf stat ${1} 2>&1 | awk '
NR == 1 { print }
/instructions/ { print }
'
}

setUp
compile g++ ./async_events.cpp /tmp/_ -lpthread
perfStat "/tmp/_"  # polling
perfStat "/tmp/_ 1"  # waiting, truly blocked

