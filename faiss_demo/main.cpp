#include <iostream>
#include <string>
#include <grpc/grpc.h>

#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include "common.grpc.pb.h"
#include "common.pb.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include <faiss/IndexFlat.h>
#include <faiss/IndexIVFFlat.h>

using idx_t = faiss::Index::idx_t;

using grpc::Status;
using grpc::Server;
using grpc::ServerContext;
using grpc::ServerBuilder;
class FaissIndexImpl final: public FaissIndex::Service {
public:
	explicit FaissIndexImpl(faiss::IndexIVFFlat *index) {
		index_ivf_flat_ = index;
		
	}

	Status Search(ServerContext* context, const SearchRequest* req, SearchResponse* res) override {
		std::cout << "k is: " << req->k() << std::endl;
		res->set_msg("hello world");
		res->set_code(1);
		res->add_distance(1.0);
		res->add_labels(1);
		return Status::OK;
	} 

private:
	faiss::IndexIVFFlat *index_ivf_flat_;
};

void start_grpc_server(faiss::IndexIVFFlat *index) {
	std::string server_address("0.0.0.0:10001");
	FaissIndexImpl service(index);
	
	grpc::reflection::InitProtoReflectionServerBuilderPlugin();

	ServerBuilder builder;

	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

	builder.RegisterService(&service);

	std::unique_ptr<Server> server(builder.BuildAndStart());

	std::cout << "Server Listening on" << server_address << std::endl;
	server->Wait();

	
}
int main() {
	IndexIVFFlat *index = dynaimic_cast<IndexIVFFlat*>(faiss::read_index("saved_item.idx"));
	start_grpc_server(&index);
	return 0;
}