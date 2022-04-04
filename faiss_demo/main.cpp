#include <iostream>
#include <string>
#include <grpc/grpc.h>

#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include "common.grpc.pb.h"
#include "common.pb.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include <faiss/index_io.h>
#include <faiss/IndexFlat.h>
#include <faiss/IndexIVFFlat.h>

#include <mutex>

using idx_t = faiss::Index::idx_t;

using grpc::Status;
using grpc::Server;
using grpc::ServerContext;
using grpc::ServerBuilder;

const int DIM = 128;
class FaissIndexImpl final: public FaissIndex::Service {
public:
	explicit FaissIndexImpl(faiss::IndexIVFFlat *index) {
		index_ivf_flat_ = index;
		
	}

	Status Search(ServerContext* context, const SearchRequest* req, SearchResponse* res) override {
		std::lock_guard<std::mutex> lock_guard(index_mu_);
		std::cout << "k is: " << req->k() <<" vector_size:" << req->vec_size()<< std::endl;
		int k = req->k();
		if (k == 0) {
			res->set_msg("unexpect k");
			res->set_code(1);
			return Status::OK;
		}
		if (req->vec_size() != DIM) {
			res->set_msg("unexpect vector length");
			res->set_code(1);
			return Status::OK;
		}

		float *q = new float[DIM];
		for (int i = 0; i < DIM; i++) {
			q[i] = req->vec(i);
		}
	

		float* dis = new float[k];
		idx_t* labels = new idx_t[k];
		if (index_ivf_flat_ != nullptr) {
			index_ivf_flat_->search(1, q, k, dis, labels);
		}
		res->set_msg("ok");
		for (int i = 0; i < k; i++) {
			res->add_distance(dis[i]);
			res->add_labels(labels[i]);
		}
		return Status::OK;
	} 

private:
	faiss::IndexIVFFlat *index_ivf_flat_;
	std::mutex index_mu_;
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
	faiss::IndexIVFFlat *index = dynamic_cast<faiss::IndexIVFFlat*>(
		faiss::read_index("saved_item.idx"));
	start_grpc_server(index);
	return 0;
}