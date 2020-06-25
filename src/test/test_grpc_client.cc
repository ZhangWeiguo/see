#include <stdio.h>

#include <iostream>
#include <string>

#include "gflags/gflags.h"
#include "grpc++/channel.h"
#include "grpc++/grpc++.h"
#include "grpc/grpc.h"
#include "test/proto/helloworld.grpc.pb.h"
#include "test/proto/helloworld.pb.h"


using namespace helloworld;

DEFINE_string(grpc_port, "8080", "GRPC PORT");

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(Greeter::NewStub(channel)) {}
  void SayHello(std::string message) {
    grpc::ClientContext context;
    HelloRequest request;
    request.set_message(message);
    HelloReply response;
    if (stub_->SayHello(&context, request, &response).ok()) {
      std::cout << "succ request = " << request.message()
                << ", response = " << response.message() << std::endl;
    } else {
      std::cout << "failed!";
    }
  }

 private:
  std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char **argv) {
  std::string port = "localhost:" + FLAGS_grpc_port;
  auto channel = grpc::CreateChannel(port, grpc::InsecureChannelCredentials());
  GreeterClient client(channel);
  client.SayHello("I am wego");
  return 0;
}