# csci499_haoyuli

This project builds a software infrastructure for a new Function-as-a-Service (FaaS) platform called Func, and on top of Func we build the infrastructure for a social network platform we’ll call Warble.  Func is intended to provide a very basic FaaS platform like AWS Lambda or Google Cloud Functions, and Warble is intended to have the same basic functionality of Twitter.


## 1. Virual Environment Setup

Install virtual environment if you are not using linux.
### 1.1. Download & Install Vagrant 

Visit https://www.vagrantup.com/downloads.html to download and install vagrant(>= 2.2.6)

### 1.2. Create virtual environment
The commands below create a directory vagrant-env and a file Vagrantfile within the new directory.

```sh
$ cd ~/ && mkdir vagrant-env
$ cd vagrant-env && vagrant init vagrant-csci499
```

### 1.3. Start virtual machine
This command will start and login to the virtual machine.

```sh
$ vagrant up && vagrant ssh
```

## 2. Project Environment Setup

### 2.1 Clone project to local

```sh
$ cd /vagrant
$ git clone https://github.com/HAOYU-LI/csci499_haoyuli.git
```

### 2.2 Add third-party lib

```sh
$ cd csci499_haoyuli
$ mkdir third_party
$ cd third_party
```
### 2.2.1 Protocol Buffers(prerequisite of GRPC)
```sh
$ [sudo] apt-get install autoconf automake libtool curl make g++ unzip
$ [sudo] git clone https://github.com/protocolbuffers/protobuf.git
$ cd protobuf
$ [sudo] git submodule update --init --recursive
$ [sudo] ./autogen.sh
$ ./configure
$ [sudo] make
$ [sudo] make check
$ [sudo] make install
$ [sudo] ldconfig # refresh shared library cache.

```

### 2.2.2 abseil-cpp(prerequisite of GRPC)
```sh
$ [sudo] git clone https://github.com/abseil/abseil-cpp.git
$ cd abseil-cpp
$ [sudo] cmake .
$ [sudo] make
$ [sudo] make install
$ cd ..
```

### 2.2.3 GRPC

```sh
$ [sudo] apt-get update
$ [sudo] apt-get install build-essential autoconf libtool pkg-config libc-ares-dev golang
$ [sudo] apt-get install cmake
$ [sudo] apt-get install libprotobuf-dev protobuf-compiler
```

#### GRPC-Clone the repository (including submodules)

Before building, you need to clone the gRPC github repository and download submodules containing source code
for gRPC's dependencies (that's done by the `submodule` command or `--recursive` flag). The following commands will clone the gRPC
repository at the latest stable version.

```sh
 $ [sudo] git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc
 $ cd grpc
 $ [sudo] git submodule update --init
 ```

#### GPRC-Building with CMake

In grpc repo
```sh
$ mkdir -p cmake/build
$ cd cmake/build
$ [sudo] cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DgRPC_PROTOBUF_PROVIDER=package -DgRPC_ZLIB_PROVIDER=package -DgRPC_CARES_PROVIDER=package -DgRPC_SSL_PROVIDER=package -DgRPC_ABSL_PROVIDER=package -DCMAKE_BUILD_TYPE=Release ../..
$ [sudo] make -j4 install
$ cd ../..
$ [sudo] make
$ [sudo] make install
$ cd ../
```

### 2.2.4 glog

```sh
$ [sudo] git clone https://github.com/google/glog.git
$ cd glog
$ [sudo] ./autogen.sh
$ [sudo] ./configure
$ [sudo] make
$ [sudo] make install
$ cd ..
```

### 2.2.5 googletest

```sh
$ [sudo] git clone https://github.com/google/googletest.git
cd googletest
$ [sudo] cmake .
$ [sudo] make
$ [sudo] make install
$ cd ..
```

### 2.2.6 gflags

```sh
$ [sudo] git clone https://github.com/gflags/gflags.git
$ [sudo] cmake .
$ [sudo] make
$ [sudo] make install
$ cd ../..
```

## 3. Build & Run project

### 3.1 Build and run key_value_server
```sh
$ cd key_value_store
$ cmake .
$ make
$ ./key_value_service
```

### 3.2 Compile warbler service
Create another terminal window. Run following commands.
```sh
$ cd warble
$ cmake .
$ make
$ cd ..
```

### 3.3 Build and run func server
```sh
$ cd func
$ cmake .
$ make
$ ./func_service_impl
```

### 3.4 Compile command-line interface
```sh
$ cd command-line-tool
$ cmake .
$ make
```

## 4. Sample execution

```sh
$ ./warble --registeruser "user1" # Register a user called user1.
$ ./warble --registeruser "user2" # Register a user called user2.
$ ./warble --user "user1" --warble "hello from user1" # A warble is posted by user1
$ ./warble --user "user2" --warble "hello from user2" --reply "<ID_OF_WARBLE_TO_REPLY>" # Reply to a warble
$ ./warble --user "user1" --follow "user2" # User1 starts to follow user2
$ ./warble --user "user1" --profile # Show following and followers of user1.

```

