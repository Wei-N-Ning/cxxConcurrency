# Coroutines

C++ boost app dev P/313

> nowadays, plenty of embedded devices still have only a single core
> developers write for those device, trying to squeeze maximum performance out 
> of them. Using Boost.Thread or std::thread for such devices is not effective
> the OS will be forced to schedule threads for execution, manage resources and
> so on, as the hardware can not run them in parallel
> so how can we make a program switch to the execution of a subprogram while
> waiting for some resource in the main part ?

## Boost.Coroutine

c++ boost app dev P/314

coroutine is a subroutine that allow multiple entry points

multiple entry points give us an ability to suspend and resume the execution
of a program at certain locations
