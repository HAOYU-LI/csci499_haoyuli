cmake_minimum_required(VERSION 3.1)

project(kvstore)

get_filename_component(store_proto "../protos/key_value_store.proto" ABSOLUTE)
get_filename_component(store_proto_path "${store_proto}" PATH)

# C++ version used
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
include_directories(BEFORE ${GTEST_INCLUDE_DIRS})
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# Finds Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's CMake installation.
set(protobuf_MODULE_COMPATIBLE TRUE)
#set(glog_DIR ../third_party/glog)
find_package(Protobuf REQUIRED)
find_package(glog REQUIRED)
find_package(gRPC CONFIG REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

include_directories(${Protobuf_INCLUDE_DIRS})
message(STATUS "Using protobuf ${protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
# set(_PROTOBUF_LIBPROTOBUF libprotobuf.a)
# set(_PROTOBUF_PROTOC protoc>)

# Hardcodes gRPC installation location
set(_GRPC_GRPCPP_UNSECURE grpc++_unsecure) 
# set(_GRPC_GRPCPP_UNSECURE /usr/local/lib/libgrpc_unsecure.a)  
set(_GRPC_CPP_PLUGIN_EXECUTABLE /usr/local/bin/grpc_cpp_plugin)

#Generate gRPC code
set(DIST_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(store_proto_srcs "${DIST_DIR}/key_value_store.pb.cc")
set(store_proto_hdrs "${DIST_DIR}/key_value_store.pb.h")
set(store_grpc_srcs "${DIST_DIR}/key_value_store.grpc.pb.cc")
set(store_grpc_hdrs "${DIST_DIR}/key_value_store.grpc.pb.h")
add_custom_command(
      OUTPUT "${store_proto_srcs}" "${store_proto_hdrs}" "${store_grpc_srcs}" "${store_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${DIST_DIR}"
        --cpp_out "${DIST_DIR}"
        -I "${store_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${store_proto}"
      DEPENDS ${store_proto})

# Include generated *.pb.h files
include_directories("${DIST_DIR}")

set(client_interface_hdrs "key_value_store_client_interface.h")

#Compile key value data structure as a library
add_library(key_value_data_structure STATIC key_value_data_structure.cc key_value_data_structure.h)

#Add test
add_executable(unittest_key_value_data_structure unittest_key_value_data_structure.cc)
target_link_libraries(unittest_key_value_data_structure PUBLIC
    gtest
    gtest_main
    key_value_data_structure
)

# Compiles key_value_service
add_executable(key_value_service key_value_service.cc
    key_value_service.h
    ${store_proto_srcs}
    ${store_grpc_srcs})

target_link_libraries(key_value_service
    ${_GRPC_GRPCPP_UNSECURE}
    ${_PROTOBUF_LIBPROTOBUF}
    gRPC::grpc++
    glog::glog
    key_value_data_structure)
