HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
CXX = g++
CPPFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++17
ifeq ($(SYSTEM),Darwin)
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++`\
           -pthread\
           -lgrpc++_reflection\
           -ldl\
           -lglog\
           -lgflags\
           -lprotoc\
           -lprotobuf\
           -Igoogle/protobuf
else
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++`\
           -pthread\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl\
           -lglog\
           -lgflags\
           -lgtest
endif
PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

PROTOS_PATH = ../protos

vpath %.proto $(PROTOS_PATH)

all: system-check warble func_service_impl key_value_service warble_service_impl unittest_key_value_data_structure

key_value_service: key_value_store.pb.o key_value_store.grpc.pb.o ../key_value_store/key_value_service.o ../key_value_store/key_value_data_structure.o
	$(CXX) $^ $(LDFLAGS) -o $@

warble_service_impl: key_value_store.pb.o key_value_store.grpc.pb.o ../key_value_store/key_value_client.o

func_service_impl: func_service.pb.o func_service.grpc.pb.o key_value_store.pb.o key_value_store.grpc.pb.o warble_service.pb.o ../warble/warble_service_impl.o ../func/func_service_impl.o ../key_value_store/key_value_client.o
	$(CXX) $^ $(LDFLAGS) -o $@

warble: func_service.pb.o func_service.grpc.pb.o key_value_store.pb.o key_value_store.grpc.pb.o warble_service.pb.o ../func/func_client_impl.o ../command_line_tool/command_line_helper.o ../command_line_tool/command_line.o
	$(CXX) $^ $(LDFLAGS) -o $@

unittest_key_value_data_structure: ../key_value_store/unittest_key_value_data_structure.cc ../key_value_store/key_value_data_structure.o 
	$(CXX) $^ $(LDFLAGS) -o $@

# unittest_warble_service: ../warble/unittest_warble_service.cc ../warble/warble_service_impl.o key_value_store.pb.o key_value_store.grpc.pb.o warble_service.pb.o key_value_store.pb.o key_value_store.grpc.pb.o ../key_value_store/key_value_client.o
# 	$(CXX) $^ $(LDFLAGS) -o $@

%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<

clean:
	rm -f *.o *.pb.cc *.pb.h warble func_service_impl key_value_service unittest_key_value_data_structure ../key_value_store/*.o ../command_line_tool/*.o ../func/*.o ../warble/*.o


# The following is to test your system and ensure a smoother experience.
# They are by no means necessary to actually compile a grpc-enabled software.

PROTOC_CMD = which $(PROTOC)
PROTOC_CHECK_CMD = $(PROTOC) --version | grep -q libprotoc.3
PLUGIN_CHECK_CMD = which $(GRPC_CPP_PLUGIN)
HAS_PROTOC = $(shell $(PROTOC_CMD) > /dev/null && echo true || echo false)
ifeq ($(HAS_PROTOC),true)
HAS_VALID_PROTOC = $(shell $(PROTOC_CHECK_CMD) 2> /dev/null && echo true || echo false)
endif
HAS_PLUGIN = $(shell $(PLUGIN_CHECK_CMD) > /dev/null && echo true || echo false)

SYSTEM_OK = false
ifeq ($(HAS_VALID_PROTOC),true)
ifeq ($(HAS_PLUGIN),true)
SYSTEM_OK = true
endif
endif

system-check:
ifneq ($(HAS_VALID_PROTOC),true)
	@echo " DEPENDENCY ERROR"
	@echo
	@echo "You don't have protoc 3.0.0 installed in your path."
	@echo "Please install Google protocol buffers 3.0.0 and its compiler."
	@echo "You can find it here:"
	@echo
	@echo "   https://github.com/google/protobuf/releases/tag/v3.0.0"
	@echo
	@echo "Here is what I get when trying to evaluate your version of protoc:"
	@echo
	-$(PROTOC) --version
	@echo
	@echo
endif
ifneq ($(HAS_PLUGIN),true)
	@echo " DEPENDENCY ERROR"
	@echo
	@echo "You don't have the grpc c++ protobuf plugin installed in your path."
	@echo "Please install grpc. You can find it here:"
	@echo
	@echo "   https://github.com/grpc/grpc"
	@echo
	@echo "Here is what I get when trying to detect if you have the plugin:"
	@echo
	-which $(GRPC_CPP_PLUGIN)
	@echo
	@echo
endif
ifneq ($(SYSTEM_OK),true)
	@false
endif