#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "common.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class FaissIndexClient {
 public:
  FaissIndexClient(std::shared_ptr<Channel> channel)
      : stub_(FaissIndex::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string Search(const std::string& user) {
    // Data we are sending to the server.
	SearchRequest request;
    request.set_k(1000);
	request.add_vec(1);

    // Container for the data we expect from the server.
    SearchResponse reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->Search(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.msg();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<FaissIndex::Stub> stub_;
};

int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint specified by
  // the argument "--target=" which is the only expected argument.
  // We indicate that the channel isn't authenticated (use of
  // InsecureChannelCredentials()).
  std::string target_str = "localhost:10001";
  FaissIndexClient faissClient(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::string reply = faissClient.Search(user);
  std::cout << "faiss_client received: " << reply << std::endl;

  return 0;
}
