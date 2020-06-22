#include <grpc++/grpc++.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc/grpc.h>

#include <iostream>

#include "test/proto/helloworld.grpc.pb.h"


using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class SearchRequestImpl final : public helloworld::Greeter::Service {
  ::grpc::Status SayHello(::grpc::ServerContext* context,
                          const ::helloworld::HelloRequest* request,
                          ::helloworld::HelloReply* response) {
    response->set_message("response: " + request->message());
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:8080");
  SearchRequestImpl service;
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}