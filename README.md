# csci499_haoyuli

This project builds a software infrastructure for a new Function-as-a-Service (FaaS) platform called Func, and on top of Func we build the infrastructure for a social network platform we’ll call Warble.  Func is intended to provide a very basic FaaS platform like AWS Lambda or Google Cloud Functions, and Warble is intended to have the same basic functionality of Twitter.


## 1. Virual Environment Setup

Install virtual environment.
### 1.1. Download & Install Vagrant 

Visit https://www.vagrantup.com/downloads.html to download and install vagrant(>= 2.2.6)

### 1.2. Start virtual machine
After adding vagrant box. This command will start and login to the virtual machine.

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

### 2.2.1 CMake(prerequisite of GRPC)
```sh
$ wget https://cmake.org/files/v3.17/cmake-3.17.0-rc1.tar.gz
$ tar cmake-3.17.0-rc1.tar.gz 
$ cd cmake-3.17.0-rc1
$ ./configure --prefix=/usr/local
$ [sudo] make
$ [sudo] make install
$ cd ..
``` 

### 2.2.2 GRPC

```sh
$ cd csci499_haoyuli/third_party
$ [sudo] apt-get update
$ [sudo] apt-get install build-essential autoconf libtool pkg-config libc-ares-dev automake golang
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
$ [sudo] cmake ../..
$ [sudo] make
$ cd ..
```

### 2.2.3 glog

```sh
$ cd csci499_haoyuli/third_party
$ [sudo] git clone https://github.com/google/glog.git
$ cd glog
$ [sudo] ./autogen.sh
$ [sudo] ./configure
$ [sudo] cmake .
$ [sudo] make
$ [sudo] make install
$ cd ..
```

### 2.2.4 googletest

```sh
$ cd csci499_haoyuli/third_party
$ [sudo] git clone https://github.com/google/googletest.git
cd googletest
$ [sudo] cmake .
$ [sudo] make
$ [sudo] make install
$ cd ..
```

### 2.2.5 gflags

```sh
$ cd csci499_haoyuli/third_party
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

Persistent storage mode can be activated by setting store flag. Once activated, key value
server will load commands from log file and execute sequentially. Users' input will also be saved into designated log file.
```sh
$ ./key_value_service --store "[LOG_FILE]"
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

## 4. Samples to run
Go to command-line-tool repo

```sh
$ cd command-line-tool
$ ./warble --registeruser "user1" # Register a user called user1.
$ ./warble --registeruser "user2" # Register a user called user2.
$ ./warble --user "user1" --warble "hello from user1" # A warble is posted by user1
$ ./warble --user "user2" --warble "hello from user2" --reply "<ID_OF_WARBLE_TO_REPLY>" # Reply to a warble
$ ./warble --user "user1" --read "<ID_OF_WARBLE_TO_READ>" # Read a warble thread that ends at warble id.
$ ./warble --user "user1" --follow "user2" # User1 starts to follow user2
$ ./warble --user "user1" --profile # Show following and followers of user1.

```

## 5. Unit test
### 5.1 Unit test for key_value_store
```sh
$ cd key_value_store
$ ./unittest_key_value_data_structure
```

### 5.2 Unit test for warble methods
```sh
$ cd warble
$ ./unittest_warble_service
```

