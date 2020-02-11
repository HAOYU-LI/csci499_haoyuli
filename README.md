# csci499_haoyuli


## Linux

```sh
 $ [sudo] apt-get install build-essential autoconf libtool pkg-config
```

If you plan to build using CMake
```sh
 $ [sudo] apt-get install cmake
```

If you are a contributor and plan to build and run tests, install the following as well:
```sh
 $ # libgflags-dev is only required if building with mpke (deprecated)
 $ [sudo] apt-get install libgflags-dev
 $ # clang and LLVM C++ lib is only required for sanitizer builds
 $ [sudo] apt-get install clang-5.0 libc++-dev
```

## Install gRPC

## Clone the repository (including submodules)

Before building, you need to clone the gRPC github repository and download submodules containing source code
for gRPC's dependencies (that's done by the `submodule` command or `--recursive` flag). The following commands will clone the gRPC
repository at the latest stable version.

```sh
 $ git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc
 $ cd grpc
 $ git submodule update --init
 ```

## Building with CMake

In grpc repo
```sh
$ mkdir -p cmake/build
$ cd cmake/build
$ cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DgRPC_PROTOBUF_PROVIDER=package -DgRPC_ZLIB_PROVIDER=package -DgRPC_CARES_PROVIDER=package -DgRPC_SSL_PROVIDER=package -DCMAKE_BUILD_TYPE=Release ../..
$ make -j4 install
$ cd ../..
```
