cmake_minimum_required(VERSION 3.1)

project(func)

get_filename_component(func_proto "../protos/func_service.proto" ABSOLUTE)
get_filename_component(func_proto_path "${func_proto}" PATH)

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

include_directories(${Protobuf_INCLUDE_DIRS})
message(STATUS "Using protobuf ${protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# Hardcodes gRPC installation location
set(_GRPC_GRPCPP_UNSECURE grpc++_unsecure) 
set(_GRPC_CPP_PLUGIN_EXECUTABLE /usr/local/bin/grpc_cpp_plugin)

#Generate gRPC code
set(DIST_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(func_proto_srcs "${DIST_DIR}/func_service.pb.cc")
set(func_proto_hdrs "${DIST_DIR}/func_service.pb.h")
set(func_grpc_srcs "${DIST_DIR}/func_service.grpc.pb.cc")
set(func_grpc_hdrs "${DIST_DIR}/func_service.grpc.pb.h")
add_custom_command(
      OUTPUT "${func_proto_srcs}" "${func_proto_hdrs}" "${func_grpc_srcs}" "${func_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${DIST_DIR}"
        --cpp_out "${DIST_DIR}"
        -I "${func_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${func_proto}"
      DEPENDS ${func_proto})

# Include generated *.pb.h files
include_directories("${DIST_DIR}")

