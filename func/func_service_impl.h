#ifndef FUNC_SERVICE_IMPL_H
#define FUNC_SERVICE_IMPL_H
#include <grpcpp/grpcpp.h>

#include "func_service.pb.h"
#include "func_service.grpc.pb.h"

using func::HookRequest;
using func::HookReply;
using func::UnhookRequest;
using func::UnhookReply;
using func::EventRequest;
using func::EventReply;
using grpc::Server;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using grpc::StatusCode;

namespace func {
class FuncServiceImpl final : public FuncService::Service {
  // The hook method Intended to allow a service (not a user) to specify that 
	// a certain piece of code (a specific function) should be used 
	// to process messages of a specific type.  
  Status hook(ServerContext* context, 
  	           const HookRequest* request, HookReply* response) override;
  // The unhook method Intended to remove relationship between
  // a specific type and a certain piece of code.
  Status unhook(ServerContext* context, 
  	           const UnhookRequest* request, UnhookReply* response) override;
  // The event method is responsible for executing the given function 
  // that was hooked previously for that type of event and responding 
  // with the appropriate reply message  (if there exists a hooked function 
  // to process that type of event -- if not, throw an error).
  Status event(ServerContext* context, 
  	           const EventRequest* request, EventReply* response) override;
}
}// End of func namespace

#endif