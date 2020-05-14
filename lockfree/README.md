# Lock-free data structures

## Video notes

lock free hash table (google talk): video_notes.md

## A simple counter using std::atomic

see: counter.cpp

NOTE: counter does not (and can not) use CAS idiom, because 
it depends on the previous computation (the lineage)

CAS works when each computation produces results independently to each other

## A simple (buggy) lock-free stack using CAS idiom

note the impl of Stack::push and Stack::pop does not account for resource deallocation.

