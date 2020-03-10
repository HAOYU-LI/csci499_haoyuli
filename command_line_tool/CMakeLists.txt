cmake_minimum_required(VERSION 3.1)

project(command-line-tool)

# C++ version used
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
include_directories(BEFORE ${GTEST_INCLUDE_DIRS})
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# Finds Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's CMake installation.
set(protobuf_MODULE_COMPATIBLE TRUE)
set(gflags_DIR /usr/local/src/gflags)
find_package(Protobuf REQUIRED)
find_package(glog REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(gflags REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

message(STATUS "Using protobuf ${protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# Hardcodes gRPC installation location
set(_GRPC_GRPCPP_UNSECURE grpc++_unsecure) 
set(_GRPC_CPP_PLUGIN_EXECUTABLE /usr/local/bin/grpc_cpp_plugin)


# Generate target executable for warble
add_executable(warble command_line.cc
    command_line.h
    command_line_helper.cc
    ../key_value_store/key_value_client.cc
    ../key_value_store/key_value_store.pb.cc
    ../key_value_store/key_value_store.grpc.pb.cc
    ../warble/warble_service.pb.cc
    ../warble/warble_service_impl.cc
    ../func/func_client_impl.cc
    ../func/func_service.grpc.pb.cc
    ../func/func_service.pb.cc
    )

target_link_libraries(warble
    ${_GRPC_GRPCPP_UNSECURE}
    ${_PROTOBUF_LIBPROTOBUF}
    gRPC::grpc++
    glog::glog
    gflags)


target_compile_features(warble PRIVATE cxx_std_17)
