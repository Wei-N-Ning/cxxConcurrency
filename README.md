# C++ Concurrency and Parallelism

## future and thread pool

C++ 17 (GCC 7.5.0) does not ship threadpool.

see perf analysis: <https://stackoverflow.com/questions/14351352/does-asynclaunchasync-in-c11-make-thread-pools-obsolete-for-avoiding-expen>

here is a high-rating threadpool: <https://github.com/progschj/ThreadPool>

## HPX

read: <https://www.modernescpp.com/index.php/c-17-new-algorithm-of-the-standard-template-library>
(good comparison to haskell; this article leads me to:)

hpx: <https://github.com/STEllAR-GROUP/hpx>
<https://en.wikipedia.org/wiki/HPX>

seems the missing gem in the standard library 17!!

another option is Intel's parallel STL + TBB but I hate vendor lock.

### HPX + Boost DevOps

the goal is to:

- vendor hpx and boost 
- strongly prefer local installation (inside the project directory)
- parameterized and automated
- easy to troubleshoot and platform agnostic

read: <https://stellar-group.github.io/hpx-docs/tags/1.4.1/html/quickstart.html#quickstart>

HPX provides brief instructions on how to build and install its dependencies

#### install the latest version of cmake

follow the official guide: <https://cmake.org/install/>
<https://cmake.org/download/> (at the time of writing, I'm using cmake 3.17.1)

```shell
./bootstrap
make -j8
make install
```

using the latest version (that is newer than the boost version) helps to 
avoid this kind of errors:

```text
CMake Warning at /usr/share/cmake-3.10/Modules/FindBoost.cmake:801 (message):
  New Boost version may have incorrect or missing dependencies and imported
  targets
Call Stack (most recent call first):
  /usr/share/cmake-3.10/Modules/FindBoost.cmake:907 (_Boost_COMPONENT_DEPENDENCIES)
  /usr/share/cmake-3.10/Modules/FindBoost.cmake:1558 (_Boost_MISSING_DEPENDENCIES)
  cmake/HPX_SetupBoostFilesystem.cmake:17 (find_package)
  CMakeLists.txt:1621 (include)
```

read: <https://github.com/Microsoft/cpprestsdk/issues/1010#issuecomment-452851468>

#### download and build boost

this has been documented in `vendor/get-boost.sh` script.

at the time of writing I'm using boost 1.72.0

a few notes:

- this likely only works on Ubuntu/Linux; I should make this script platform agnostic
- I don't touch multithreading/single-threading variants on Linux, but I remember having to do that on Windows
- `--build-type=complete` **DOES NOT WORK** therefore I have to call multiple times the b2 script to build different variants
- totally build time is less than 10 minutes

to use this script, do:

```shell
cd vendor
mkdir boost_<version>
./get_boost <version> boost_<version>
```

after this, I can set `BOOST_ROOT` cmake variable to `boost_<version>`:
`cmake -DBOOST_root=boost_<version>`

BOOST_ROOT is documented by cmake: <https://cmake.org/cmake/help/v3.10/module/FindBoost.html>

#### download and build hwloc - portable hardware locality library

site: <https://www-lb.open-mpi.org/projects/hwloc/>
at the time of writing, I'm using 2.2.0

download and untar,
run `./configure --prefix=<local vendor dir>`, then `make -j8 && make install`

for the meaning behind `--prefix` read: <https://stackoverflow.com/questions/11307465/destdir-and-prefix-of-make>

for project that provides autoconf `configure` script, **I should only set 
the prefix (installation dest) once**; I **should not touch DESTDIR when
calling make** as DESTDIR is concatenated after `--prefix`

#### download and build jemalloc - alternative, high performance memory allocator

site: <http://jemalloc.net/>
at the time of writing, I'm using 5.2.1

same building process as above 

note, I need to tell cmake that I want to use jemalloc when building hpx,
i.e. 

```shell
-DHPX_WITH_MALLOC=jemalloc -DJEMALLOC_ROOT=<jemalloc inst dir>
```

#### download and build hpx

site: <http://hpx.stellar-group.org/>
I can get its release tarball from its github mirror;

here is the cmake command:

```shell
cmake \
-DBOOST_ROOT=/home/wein/work/tmp/boost_dist \
-DHWLOC_ROOT=/home/wein/work/tmp/hwloc_dist \
-DHPX_WITH_MALLOC=jemalloc \
-DJEMALLOC_ROOT=/home/wein/work/tmp/jemalloc_dist ..
```

note I have to run `make -j8 tests` to **actually build all the unit tests**!
then running `make test` (or ctest) won't throw massive amount of errors

