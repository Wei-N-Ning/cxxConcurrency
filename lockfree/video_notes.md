
## source:

lock free hash table (google talk)

## goal:
```
no locks, even during table resize
no spin-locks
no blocking while hold locks
all CAS spin-loops bounded
make progress even if other threads die (no damage to the table)

```

## requires:
```
CAS (compare and swap)
 - test a value in memory, if unequal do not update, 
LL/SC (load-linked / store-conditional - PowerPc only)

```

the less percentage on read (99% reads, 95%, 75% ...), 
the less caching can help,
the more his lock-free hash table can speed things up

extreme case is when update (write) is the dominating action, 
this is the case lock-free can greatly help, scale almost 
linearly (with the amount of CPU/hw threads)


in a nut shell, what all the threads are doing is to see what state 
they are dealing with, and what is the next state - recall his 
state machine model;

on the contrary, thinking about the key and value individually might 
result in complicated switch-cases

> can I create a degenerated state machine (say, only allow set and 
get) then verify its correctness?

there can be only one prime value ('V) - all the inbetween primes 
are trashed; the only prime represents the value that is older 
than the value I copy into

> recall that I recently tried hard to understand the patience-sort
algorithm, and it turned out that it also did not book keep the 
in-between state (e.g. all the less optimal solutions)

## testing scalability:

see the gradation from 99% read to 75% read - the mutex based 
implementation had a stroke at high thread count

```
always start with a small hash table
let it rapidly scale to the ideal size
test the extreme case: large number of resize - see who has stroke
```

there is one word that holds the pointer to the next new time table,
and they (the threads) CAS it (from null to the new table); 
one guy wins the CAS;
the other guys lose it and throw their table aways;
refer to the state machine diagram

## summary:

any thread can see any state at any time

they must assume values change at each step

state graphs really helped coding & debugging

tools to write code - now that he has a state machine, 
why not have the source code automatically generated from it? 
example and use case is Java's concurrent vector


