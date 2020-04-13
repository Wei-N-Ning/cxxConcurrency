# vendor directory

boost: 1.72.0
hwloc: 2.2.0
jemalloc: 5.2.1

hpx: 1.4.1

## hpx

cmake command

```shell
cmake \
-DBOOST_ROOT=/home/wein/work/dev/cxx/github.com/powergun/cxxConcurrency/vendor/boost-1.72.0 \
-DHWLOC_ROOT=/home/wein/work/dev/cxx/github.com/powergun/cxxConcurrency/vendor/hwloc-2.2.0 \
-DHPX_WITH_MALLOC=jemalloc \
-DJEMALLOC_ROOT=/home/wein/work/dev/cxx/github.com/powergun/cxxConcurrency/vendor/jemalloc-5.2.1 \
-DCMAKE_INSTALL_PREFIX=/home/wein/work/dev/cxx/github.com/powergun/cxxConcurrency/vendor/hpx-1.4.1 ..
```

keep both the installation dir and the source dir in the `vendor` directory