also note that I need to use `CMAKE_INSTALL_PREFIX` while building hpx,
see: <https://cmake.org/cmake/help/v3.0/variable/CMAKE_INSTALL_PREFIX.html>
this is equivalent to `./configure --prefix=<dir>`

## Boost linking does right

while building hpx runtime (a shared library) I got this error, saying
I could not build this shared library from the statically linked boost
libraries. I must set `-fPIC`.

this reminded me my previous research on symbol-hidding and clean-build.

### how to set fPIC with boost's b2 build system

`./b2 -j 12 cxxflags='-fPIC' cflags='-fPIC' --link='static'`

there are some references but no answers this question directly:

<https://boostorg.github.io/build/manual/develop/index.html>

this explains `-j 12` - it does make compiling boost faster:

<https://stackoverflow.com/questions/33566782/building-all-of-boost-in-a-few-minutes>

### how to set fPIC with automake / configure

`./configure CFLAGS=-fPIC CXXFLAGS=-fPIC`

see: <https://stackoverflow.com/questions/629961/how-can-i-set-ccshared-fpic-while-executing-configure>

### how to verify the resulting static libs ARE indeed built with fPIC

there is no direct method. I find that the quickest way is to create
a minimal toy project that produces a shared library which is linked
with the boost static component.

if fPIC was given, gcc will happily produce the shared lib; otherwise
it throws the same error I got with hpx.

### motivation behind all these

why am I so fascinated with boost's static libraries??

see: <https://stackoverflow.com/questions/27848105/how-to-force-compilation-of-boost-to-use-fpic>

the answer in this post is outdated (this one is based on bjam), but the
concept is the same

### symbol hiding and clean build, again

see `cxxCleanBuild` for the previous research;

the goal is to make sure, if client has a version of boost that is
either statically, or dynamically linked with the client's runtime,
my library (which also has "a copy of boost") won't cause symbol
clashing.

this leads to some review on weak symbol, the meaning behind `W w u t`
in `nm`, see below:

<https://www.quora.com/What-are-strong-and-weak-symbols-in-C>

eventually I remember that, GCC offers `fvisibility=hidden` to simplify
this task, see:

<http://anadoxin.org/blog/control-over-symbol-exports-in-gcc.html>

and there is even a demo:

<https://labjack.com/news/simple-cpp-symbol-visibility-demo>

version script is a legacy from the past, but back in the days at Wt, this was
the only way to hide symbols:

<https://stackoverflow.com/questions/22102470/link-a-static-library-to-a-shared-library-and-hide-exported-symbols>

### symbol hiding in CMake

the Cmake's way of setting `fvisibility=hidden` is to use a DVar:

<https://stackoverflow.com/questions/17080869/what-is-the-cmake-equivalent-to-gcc-fvisibility-hidden-when-controlling-the-e>

`set(CMAKE_CXX_VISIBILITY_PRESET hidden)`

### the unfinished question... does symbol hiding work or not

I can still see the boost symbols (mingled) in `nm`, which show up
as either weak symbols or in the text section (what does that mean?)

the toy project is:

```c++
#include <boost/timer/timer.hpp>
#include <cmath>

extern "C" int compute()
{
    boost::timer::auto_cpu_timer t;

    for ( long i = 0; i < 100000000; ++i )
        std::sqrt( 123.456L );  // burn some time

    return 0;
}
```

```cmake
cmake_minimum_required(VERSION 3.10)
project(test LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_NO_SYSTEM_PATHS TRUE)
set(Boost_USE_STATIC_LIBS TRUE)
set(BOOST_ROOT /tmp/boost_1_72_0)
find_package(Boost 1.72.0 REQUIRED COMPONENTS timer)

add_library(cow SHARED cow.cpp)
target_link_libraries(cow ${Boost_LIBRARIES})
target_include_directories(cow PRIVATE ${Boost_INCLUDE_DIRS})
set_target_properties(cow PROPERTIES CMAKE_CXX_VISIBILITY_PRESET hidden)

add_library(dud SHARED cow.cpp)
target_link_libraries(dud ${Boost_LIBRARIES})
target_include_directories(dud PRIVATE ${Boost_INCLUDE_DIRS})
set_target_properties(dud PROPERTIES CMAKE_CXX_VISIBILITY_PRESET hidden)
```

I deliberately build two shared library **using the same source code**;
if the `compute` function were in a unique, per-library namespace,
when the client code is linked against both `cow` and `dud`, will there
be symbol clashing???

TODO: I will answer this later.

### GDB to inspect the live process, but it does not work

the closest point to the answer I have got is that, I loaded both shared
libraries via Python's `ctypes.cdll.LoadLibrary()` function and inspected
the Python process's function table in GDB

`info functions <regex>`

it does not see any boost functions, nor does it see the `compute()` function,
the only one that is exported in the shared library.



