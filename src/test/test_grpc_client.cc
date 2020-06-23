#include <iostream>
#include <string>
#include "grpc/grpc.h"
#include "grpc++/grpc++.h"
#include "grpc/channel.h"

#include "test/proto/helloworld.pb.h"
#include "test/proto/helloworld.grpc.pb.h"

using namespace helloworld;

class GreeterClient {
public:
  GreeterClient(std::shared_ptr<grpc::Channel> channel): stub_(Greeter::NewStub(channel))  { }
	void SayHello(std::string message) {
		grpc::ClientContext context;
    HelloRequest request;
    request.set_message(message);
    HelloReply response;
	  if (stub_->SayHello(&context, &request, &response))
			std::cout << "succ request = " << request.message() << ", response = " << response.message() << std::endl;
		} else {
      std::cout << "failed!"
    }
	}
private:
	std::unique_ptr<Greeter::Stub> stub_;
};


int main(int argc, char **argv) {
	auto channel = grpc::CreateChannel("localhost:5000", grpc::InsecureChannelCredentials());
	GreeterClient client(channel);
	client.SayHello("I am wego");
	return 0;
}