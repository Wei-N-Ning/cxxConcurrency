# C++ Concurrency and Parallelism

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


