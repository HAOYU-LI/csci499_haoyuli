cmake_minimum_required(VERSION 3.1)

project(warble)

get_filename_component(warble_proto "../protos/warble_service.proto" ABSOLUTE)
get_filename_component(warble_proto_path "${warble_proto}" PATH)

# C++ version used
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
include_directories(BEFORE ${GTEST_INCLUDE_DIRS})
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# Finds Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's CMake installation.
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf REQUIRED)
find_package(glog REQUIRED)
find_package(gRPC CONFIG REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

message(STATUS "Using protobuf ${protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# Hardcodes gRPC installation location
set(_GRPC_GRPCPP_UNSECURE grpc++_unsecure) 
set(_GRPC_CPP_PLUGIN_EXECUTABLE /usr/local/bin/grpc_cpp_plugin)

# Generate gRPC code
set(DIST_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(warble_proto_srcs "${DIST_DIR}/warble_service.pb.cc")
set(warble_proto_hdrs "${DIST_DIR}/warble_service.pb.h")
add_custom_command(
      OUTPUT "${warble_proto_srcs}" "${warble_proto_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --cpp_out "${DIST_DIR}"
        -I "${warble_proto_path}"
        "${warble_proto}"
      DEPENDS ${warble_proto})

# Include unit test:
add_executable(unittest_warble_service unittest_warble_service.cc
    warble_service_impl.cc
    ../key_value_store/key_value_client.cc
    ../key_value_store/key_value_store.pb.cc
    ../key_value_store/key_value_store.grpc.pb.cc
    ${warble_proto_srcs})

target_link_libraries(unittest_warble_service PUBLIC
    gtest
    gtest_main
    ${_GRPC_GRPCPP_UNSECURE}
    ${_PROTOBUF_LIBPROTOBUF}
    gRPC::grpc++
    glog::glog)
